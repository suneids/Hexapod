#include "activities.h"
#include "HAL_STM32F103C6T6/inc/fdcan.h"
#define CAN_ID_LEG_COMMAND  0x100u
static GaitState_t gait_state = GAIT_IDLE;
static uint32_t gait_timer = 0;



static void PackInt16(uint8_t *dst, int16_t value){
	uint16_t v = (uint16_t)value;

	dst[0] = (uint8_t)(v & 0xFFu);
	dst[1] = (uint8_t)(v >> 8u);
}


static uint8_t LegPacketChecksum(const uint8_t *data){
	uint8_t checksum = 0u;

	for(uint8_t i = 0; i < 7u; i++){
		checksum ^= data[i];
	}

	return checksum;
}


static void Tripod_Set(const uint8_t tripod[3], int16_t coxa, int16_t femur, int16_t tibia){
	for(uint8_t i = 0; i < 3; i++){
		LEG_SetAngles(tripod[i], coxa, femur, tibia);
	}
}


void BRAIN_Init(){
	SysTick_Init();
	FDCAN_Init(FDCAN1, 500000u);
	USART_Init(USART2, 9600, 0, 1);
	GPIO_PinMode(brain_led);
	GPIO_DigitalWrite(brain_led, 0);
}


void LEG_SetAngles(uint8_t leg, int16_t coxa, int16_t femur, int16_t tibia){

	uint8_t data[8];

	data[0] = leg;

	PackInt16(&data[1], coxa*coxa_sign[leg-1]);
	PackInt16(&data[3], femur);
	PackInt16(&data[5], tibia);

	data[7] = LegPacketChecksum(data);

	FDCAN_Send(FDCAN1, CAN_ID_LEG_COMMAND, data, 8);
}


void SPIDER_StepBase(){
	if(gait_state != GAIT_IDLE)
		return;

	Tripod_Set(
		tripod_A,
		COXA_CENTER,
		FEMUR_GROUND,
		TIBIA_GROUND
	);

	Tripod_Set(
		tripod_B,
		COXA_CENTER,
		FEMUR_GROUND,
		TIBIA_GROUND
	);

}


void SPIDER_StepForward(){
	if(gait_state != GAIT_IDLE) return;

	gait_state = GAIT_FORWARD_A_LIFT;
	gait_timer = millis();
}


void SPIDER_StepBackward(){

}


void SPIDER_StepLeft(){

}


void SPIDER_StepRight(){

}


void SPIDER_GaitUpdate(void){

	if(gait_state == GAIT_IDLE){
		return;
	}

	if((millis() - gait_timer) < GAIT_PHASE_TIME){
		return;
	}

	gait_timer = millis();


	switch(gait_state){
	case GAIT_FORWARD_A_LIFT:
			/*
			 * A отрываем от земли.
			 * B пока держит паука.
			 */
			Tripod_Set(tripod_A, COXA_CENTER, FEMUR_LIFT, TIBIA_LIFT);
			gait_state = GAIT_FORWARD_A_SWING;
			break;
		case GAIT_FORWARD_A_SWING:
			/*
			 * A в воздухе -> вперёд.
			 */
			Tripod_Set(tripod_A, COXA_FORWARD, FEMUR_LIFT, TIBIA_LIFT);
			gait_state = GAIT_FORWARD_A_DOWN;
			break;
		case GAIT_FORWARD_A_DOWN:
			/*
			 * Поставили A на землю.
			 */
			Tripod_Set(tripod_A, COXA_FORWARD, FEMUR_GROUND, TIBIA_GROUND);
			gait_state = GAIT_FORWARD_B_LIFT;
			break;
		case GAIT_FORWARD_B_LIFT:
			/*
			 * Поднимаем B.
			 */
			Tripod_Set(tripod_B, COXA_CENTER, FEMUR_LIFT, TIBIA_LIFT);
			gait_state = GAIT_FORWARD_B_SWING;
			break;
		case GAIT_FORWARD_B_SWING:
			/*
			 * B в воздухе -> вперёд.
			 */
			Tripod_Set(tripod_B, COXA_FORWARD, FEMUR_LIFT, TIBIA_LIFT);
			gait_state = GAIT_FORWARD_B_DOWN;
			break;
		case GAIT_FORWARD_B_DOWN:

			/*
			 * Ставим B.
			 *
			 * После этого:
			 * все ноги повернуты вперед, корпус не сдвинулся
			 */
			Tripod_Set(tripod_B, COXA_FORWARD, FEMUR_GROUND, TIBIA_GROUND);
			gait_state = GAIT_FORWARD_PUSH;
			break;
		case GAIT_FORWARD_PUSH:
			/*
			 * Толкаем корпус вперед, возвращаясь на исходную
			 */
			Tripod_Set(tripod_A, COXA_CENTER, FEMUR_GROUND, TIBIA_GROUND);
			Tripod_Set(tripod_B, COXA_CENTER, FEMUR_GROUND, TIBIA_GROUND);
			gait_state = GAIT_IDLE;
			break;
		default:
			gait_state = GAIT_IDLE;
			break;
	}
}


static uint8_t move_command = 0;
static uint32_t last_command_time = 0;


//LegAngles_t Leg_IK(float x, float y, float z){
//
//}
