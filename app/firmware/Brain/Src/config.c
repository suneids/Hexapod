#include "config.h"
//Слева минусовый угол соха - , бедро отрицательное вверх, голень.
//
const uint8_t tripod_A[3] = {
	LEG_LF,
	LEG_LR,
	LEG_RM
};

const uint8_t tripod_B[3] = {
	LEG_RF,
	LEG_RR,
	LEG_LM
};

const int8_t coxa_sign[6] = {
		1, 1, 1, -1, -1, -1
};

GPIO_Pin_t brain_led = {
		.port  = GPIOC,
		.number = 6,
		.moder = GPIO_MODE_OUTPUT,
		.otype = GPIO_OTYPE_PP,
		.pull  = GPIO_NOPULL,
		.speed = GPIO_SPEED_LOW,
		.af    = 0
	};
const LegTransform_t home = {.x = 100, .y = 0, .z = 217.94 };
LegTransform_t leg_pos[6] = {0};
LegAngles_t leg_angles[6] = {0};
