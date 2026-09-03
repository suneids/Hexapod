#include "control.h"
#include "activities.h"

#include "HAL_STM32F103C6T6/inc/usart.h"
#include "HAL_STM32F103C6T6/inc/tim.h"


#define CONTROL_USART           USART2
#define CONTROL_MAX_PAYLOAD     16u
#define CONTROL_TIMEOUT_MS      200u


typedef enum{
	RX_WAIT_AA = 0,
	RX_WAIT_55,
	RX_ID,
	RX_CMD,
	RX_LEN,
	RX_PAYLOAD,
	RX_CRC_LO,
	RX_CRC_HI
} ControlRxState_t;


static ControlRxState_t rx_state = RX_WAIT_AA;

static uint8_t rx_id = 0u;
static uint8_t rx_cmd = 0u;
static uint8_t rx_len = 0u;
static uint8_t rx_payload[CONTROL_MAX_PAYLOAD];
static uint8_t rx_index = 0u;

static uint16_t rx_crc = 0xFFFFu;
static uint16_t received_crc = 0u;

static uint8_t move_command = MOVE_NONE;
static uint32_t last_command_time = 0u;


static uint16_t CRC16_Update(uint16_t crc, uint8_t data){

	crc ^= data;

	for(uint8_t i = 0u; i < 8u; i++){

		if(crc & 1u){
			crc = (crc >> 1u) ^ 0xA001u;
		}
		else{
			crc >>= 1u;
		}
	}

	return crc;
}


static void Control_ResetParser(void){

	rx_state = RX_WAIT_AA;

	rx_id = 0u;
	rx_cmd = 0u;
	rx_len = 0u;
	rx_index = 0u;

	rx_crc = 0xFFFFu;
	received_crc = 0u;
}


static void Control_ProcessPacket(void){

	if(rx_id != DEVICE_SPIDER){
		return;
	}

	if(rx_cmd == CMD_MOVE){

		if(rx_len != 1u){
			return;
		}

		move_command =
			rx_payload[0] &
			(MOVE_W | MOVE_S | MOVE_A | MOVE_D | MOVE_BASE);

		last_command_time = millis();

	}
}


static void Control_ParseByte(uint8_t byte){

	switch(rx_state){
		case RX_WAIT_AA:
			if(byte == 0xAAu){
				rx_state = RX_WAIT_55;
			}
			break;
		case RX_WAIT_55:
			if(byte == 0x55u){
				rx_crc = 0xFFFFu;
				rx_state = RX_ID;
			}
			else if(byte != 0xAAu){

				rx_state = RX_WAIT_AA;
			}
			break;
		case RX_ID:
			rx_id = byte;
			rx_crc = CRC16_Update(rx_crc, byte);
			rx_state = RX_CMD;
			break;
		case RX_CMD:
			rx_cmd = byte;
			rx_crc = CRC16_Update(rx_crc, byte);
			rx_state = RX_LEN;
			break;
		case RX_LEN:
			rx_len = byte;
			rx_crc = CRC16_Update(rx_crc, byte);
			rx_index = 0u;
			if(rx_len > CONTROL_MAX_PAYLOAD){
				Control_ResetParser();
			}
			else if(rx_len == 0u){
				rx_state = RX_CRC_LO;
			}
			else{
				rx_state = RX_PAYLOAD;
			}
			break;
		case RX_PAYLOAD:
			rx_payload[rx_index++] = byte;
			rx_crc = CRC16_Update(rx_crc, byte);
			if(rx_index >= rx_len){
				rx_state = RX_CRC_LO;
			}
			break;
		case RX_CRC_LO:
			received_crc = byte;
			rx_state = RX_CRC_HI;
			break;
		case RX_CRC_HI:
			received_crc |= ((uint16_t)byte << 8u);
			if(received_crc == rx_crc){
				Control_ProcessPacket();
			}
			Control_ResetParser();
			break;
		default:
			Control_ResetParser();
			break;
	}
}


void Control_Update(void){
	/*
	 * Выгребаем всё, что пришло от HC-12.
	 */
	while(USART_Available(CONTROL_USART)){
		GPIO_PinToggle(brain_led);
		uint8_t byte = (uint8_t)USART_ReadByte(CONTROL_USART);

		Control_ParseByte(byte);
	}
	/*
	 * Если связь пропала — прекращаем движение.
	 *
	 * ПК должен периодически присылать текущее
	 * состояние кнопок.
	 */
	if((millis() - last_command_time) > CONTROL_TIMEOUT_MS) move_command = MOVE_NONE;

	/*
	 * Отсюда начинается уже существующий
	 * уровень управления походкой.
	 */
	switch(move_command){
		case MOVE_BASE:
			SPIDER_StepBase();
			break;
		case MOVE_W:
			SPIDER_StepForward();
			break;
		case MOVE_S:
			SPIDER_StepBackward();
			break;
		case MOVE_A:
			SPIDER_StepLeft();
			break;
		case MOVE_D:
			SPIDER_StepRight();
			break;
		case (MOVE_W | MOVE_A):
			// потом WA
			break;
		case (MOVE_W | MOVE_D):
			// потом WD
			break;
		case (MOVE_S | MOVE_A):
			// потом SA
			break;
		case (MOVE_S | MOVE_D):
			// потом SD
			break;
		case MOVE_NONE:
		default:
			break;
	}
}
