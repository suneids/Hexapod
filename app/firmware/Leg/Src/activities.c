#include "activities.h"
#include "HAL_STM32F103C6T6/inc/mcu_config.h"
#define CAN_ID_LEG_COMMAND  0x100u
#define LEG_MOTION_PERIOD_MS  10
#define COXA_STEP_DEG         2.0f
#define FEMUR_STEP_DEG        2.0f
#define TIBIA_STEP_DEG        2.0f

static int16_t UnpackInt16(const uint8_t *src){
	uint16_t value = (uint16_t)src[0] | ((uint16_t)src[1] << 8u);
	return (int16_t)value;
}


static uint8_t LegPacketChecksum(const uint8_t *data){
	uint8_t checksum = 0u;
	for(uint8_t i = 0; i < 7u; i++)
		checksum ^= data[i];

	return checksum;
}


void LEG_Init(){
	SysTick_Init();
	uint32_t tim_clk = RCC_GetTIMPclk1_Hz();
	uint32_t psc = (tim_clk / 1000000u) - 1u;

	TIM_Init(TIM2, psc, 19999, 0);
	TIM_Init(TIM1, psc, 19999, 0);


	PWM_Init(coxa1);
	PWM_Init(femur1);
	PWM_Init(tibia1);


	PWM_Init(coxa2);
	PWM_Init(femur2);
	PWM_Init(tibia2);

	FDCAN_Init(FDCAN1, 500000u);
}


void Leg_CAN_Update(void){
	uint32_t can_id;
	uint8_t data[8];
	uint8_t len;
	while(FDCAN_Read(FDCAN1, &can_id, data, &len)){
		if(can_id != CAN_ID_LEG_COMMAND) continue;
		if(len != 8u) continue;
		if(data[7] != LegPacketChecksum(data)) continue;
		uint8_t leg = data[0];
		int16_t coxa = UnpackInt16(&data[1]);
		int16_t femur = UnpackInt16(&data[3]);
		int16_t tibia =	UnpackInt16(&data[5]);

		/*
		 * Здесь конкретная G0-плата знает,
		 * какими двумя ногами она управляет.
		 */

		if(leg == LEG_ID1)
		{
		    leg1.coxa_target  = (float)coxa;
		    leg1.femur_target = (float)femur;
		    leg1.tibia_target = (float)tibia;
		    if(!leg1.active)
		    {
		        leg1.coxa  = coxa;
		        leg1.femur = femur;
		        leg1.tibia = tibia;
		        leg1.active = 1;
		    }
		}
		else if(leg == LEG_ID2)
		{
		    leg2.coxa_target  = (float)coxa;
		    leg2.femur_target = (float)femur;
		    leg2.tibia_target = (float)tibia;
		    if(!leg2.active)
		    {
		        leg2.coxa  = coxa;
		        leg2.femur = femur;
		        leg2.tibia = tibia;
		        leg2.active = 1;
		    }
		}
	}
}

static float MoveTowards(float current, float target, float step)
{
    if(current < target)
    {
        current += step;
        if(current > target) current = target;
    }
    else if(current > target)
    {
        current -= step;
        if(current < target) current = target;
    }

    return current;
}


void Leg_Motion_Update(void)
{
    static uint32_t timer = 0;
    if(!leg1.active && !leg2.active) return;
    if((millis() - timer) < LEG_MOTION_PERIOD_MS)
        return;

    timer = millis();
    if(leg1.active){
		leg1.coxa  = MoveTowards(leg1.coxa,  leg1.coxa_target,  COXA_STEP_DEG);
		leg1.femur = MoveTowards(leg1.femur, leg1.femur_target, FEMUR_STEP_DEG);
		leg1.tibia = MoveTowards(leg1.tibia, leg1.tibia_target, TIBIA_STEP_DEG);

		COXA_SetAngleDeg(coxa1, leg1.coxa);
		FEMUR_SetAngleDeg(femur1, leg1.femur);
		TIBIA_SetAngleDeg(tibia1, leg1.tibia);
    }

    if(leg2.active){
		leg2.coxa  = MoveTowards(leg2.coxa,  leg2.coxa_target,  COXA_STEP_DEG);
		leg2.femur = MoveTowards(leg2.femur, leg2.femur_target, FEMUR_STEP_DEG);
		leg2.tibia = MoveTowards(leg2.tibia, leg2.tibia_target, TIBIA_STEP_DEG);

		COXA_SetAngleDeg(coxa2, leg2.coxa);
		FEMUR_SetAngleDeg(femur2, leg2.femur);
		TIBIA_SetAngleDeg(tibia2, leg2.tibia);
    }
}

