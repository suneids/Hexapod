#include "activities.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define COXA_PWM_MID_US       1470
#define COXA_PWM_FORWARD_US   1000
#define COXA_PWM_BACKWARD_US  2000
#define COXA_DEG_POS_MAX       45
#define COXA_DEG_NEG_MAX       45

#define FEMUR_PWM_MID_US      1470
#define FEMUR_PWM_FORWARD_US   400
#define FEMUR_PWM_BACKWARD_US 2350
#define FEMUR_DEG_POS_MAX      90
#define FEMUR_DEG_NEG_MAX      90

#define TIBIA_PWM_MID_US      1470
#define TIBIA_PWM_FORWARD_US   400
#define TIBIA_PWM_BACKWARD_US 2350
#define TIBIA_DEG_POS_MAX      90
#define TIBIA_DEG_NEG_MAX      90

#ifndef LEG_MOTION_PERIOD_MS
#define LEG_MOTION_PERIOD_MS 10u
#endif
#ifndef COXA_STEP_DEG
#define COXA_STEP_DEG 2.0f
#endif
#ifndef FEMUR_STEP_DEG
#define FEMUR_STEP_DEG 2.0f
#endif
#ifndef TIBIA_STEP_DEG
#define TIBIA_STEP_DEG 2.0f
#endif



typedef struct {
    float coxa, femur, tibia;
    float coxa_target, femur_target, tibia_target;
    uint8_t active;
} LegMotion_t;

static GaitState_t gait_state = GAIT_IDLE;
static volatile MoveCommand_t requested_move = MOVE_STOP;
static LegMotion_t motion[6];
static uint32_t gait_timer = 0;
static uint32_t gait_update_timer = 0;
static uint8_t next_tripod = 0;
static float gait_direction = 1.0f;

/* Адреса существующих GPIO_Pin_t: глобальная инициализация допустима в C.
   Если массив с таким именем уже есть в другом .c, оставь только одно определение.
   Если он объявлен extern в .h, тип должен совпадать. */

static float MoveTowards(float current, float target, float step){
    if(current < target){
        current += step;
        if(current > target) current = target;
    }
    else if(current > target){
        current -= step;
        if(current < target) current = target;
    }
    return current;
}

/* Проверяем программные углы, а не фактическое положение сервоприводов. */
static bool LEG_MotionComplete(void){
    for(uint8_t i = 0; i < 6u; i++){
        const LegMotion_t *m = &motion[i];
        if(!m->active) continue;
        if(m->coxa != m->coxa_target ||
           m->femur != m->femur_target ||
           m->tibia != m->tibia_target){
            return false;
        }
    }
    return true;
}

/* TIM1/15/17 на APB2, TIM2/3/4/5 на APB1.
   Частота таймеров APB2 вычисляется из PCLK2 и делителя APB2. */
void BRAIN_Init(void){
    SysTick_Init();
    USART_Init(USART3, 9600, 0, 1);
    GPIO_PinMode(brain_led);
    GPIO_DigitalWrite(brain_led, 0);

    uint32_t psc1 = RCC_GetTIMPclk1_Hz() / 1000000u - 1u;
    uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;
    uint32_t timclk2 = RCC_GetPclk2_Hz() * ((ppre2 >= 4u) ? 2u : 1u);
    uint32_t psc2 = timclk2 / 1000000u - 1u;

    TIM_Init(TIM1,  psc2, 19999, 0);
    TIM_Init(TIM2,  psc1, 19999, 0);
    TIM_Init(TIM3,  psc1, 19999, 0);
    TIM_Init(TIM4,  psc1, 19999, 0);
    TIM_Init(TIM5,  psc1, 19999, 0);
    TIM_Init(TIM15, psc2, 19999, 0);
    TIM_Init(TIM17, psc2, 19999, 0);

    for(uint8_t i = 0; i < 6u; i++){
        leg_pos[i] = home;
        gait_start_pos[i] = home;
        for(uint8_t j = 0; j < 3u; j++){
            PWM_Init(*legs_pins[i][j]);
        }
    }
}

void COXA_SetAngleDeg(GPIO_Pin_t coxa, float deg){
    uint32_t value = COXA_PWM_MID_US;
    if(deg >  COXA_DEG_POS_MAX) deg =  COXA_DEG_POS_MAX;
    if(deg < -COXA_DEG_NEG_MAX) deg = -COXA_DEG_NEG_MAX;
    if(deg > 0.0f)
        value = COXA_PWM_MID_US + deg * (COXA_PWM_FORWARD_US - COXA_PWM_MID_US) / COXA_DEG_POS_MAX;
    else if(deg < 0.0f)
        value = COXA_PWM_MID_US + deg * (COXA_PWM_MID_US - COXA_PWM_BACKWARD_US) / COXA_DEG_NEG_MAX;
    PWM_Write(coxa, value);
}

