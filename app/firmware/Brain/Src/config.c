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

GPIO_Pin_t leg1_coxa   = {.port  = GPIOB, .number = 9, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},
		  leg1_femur  = {.port  = GPIOB, .number = 8, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},
		  leg1_tibia  = {.port  = GPIOA, .number = 12, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 10},

		  leg2_coxa   = {.port  = GPIOA, .number = 10, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af   = 6},
		  leg2_femur  = {.port  = GPIOA, .number = 9, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 6},
		  leg2_tibia  = {.port  = GPIOA, .number = 8, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 6},

		  leg3_coxa   = {.port  = GPIOB, .number = 15, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 1},
		  leg3_femur  = {.port  = GPIOB, .number = 14, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 1},
		  leg3_tibia  = {.port  = GPIOA, .number = 11, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 11},

		  leg4_coxa   = {.port  = GPIOB, .number = 6, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},
		  leg4_femur  = {.port  = GPIOB, .number = 1, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},
		  leg4_tibia  = {.port  = GPIOB, .number = 0, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},

		  leg5_coxa   = {.port  = GPIOA, .number = 7, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 1},
		  leg5_femur  = {.port  = GPIOA, .number = 6, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},
		  leg5_tibia  = {.port  = GPIOA, .number = 3, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 2},

		  leg6_coxa   = {.port  = GPIOA, .number = 2, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 1},
		  leg6_femur  = {.port  = GPIOA, .number = 1, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 1},
		  leg6_tibia  = {.port  = GPIOA, .number = 0, .moder = GPIO_MODE_AF, .otype = GPIO_OTYPE_PP, .pull  = GPIO_NOPULL, .speed = GPIO_SPEED_LOW, .af    = 1};

GPIO_Pin_t *const legs_pins[6][3] = {
		{ &leg1_coxa, &leg1_femur, &leg1_tibia },
		{ &leg2_coxa, &leg2_femur, &leg2_tibia },
		{ &leg3_coxa, &leg3_femur, &leg3_tibia },
		{ &leg4_coxa, &leg4_femur, &leg4_tibia },
		{ &leg5_coxa, &leg5_femur, &leg5_tibia },
		{ &leg6_coxa, &leg6_femur, &leg6_tibia }
};
const LegTransform_t home = {.x = 100, .y = 0, .z = 217.94 };
LegTransform_t leg_pos[6] = {0};
LegAngles_t leg_angles[6] = {0};
LegTransform_t gait_start_pos[6];
