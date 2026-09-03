#include "HAL_STM32F103C6T6/inc/mcu_config.h"
#include "HAL_STM32F103C6T6/inc/tim.h"
#include "HAL_STM32F103C6T6/inc/usart.h"
#include <stdint.h>

#ifndef CONFIG_H
#define CONFIG_H

#define COXA_CENTER     0

#define COXA_FORWARD   25
#define COXA_BACKWARD -25

#define FEMUR_GROUND    60
#define TIBIA_GROUND    30

#define FEMUR_LIFT     45
#define TIBIA_LIFT     30

#define GAIT_PHASE_TIME 250

#define CMD_MOVE       0x10u

#define MOVE_NONE      0x00u
#define MOVE_W         (1u << 0)
#define MOVE_S         (1u << 1)
#define MOVE_A         (1u << 2)
#define MOVE_D         (1u << 3)
#define MOVE_BASE      (1u << 4)





typedef struct{
    uint8_t move;
    uint8_t speed;
} ControlPacket_t;

typedef struct{
    float vx;
    float vy;
    float wz;
} BodyVelocity_t;

typedef struct {
    int16_t coxa;
    int16_t femur;
    int16_t tibia;
    uint8_t seq;
    uint8_t flags;
} LegCommand_t;

typedef enum{
	LEG_LF = 1,   // left front
	LEG_LM = 2,   // left middle
	LEG_LR = 3,   // left rear

	LEG_RF = 6,   // right front
	LEG_RM = 5,   // right middle
	LEG_RR = 4    // right rear
} LegID_t;

typedef enum{
	GAIT_IDLE = 0,

	GAIT_FORWARD_A_LIFT,
	GAIT_FORWARD_A_SWING,
	GAIT_FORWARD_A_DOWN,

	GAIT_FORWARD_B_LIFT,
	GAIT_FORWARD_B_SWING,
	GAIT_FORWARD_B_DOWN,

	GAIT_FORWARD_PUSH

} GaitState_t;

extern const uint8_t tripod_A[3];
extern const uint8_t tripod_B[3];
extern const int8_t coxa_sign[6];

extern GPIO_Pin_t brain_led;

#endif
