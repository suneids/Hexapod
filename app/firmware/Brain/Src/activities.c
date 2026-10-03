#include "activities.h"
#include "HAL_STM32F103C6T6/inc/fdcan.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define CAN_ID_LEG_COMMAND 0x100u

static GaitState_t gait_state = GAIT_IDLE;

static uint32_t gait_timer = 0;
static uint32_t gait_update_timer = 0;

static uint8_t next_tripod = 0;

static LegTransform_t gait_start_pos[6];
static float gait_direction = 1.0f;

static void PackInt16(uint8_t *dst, int16_t value){
    uint16_t v = (uint16_t)value;

    dst[0] = (uint8_t)(v & 0xFFu);
    dst[1] = (uint8_t)(v >> 8u);
}


static uint8_t LegPacketChecksum(const uint8_t *data){
    uint8_t checksum = 0u;

    for(uint8_t i = 0; i < 7u; i++){
        checksum ^= data[i];
    }

    return checksum;
}


static float Lerp(float from, float to, float phase){
    return from + (to - from) * phase;
}


static bool LEG_SetTrajectoryPoint(uint8_t leg, float y, float z){
    float x2 = LEG_WORK_RADIUS * LEG_WORK_RADIUS - y * y;

    if(x2 < 0.0f){
        return false;
    }

    float x = sqrtf(x2);

    return LEG_SetPosition(leg, x, y, z);
}


static float SmoothStep(float t){
    return t * t * (3.0f - 2.0f * t);
}


static bool LEG_Push(uint8_t leg, float target_y, float phase){
    float start_y = gait_start_pos[leg - 1u].y;
    float y = Lerp(start_y, target_y, phase);

    return LEG_SetTrajectoryPoint(leg, y, LEG_GROUND_Z);
}


static bool LEG_Swing(uint8_t leg, float target_y, float phase){
    float start_y = gait_start_pos[leg - 1u].y;
    float y = Lerp(start_y, target_y, phase);

    float lift_phase;

    if(phase < 0.3f){
        lift_phase = SmoothStep(phase / 0.3f);
    }
    else if(phase > 0.7f){
        lift_phase = SmoothStep((1.0f - phase) / 0.3f);
    }
    else{
        lift_phase = 1.0f;
    }

    float z = LEG_GROUND_Z - STEP_HEIGHT * lift_phase;

    return LEG_SetTrajectoryPoint(leg, y, z);
}


static bool Tripod_Push(const uint8_t tripod[3], float target_y, float phase){
    for(uint8_t i = 0; i < 3u; i++){
        if(!LEG_Push(tripod[i], target_y, phase)){
            return false;
        }
    }

    return true;
}


static bool Tripod_Swing(const uint8_t tripod[3], float target_y, float phase){
    for(uint8_t i = 0; i < 3u; i++){
        if(!LEG_Swing(tripod[i], target_y, phase)){
            return false;
        }
    }

    return true;
}


static void SPIDER_StepStart(float direction){
    if(gait_state != GAIT_IDLE){
        return;
    }

    for(uint8_t i = 0; i < 6u; i++){
        gait_start_pos[i] = leg_pos[i];
    }

    gait_direction = direction;

    uint32_t now = millis();
    gait_timer = now;
    gait_update_timer = now;

    if(next_tripod == 0u){
        gait_state = GAIT_A_SWING;
    }
    else{
        gait_state = GAIT_B_SWING;
    }
}


void BRAIN_Init(void){
    SysTick_Init();
    FDCAN_Init(FDCAN1, 500000u);
    USART_Init(USART2, 9600, 0, 1);

    GPIO_PinMode(brain_led);
    GPIO_DigitalWrite(brain_led, 0);

    for(uint8_t i = 0; i < 6u; i++){
        leg_pos[i] = home;
        gait_start_pos[i] = home;
    }
}


void LEG_SetAngles(uint8_t leg, int16_t coxa, int16_t femur, int16_t tibia){
    if(leg < 1u || leg > 6u){
        return;
    }

    uint8_t data[8];

    data[0] = leg;

    PackInt16(&data[1], coxa * coxa_sign[leg - 1u]);
    PackInt16(&data[3], femur);
    PackInt16(&data[5], tibia);

    data[7] = LegPacketChecksum(data);

    FDCAN_Send(FDCAN1, CAN_ID_LEG_COMMAND, data, 8);
    leg_angles[leg-1].coxa = coxa;
    leg_angles[leg-1].femur = femur;
    leg_angles[leg-1].tibia = tibia;
}


