#include "activities.h"
#include "../inc/HAL_STM32F103C6T6/inc/mcu_config.h"
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

	//FDCAN_Init(FDCAN, bitrate)
}


void COXA_SetAngleDeg(GPIO_Pin_t coxa, float deg){
	uint32_t value = COXA_PWM_MID_US;
	deg = deg > 45  ?  45 : deg;
	deg = deg < -45 ? -45 : deg;
	if(deg > 0)      value = COXA_PWM_MID_US + deg * (COXA_PWM_FORWARD_US - COXA_PWM_MID_US) / 45;
	else if(deg < 0) value = COXA_PWM_MID_US + deg * (COXA_PWM_MID_US - COXA_PWM_BACKWARD_US) / 45; //угол уже отрицательный поэтому не вычитаем

	PWM_Write(coxa, value);
}


void FEMUR_SetAngleDeg(GPIO_Pin_t femur, float deg){
	uint32_t value = FEMUR_PWM_MID_US;
	deg = deg > 45  ?  45 : deg;
	deg = deg < -45 ? -45 : deg;
	if(deg > 0)      value = FEMUR_PWM_MID_US + deg * (FEMUR_PWM_FORWARD_US - FEMUR_PWM_MID_US) / 45;
	else if(deg < 0) value = FEMUR_PWM_MID_US + deg * (FEMUR_PWM_MID_US - FEMUR_PWM_BACKWARD_US) / 45; //угол уже отрицательный поэтому не вычитаем

	PWM_Write(femur, value);
}

void TIBIA_SetAngleDeg(GPIO_Pin_t tibia, float deg){
	uint32_t value = TIBIA_PWM_MID_US;
	deg = deg > 45  ?  45 : deg;
	deg = deg < -45 ? -45 : deg;
	if(deg > 0)      value = TIBIA_PWM_MID_US + deg * (TIBIA_PWM_FORWARD_US - TIBIA_PWM_MID_US) / 45;
	else if(deg < 0) value = TIBIA_PWM_MID_US + deg * (TIBIA_PWM_MID_US - TIBIA_PWM_BACKWARD_US) / 45; //угол уже отрицательный поэтому не вычитаем

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

    FEMUR_SetAngleDeg(femur1, 0);
    FEMUR_SetAngleDeg(femur2, 0);

    TIBIA_SetAngleDeg(tibia1, 0);
    TIBIA_SetAngleDeg(tibia2, 0);
}