void COXA_SetAngleDeg(GPIO_Pin_t coxa, float deg){
	uint32_t value = COXA_PWM_MID_US;
	 deg = deg >  COXA_DEG_POS_MAX ?  COXA_DEG_POS_MAX : deg;
	 deg = deg < -COXA_DEG_NEG_MAX ? -COXA_DEG_NEG_MAX : deg;
	if(deg > 0)      value = COXA_PWM_MID_US + deg * (COXA_PWM_FORWARD_US - COXA_PWM_MID_US) / COXA_DEG_POS_MAX;
	else if(deg < 0) value = COXA_PWM_MID_US + deg * (COXA_PWM_MID_US - COXA_PWM_BACKWARD_US) / COXA_DEG_NEG_MAX; //угол уже отрицательный поэтому не вычитаем

	PWM_Write(coxa, value);
}


void FEMUR_SetAngleDeg(GPIO_Pin_t femur, float deg){
	uint32_t value = FEMUR_PWM_MID_US;
	 deg = deg >  FEMUR_DEG_POS_MAX ?  FEMUR_DEG_POS_MAX : deg;
	 deg = deg < -FEMUR_DEG_NEG_MAX ? -FEMUR_DEG_NEG_MAX : deg;

	if(deg > 0)      value = FEMUR_PWM_MID_US + deg * (FEMUR_PWM_FORWARD_US - FEMUR_PWM_MID_US) / FEMUR_DEG_POS_MAX;
	else if(deg < 0) value = FEMUR_PWM_MID_US + deg * (FEMUR_PWM_MID_US - FEMUR_PWM_BACKWARD_US) / FEMUR_DEG_NEG_MAX; //угол уже отрицательный поэтому не вычитаем

	PWM_Write(femur, value);
}

void TIBIA_SetAngleDeg(GPIO_Pin_t tibia, float deg){
	uint32_t value = TIBIA_PWM_MID_US;
	deg = deg >  TIBIA_DEG_POS_MAX ?  TIBIA_DEG_POS_MAX : deg;
	deg = deg < -TIBIA_DEG_NEG_MAX ? -TIBIA_DEG_NEG_MAX : deg;
	if(deg > 0)      value = TIBIA_PWM_MID_US + deg * (TIBIA_PWM_FORWARD_US - TIBIA_PWM_MID_US) / TIBIA_DEG_POS_MAX;
	else if(deg < 0) value = TIBIA_PWM_MID_US + deg * (TIBIA_PWM_MID_US - TIBIA_PWM_BACKWARD_US) / TIBIA_DEG_NEG_MAX; //угол уже отрицательный поэтому не вычитаем

	PWM_Write(tibia, value);
}


static void SweepJoint(JointSetFunc setAngle,
					   GPIO_Pin_t joint,
                       int16_t min_angle,
                       int16_t max_angle,
                       int16_t step,
                       uint32_t delay_ms)
{
    // -45 -> +45
    for(int16_t angle = min_angle; angle <= max_angle; angle += step)
    {
        setAngle(joint, angle);
        SysTick_Delay(delay_ms);
    }

    // +45 -> -45
    for(int16_t angle = max_angle; angle >= min_angle; angle -= step)
    {
        setAngle(joint, angle);
        SysTick_Delay(delay_ms);

        // защита от возможной вечной прокрутки, если step кривой
        if(angle == min_angle) break;
    }
}


void SetBase(){
    COXA_SetAngleDeg(coxa1, 0);
    COXA_SetAngleDeg(coxa2, 0);

    FEMUR_SetAngleDeg(femur1, 60);
    FEMUR_SetAngleDeg(femur2, 60);

    TIBIA_SetAngleDeg(tibia1, 30);
    TIBIA_SetAngleDeg(tibia2, 30);
}


