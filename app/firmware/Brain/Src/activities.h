#include "config.h"
#ifndef ACTIVITIES_H
#define ACTIVITIES_H
void LEG_SetAngles(uint8_t leg, int16_t coxa, int16_t femur, int16_t tibia);
void BRAIN_Init();
void SPIDER_StepBase();
void SPIDER_StepForward();
void SPIDER_StepBackward();
void SPIDER_StepLeft();
void SPIDER_StepRight();
void SPIDER_GaitUpdate(void);
void Control_Update(void);

//LegAngles_t Leg_IK(float x, float y, float z);

#endif
