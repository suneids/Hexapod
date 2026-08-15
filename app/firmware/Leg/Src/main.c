//REVESION FOR MK1 - 1 MCU for 2 legs
#include "config.h"
#include "activities.h"

int main(void)
{
    LEG_Init();

    SetBase();

    SysTick_Delay(1000);

    while(1)
    {
//    	SetBase();
//        Таз
//        SweepJoint(COXA_SetAngleDeg, coxa1, -45, 45, 1, 20);
//        COXA_SetAngleDeg(coxa1, 0);
//        SysTick_Delay(500);

//        // Бедро
//        SweepJoint(FEMUR_SetAngleDeg, femur1, -45, 45, 1, 20);
//        FEMUR_SetAngleDeg(femur1, 0);
//        SysTick_Delay(500);
//
//        // Голень
//        SweepJoint(TIBIA_SetAngleDeg, -45, 45, 1, 20);
//        TIBIA_SetAngleDeg(0);
//        SysTick_Delay(500);
    }
}
