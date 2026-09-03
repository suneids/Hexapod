#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>

#define DEVICE_SPIDER  0x07u
#define CMD_MOVE       0x10u

#define MOVE_NONE      0x00u
#define MOVE_W         (1u << 0)
#define MOVE_S         (1u << 1)
#define MOVE_A         (1u << 2)
#define MOVE_D         (1u << 3)

void Control_Update(void);

#endif
