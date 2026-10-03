#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include <stddef.h>
#define DEVICE_HEXAPOD  0x07u
#define CMD_MOVE       0x10u
#define CMD_STATE      0x0A

#define MOVE_NONE      0x00u
#define MOVE_W         (1u << 0)
#define MOVE_S         (1u << 1)
#define MOVE_A         (1u << 2)
#define MOVE_D         (1u << 3)


#define CONTROL_USART           USART2
#define CONTROL_MAX_PAYLOAD     16u
#define CONTROL_TIMEOUT_MS      200u
#define PKT_SOF1 0xAA
#define PKT_SOF2 0x55
#define SPIDER_LEG_COUNT       6u
#define SPIDER_COORDS_PER_LEG  3u
#define SPIDER_STATE_COUNT     (SPIDER_LEG_COUNT * SPIDER_COORDS_PER_LEG)

void Control_Update(void);
void Control_SendCurrentState(const int8_t *data, size_t count);
#endif
