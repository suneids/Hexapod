#ifndef RADIO_PROTOCOL_H
#define RADIO_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <QByteArray>
#include <QQueue>
#include <QTimer>
#include <functional>



#define PKT_SOF1 0xAA
#define PKT_SOF2 0x55

#define PKT_MAX_DATA 128

#define DEV_PC          0x00
#define DEV_LIGHT       0x01
#define DEV_GREENHOUSE  0x02
#define DEV_HYGROMETER  0x03
#define DEV_HEXAPOD      0x07
#define DEV_BROADCAST   0xFF


#define CMD_GREENHOUSE_STATUS 0x30
#define CMD_GREENHOUSE_FAN_SET         0x31
#define CMD_GREENHOUSE_PUMP_SET        0x32
#define CMD_GREENHOUSE_AUTOWATER_SET   0x33
#define CMD_GREENHOUSE_AUTOVENT_SET    0x34
#define CMD_GREENHOUSE_SOIL_LIMIT_SET  0x35

#define CMD_STATUS_REQUEST   0x70
#define CMD_STATUS_RESPONSE  0x71

#define CMD_HEXAPOD_MOVE      0x10
#define CMD_HEXAPOD_STATE     0x0A

typedef struct {
    uint8_t id;
    uint8_t cmd;
    uint8_t len;
    uint8_t data[PKT_MAX_DATA];
    uint16_t crc;
} RadioPacket_t;

uint16_t crc16(const uint8_t *data, uint16_t len);

QByteArray makePacket(uint8_t id, uint8_t cmd, const QByteArray &payload);

#endif