void FEMUR_SetAngleDeg(GPIO_Pin_t femur, float deg){
    uint32_t value = FEMUR_PWM_MID_US;
    if(deg >  FEMUR_DEG_POS_MAX) deg =  FEMUR_DEG_POS_MAX;
    if(deg < -FEMUR_DEG_NEG_MAX) deg = -FEMUR_DEG_NEG_MAX;
    if(deg > 0.0f)
        value = FEMUR_PWM_MID_US + deg * (FEMUR_PWM_FORWARD_US - FEMUR_PWM_MID_US) / FEMUR_DEG_POS_MAX;
    else if(deg < 0.0f)
        value = FEMUR_PWM_MID_US + deg * (FEMUR_PWM_MID_US - FEMUR_PWM_BACKWARD_US) / FEMUR_DEG_NEG_MAX;
    PWM_Write(femur, value);
}

void TIBIA_SetAngleDeg(GPIO_Pin_t tibia, float deg){
    uint32_t value = TIBIA_PWM_MID_US;
    if(deg >  TIBIA_DEG_POS_MAX) deg =  TIBIA_DEG_POS_MAX;
    if(deg < -TIBIA_DEG_NEG_MAX) deg = -TIBIA_DEG_NEG_MAX;
    if(deg > 0.0f)
        value = TIBIA_PWM_MID_US + deg * (TIBIA_PWM_FORWARD_US - TIBIA_PWM_MID_US) / TIBIA_DEG_POS_MAX;
    else if(deg < 0.0f)
        value = TIBIA_PWM_MID_US + deg * (TIBIA_PWM_MID_US - TIBIA_PWM_BACKWARD_US) / TIBIA_DEG_NEG_MAX;
    PWM_Write(tibia, value);
}

void LEG_SetAngles(uint8_t leg, int16_t coxa, int16_t femur, int16_t tibia){
    if(leg < 1u || leg > 6u) return;

    uint8_t i = leg - 1u;
    LegMotion_t *m = &motion[i];
    m->coxa_target  = (float)(coxa * coxa_sign[i]);
    m->femur_target = (float)femur;
    m->tibia_target = (float)tibia;

    if(!m->active){
        m->coxa = m->coxa_target;
        m->femur = m->femur_target;
        m->tibia = m->tibia_target;
        m->active = 1u;
    }

    leg_angles[i].coxa = coxa;
    leg_angles[i].femur = femur;
    leg_angles[i].tibia = tibia;
}

void Leg_Motion_Update(void){
    static uint32_t timer = 0;
    uint32_t now = millis();
    if((uint32_t)(now - timer) < LEG_MOTION_PERIOD_MS) return;
    timer = now;

    for(uint8_t i = 0; i < 6u; i++){
        LegMotion_t *m = &motion[i];
        if(!m->active) continue;
        m->coxa  = MoveTowards(m->coxa,  m->coxa_target,  COXA_STEP_DEG);
        m->femur = MoveTowards(m->femur, m->femur_target, FEMUR_STEP_DEG);
        m->tibia = MoveTowards(m->tibia, m->tibia_target, TIBIA_STEP_DEG);
        COXA_SetAngleDeg(*legs_pins[i][0], m->coxa);
        FEMUR_SetAngleDeg(*legs_pins[i][1], m->femur);
        TIBIA_SetAngleDeg(*legs_pins[i][2], m->tibia);
    }
}

bool LEG_IK(float x, float y, float z, LegAngles_t *a){
    float R = sqrtf(x * x + y * y);
    if(R < L_COXA) return false;

    float r = R - L_COXA;
    float d2 = r * r + z * z;
    float D = (d2 - L_FEMUR * L_FEMUR - L_TIBIA * L_TIBIA) /
              (2.0f * L_FEMUR * L_TIBIA);
    if(D < -1.0f || D > 1.0f) return false;

    float q3 = acosf(D);
    float q2 = atan2f(z, r) - atan2f(L_TIBIA * sinf(q3), L_FEMUR + L_TIBIA * cosf(q3));
    float q1 = atan2f(y, x);

    a->coxa = q1 * RAD_TO_DEG;
    a->femur = q2 * RAD_TO_DEG;
    a->tibia = q3 * RAD_TO_DEG;

    if(a->coxa < COXA_MIN || a->coxa > COXA_MAX) return false;
    if(a->femur < FEMUR_MIN || a->femur > FEMUR_MAX) return false;
    if(a->tibia < TIBIA_MIN || a->tibia > TIBIA_MAX) return false;
    return true;
}