bool LEG_SetPosition(uint8_t leg, float x, float y, float z){
    if(leg < 1u || leg > 6u){
        return false;
    }

    LegAngles_t a;

    if(!LEG_IK(x, y, z, &a)){
        return false;
    }

    LEG_SetAngles(
        leg,
        (int16_t)lroundf(a.coxa),
        (int16_t)lroundf(a.femur),
        (int16_t)lroundf(a.tibia)
    );

    leg_pos[leg - 1u].x = x;
    leg_pos[leg - 1u].y = y;
    leg_pos[leg - 1u].z = z;

    return true;
}


void SPIDER_StepBase(void){
    if(gait_state != GAIT_IDLE){
        return;
    }

    for(uint8_t leg = 1u; leg <= 6u; leg++){
        LEG_SetPosition(leg, home.x, home.y, home.z);
    }

    next_tripod = 0u;
}


void SPIDER_StepForward(void){
    SPIDER_StepStart(1.0f);
}


void SPIDER_StepBackward(void){
    SPIDER_StepStart(-1.0f);
}


void SPIDER_StepLeft(void){

}


void SPIDER_StepRight(void){

}


void SPIDER_GaitUpdate(void){
    if(gait_state == GAIT_IDLE){
        return;
    }

    float swing_target = STEP_HALF * gait_direction;
    float push_target = -STEP_HALF * gait_direction;
    uint32_t now = millis();

    if((now - gait_update_timer) < GAIT_UPDATE_TIME){
        return;
    }

    gait_update_timer = now;

    uint32_t elapsed = now - gait_timer;
    float phase = (float)elapsed / (float)GAIT_HALF_TIME;

    if(phase > 1.0f){
        phase = 1.0f;
    }

    bool ok = true;

    switch(gait_state){
		case GAIT_A_SWING:{
			ok = Tripod_Swing(tripod_A, swing_target, phase);

			if(ok){
				ok = Tripod_Push(tripod_B, push_target, phase);
			}

			if(!ok){
				gait_state = GAIT_IDLE;
				return;
			}

			if(phase >= 1.0f){
				next_tripod = 1u;
				gait_state = GAIT_IDLE;
			}

			break;
		}

		case GAIT_B_SWING:{
			ok = Tripod_Swing(tripod_B, swing_target, phase);

			if(ok){
				ok = Tripod_Push(tripod_A, push_target, phase);
			}

			if(!ok){
				gait_state = GAIT_IDLE;
				return;
			}

			if(phase >= 1.0f){
				next_tripod = 0u;
				gait_state = GAIT_IDLE;
			}

			break;
		}
        default:{
            gait_state = GAIT_IDLE;
            break;
        }
    }
}


bool LEG_IK(float x, float y, float z, LegAngles_t *a){
    float R = sqrtf(x * x + y * y);

    if(R < L_COXA){
        return false;
    }

    float r = R - L_COXA;
    float d2 = r * r + z * z;
    float D = (d2 - L_FEMUR * L_FEMUR - L_TIBIA * L_TIBIA) / (2.0f * L_FEMUR * L_TIBIA);

    if(D < -1.0f || D > 1.0f){
        return false;
    }

    float q3 = acosf(D);
    float q2 = atan2f(z, r) - atan2f(L_TIBIA * sinf(q3), L_FEMUR + L_TIBIA * cosf(q3));
    float q1 = atan2f(y, x);

    a->coxa = q1 * RAD_TO_DEG;
    a->femur = q2 * RAD_TO_DEG;
    a->tibia = q3 * RAD_TO_DEG;

    if(a->coxa < COXA_MIN || a->coxa > COXA_MAX){
        return false;
    }

    if(a->femur < FEMUR_MIN || a->femur > FEMUR_MAX){
        return false;
    }

    if(a->tibia < TIBIA_MIN || a->tibia > TIBIA_MAX){
        return false;
    }

    return true;
}


void BRAIN_SendCurrentState(void){
    int8_t state[SPIDER_STATE_COUNT];

    for(uint8_t i = 0; i < SPIDER_LEG_COUNT; i++){
        state[i * 3 + 0] = (int8_t)leg_angles[i].coxa;
        state[i * 3 + 1] = (int8_t)leg_angles[i].femur;
        state[i * 3 + 2] = (int8_t)leg_angles[i].tibia;
    }

    Control_SendCurrentState(state, SPIDER_STATE_COUNT);
}
