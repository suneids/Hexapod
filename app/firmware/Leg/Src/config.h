#include "../Inc/HAL_STM32F103C6T6/inc/pwm.h"
#include "../Inc/HAL_STM32F103C6T6/inc/tim.h"
#include "../Inc/HAL_STM32F103C6T6/inc/fdcan.h"

#define COXA_PWM_MID_US  1470
#define COXA_PWM_FORWARD_US  1000
#define COXA_PWM_BACKWARD_US 2000
#define COXA_DEG_POS_MAX 45
#define COXA_DEG_NEG_MAX 45

#define FEMUR_PWM_MID_US  1470
#define FEMUR_PWM_FORWARD_US  1000
#define FEMUR_PWM_BACKWARD_US 2000
#define FEMUR_DEG_POS_MAX 60
#define FEMUR_DEG_NEG_MAX 60

#define TIBIA_PWM_MID_US  1470
#define TIBIA_PWM_FORWARD_US  1000
#define TIBIA_PWM_BACKWARD_US 2000
#define TIBIA_DEG_POS_MAX 60
#define TIBIA_DEG_NEG_MAX 60

#define LEG_ID1 5
#define LEG_ID2 6

extern GPIO_Pin_t coxa1, femur1, tibia1, coxa2, femur2, tibia2;
