#include "config.h"
#ifndef ACTIVITIES_H
#define ACTIVITIES_H
void LEG_Init();
void SetBase();
void COXA_SetAngleDeg(GPIO_Pin_t coxa, float deg);
void FEMUR_SetAngleDeg(GPIO_Pin_t femur, float deg);
void TIBIA_SetAngleDeg(GPIO_Pin_t tibia, float deg);
typedef void (*JointSetFunc)( GPIO_Pin_t joint, float angle);

static void SweepJoint(JointSetFunc setAngle, GPIO_Pin_t joint,
					   int16_t min_angle, int16_t max_angle, int16_t step, uint32_t delay_ms);

#endif
