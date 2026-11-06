#include "config.h"
#include "control.h"
#ifndef ACTIVITIES_H
#define ACTIVITIES_H
void LEG_SetAngles(uint8_t leg, int16_t coxa, int16_t femur, int16_t tibia);
void BRAIN_SendCurrentState(void);
void SPIDER_StepBase();
void SPIDER_StepForward();
void SPIDER_StepBackward();
void SPIDER_StepLeft();
void SPIDER_StepRight();
void SPIDER_Stop(void);
void SPIDER_GaitUpdate(void);
void Control_Update(void);
void Leg_Motion_Update(void);
bool LEG_IK(float x, float y, float z, LegAngles_t *a);
bool LEG_SetPosition(uint8_t leg, float x, float y, float z);
//LegAngles_t Leg_IK(float x, float y, float z);
typedef enum {
    MOVE_STOP,
    MOVE_FORWARD,
    MOVE_BACKWARD
} MoveCommand_t;
void COXA_SetAngleDeg(GPIO_Pin_t coxa, float deg);
void FEMUR_SetAngleDeg(GPIO_Pin_t femur, float deg);
void TIBIA_SetAngleDeg(GPIO_Pin_t tibia, float deg);
void BRAIN_Init(void);

#endif