bool LEG_SetPosition(uint8_t leg, float x, float y, float z){
    if(leg < 1u || leg > 6u) return false;

    LegAngles_t a;
    if(!LEG_IK(x, y, z, &a)) return false;

    LEG_SetAngles(leg, (int16_t)lroundf(a.coxa), (int16_t)lroundf(a.femur), (int16_t)lroundf(a.tibia));

    leg_pos[leg - 1u].x = x;
    leg_pos[leg - 1u].y = y;
    leg_pos[leg - 1u].z = z;
    return true;
}

void SPIDER_StepBase(void){
    if(gait_state != GAIT_IDLE) return;
    requested_move = MOVE_STOP;

    for(uint8_t leg = 1u; leg <= 6u; leg++){
        LEG_SetPosition(leg, home.x, home.y, home.z);
    }
    next_tripod = 0u;
}

/* Новая команда немедленно заменяет предыдущую, но текущая фаза продолжается. */
void SPIDER_StepForward(void){
    requested_move = MOVE_FORWARD;
}

void SPIDER_StepBackward(void){
    requested_move = MOVE_BACKWARD;
}

void SPIDER_Stop(void){
    requested_move = MOVE_STOP;
}

/* Пока не реализованы: оставлены для совместимости с существующим API. */
void SPIDER_StepLeft(void){
}

void SPIDER_StepRight(void){
}

void SPIDER_GaitUpdate(void){
    uint32_t now = millis();

    if(gait_state == GAIT_IDLE){
        if(requested_move == MOVE_STOP || !LEG_MotionComplete()) return;

        for(uint8_t i = 0; i < 6u; i++){
            gait_start_pos[i] = leg_pos[i];
        }

        gait_direction = (requested_move == MOVE_FORWARD) ? 1.0f : -1.0f;
        gait_timer = now;
        gait_update_timer = now - GAIT_UPDATE_TIME;
        gait_state = (next_tripod == 0u) ? GAIT_A_SWING : GAIT_B_SWING;
    }

    if((uint32_t)(now - gait_update_timer) < GAIT_UPDATE_TIME) return;
    gait_update_timer = now;

    float phase = (float)(now - gait_timer) / (float)GAIT_HALF_TIME;
    if(phase > 1.0f) phase = 1.0f;

    const uint8_t *swing_tripod = (gait_state == GAIT_A_SWING) ? tripod_A : tripod_B;
    float swing_target =  STEP_HALF * gait_direction;
    float push_target  = -STEP_HALF * gait_direction;

    float lift_phase;
    if(phase < 0.3f){
        float t = phase / 0.3f;
        lift_phase = t * t * (3.0f - 2.0f * t);
    }
    else if(phase > 0.7f){
        float t = (1.0f - phase) / 0.3f;
        lift_phase = t * t * (3.0f - 2.0f * t);
    }
    else{
        lift_phase = 1.0f;
    }

    for(uint8_t i = 0; i < 6u; i++){
        bool swing = false;
        for(uint8_t j = 0; j < 3u; j++){
            if(swing_tripod[j] == i + 1u){
                swing = true;
                break;
            }
        }

        float target_y = swing ? swing_target : push_target;
        float y = gait_start_pos[i].y + (target_y - gait_start_pos[i].y) * phase;
        float z = swing ? LEG_GROUND_Z - STEP_HEIGHT * lift_phase : LEG_GROUND_Z;
        float x2 = LEG_WORK_RADIUS * LEG_WORK_RADIUS - y * y;

        if(x2 < 0.0f || !LEG_SetPosition(i + 1u, sqrtf(x2), y, z)){
            requested_move = MOVE_STOP;
            gait_state = GAIT_IDLE;
            return;
        }
    }

    /* Фаза закончена только после программного достижения целевых углов.
       При STOP робот больше не начинает следующую фазу. */
    if(phase >= 1.0f && LEG_MotionComplete()){
        next_tripod = (next_tripod == 0u) ? 1u : 0u;
        gait_state = GAIT_IDLE;
    }
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
