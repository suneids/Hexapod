#include "HAL_STM32F103C6T6/inc/mcu_config.h"
#include "HAL_STM32F103C6T6/inc/tim.h"
#include "HAL_STM32F103C6T6/inc/usart.h"
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#ifndef CONFIG_H
#define CONFIG_H


#define COXA_MIN       -45
#define COXA_MAX        45

#define COXA_FORWARD    25
#define COXA_BACKWARD  -25

#define FEMUR_MIN      -90
#define FEMUR_MAX       90

#define TIBIA_MIN      -90
#define TIBIA_MAX       90

#define GAIT_PHASE_TIME 250

#define GAIT_UPDATE_TIME 30u
#define STEP_HALF          45.0f   // стопа ходит от -35 до +35 мм
#define STEP_HEIGHT        35.0f   // подъём стопы
#define GAIT_HALF_TIME     500u    // мс на половину цикла
#define LEG_WORK_RADIUS    100.0f
#define LEG_GROUND_Z       217.94f
#define L_COXA   55.0f
#define L_FEMUR  90.0f
#define L_TIBIA 140.0f
#define RAD_TO_DEG 57.2957795f



#define CMD_MOVE       0x10u

#define MOVE_NONE      0x00u
#define MOVE_W         (1u << 0)
#define MOVE_S         (1u << 1)
#define MOVE_A         (1u << 2)
#define MOVE_D         (1u << 3)
#define MOVE_BASE      (1u << 4)




typedef struct {
    float coxa;
    float femur;
    float tibia;
} LegAngles_t;



typedef struct{
    uint8_t move;
    uint8_t speed;
} ControlPacket_t;

typedef struct{
    float vx;
    float vy;
    float wz;
} BodyVelocity_t;


typedef struct{
    float x;
    float y;
    float z;
} LegTransform_t;


typedef enum{
    GAIT_IDLE = 0,
    GAIT_A_SWING,
    GAIT_B_SWING
} GaitState_t;


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


extern const uint8_t tripod_A[3];
extern const uint8_t tripod_B[3];
extern const int8_t coxa_sign[6];

extern const LegTransform_t home;
extern LegTransform_t leg_pos[6];
extern LegAngles_t leg_angles[6];
extern GPIO_Pin_t brain_led;

#endif
