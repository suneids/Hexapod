//REVESION FOR MK1 - 1 MCU for 2 legs
#include "pwm.h"
#include "tim.h"
#define COXA_PWM_MID_US  1470
#define COXA_PWM_FORWARD_US  1000
#define COXA_PWM_BACKWARD_US 2000
#define COXA_DEG_POS_MAX 45
#define COXA_DEG_NEG_MAX 45

#define FEMUR_PWM_MID_US  1470
#define FEMUR_PWM_FORWARD_US  1000
#define FEMUR_PWM_BACKWARD_US 2000
#define FEMUR_DEG_POS_MAX 45
#define FEMUR_DEG_NEG_MAX 45

#define TIBIA_PWM_MID_US  1470
#define TIBIA_PWM_FORWARD_US  1000
#define TIBIA_PWM_BACKWARD_US 2000
#define TIBIA_DEG_POS_MAX 45
#define TIBIA_DEG_NEG_MAX 45



GPIO_Pin_t coxa1  = { .port = GPIOA, .number = 0},
		   femur1 = { .port = GPIOA, .number = 1},
		   tibia1 = { .port = GPIOA, .number = 2},

		   coxa2  = { .port = GPIOB, .number = 0},
		   femur2= { .port = GPIOB, .number = 1},
		   tibia2 = { .port = GPIOB, .number = 2};


void LEG_Init(){
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	SysTick_Init();
	TIM_Init(TIM2, 7, 20000, 0);
	PWM_Init(coxa1);
	PWM_Init(femur1);
	PWM_Init(tibia1);

	PWM_Init(coxa2);
	PWM_Init(femur2);
	PWM_Init(tibia2);
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


typedef void (*JointSetFunc)(float angle);

static void SweepJoint(JointSetFunc setAngle,
                       int16_t min_angle,
                       int16_t max_angle,
                       int16_t step,
                       uint32_t delay_ms)
{
    // -45 -> +45
    for(int16_t angle = min_angle; angle <= max_angle; angle += step)
    {
        setAngle(angle);
        SysTick_Delay(delay_ms);
    }

    // +45 -> -45
    for(int16_t angle = max_angle; angle >= min_angle; angle -= step)
    {
        setAngle(angle);
        SysTick_Delay(delay_ms);

        // защита от возможной вечной прокрутки, если step кривой
        if(angle == min_angle) break;
    }
}

int main(void)
{
    LEG_Init();

    COXA_SetAngleDeg(coxa1, 0);
    COXA_SetAngleDeg(coxa2, 0);
    FEMUR_SetAngleDeg(femur1, 0);
    FEMUR_SetAngleDeg(femur1, 0);
    TIBIA_SetAngleDeg(tibia1, 0);
    TIBIA_SetAngleDeg(tibia2, 0);

    SysTick_Delay(1000);

    while(1)
    {
//        // Таз
//        SweepJoint(COXA_SetAngleDeg, -45, 45, 1, 20);
//        COXA_SetAngleDeg(0);
//        SysTick_Delay(500);
//
//        // Бедро
//        SweepJoint(FEMUR_SetAngleDeg, -45, 45, 1, 20);
//        FEMUR_SetAngleDeg(0);
//        SysTick_Delay(500);
//
//        // Голень
//        SweepJoint(TIBIA_SetAngleDeg, -45, 45, 1, 20);
//        TIBIA_SetAngleDeg(0);
//        SysTick_Delay(500);
    }
}
