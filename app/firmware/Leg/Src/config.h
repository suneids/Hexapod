#include "HAL_STM32F103C6T6/inc/pwm.h"
#include "HAL_STM32F103C6T6/inc/tim.h"
#include "HAL_STM32F103C6T6/inc/fdcan.h"
#ifndef CONFIG_H
#define CONFIG_H

#define COXA_PWM_MID_US  1470
#define COXA_PWM_FORWARD_US  1000
#define COXA_PWM_BACKWARD_US 2000
#define COXA_DEG_POS_MAX 45
#define COXA_DEG_NEG_MAX 45

#define FEMUR_PWM_MID_US  1470
#define FEMUR_PWM_FORWARD_US  400
#define FEMUR_PWM_BACKWARD_US 2350
#define FEMUR_DEG_POS_MAX 90
#define FEMUR_DEG_NEG_MAX 90

#define TIBIA_PWM_MID_US  1470
#define TIBIA_PWM_FORWARD_US  400
#define TIBIA_PWM_BACKWARD_US 2350
#define TIBIA_DEG_POS_MAX 90
#define TIBIA_DEG_NEG_MAX 90


#define LEG_CONTROLLER_ID 3

#if LEG_CONTROLLER_ID == 1
    #define LEG_ID1 1
    #define LEG_ID2 2
#elif LEG_CONTROLLER_ID == 2
    #define LEG_ID1 3
    #define LEG_ID2 4
#elif LEG_CONTROLLER_ID == 3
    #define LEG_ID1 5
    #define LEG_ID2 6
#endif

typedef struct
{
    float coxa;
    float femur;
    float tibia;

    float coxa_target;
    float femur_target;
    float tibia_target;

    uint8_t active;
} LegMotion_t;

extern LegMotion_t leg1;
extern LegMotion_t leg2;

extern GPIO_Pin_t coxa1, femur1, tibia1, coxa2, femur2, tibia2;

#endif
