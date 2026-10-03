#include "config.h"
GPIO_Pin_t coxa1  = { .port = GPIOA, .number = 3},
		   femur1 = { .port = GPIOA, .number = 1},
		   tibia1 = { .port = GPIOA, .number = 2},

		   coxa2  = { .port = GPIOA, .number = 8},
		   femur2 = { .port = GPIOA, .number = 9},
		   tibia2 = { .port = GPIOA, .number = 10};


LegMotion_t leg1 = {0};
LegMotion_t leg2 = {0};
