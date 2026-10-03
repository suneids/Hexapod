#include <QByteArray>
#include <cstdint>

uint16_t crc16(const uint8_t *data, uint16_t len){
    uint16_t crc = 0xFFFF;

    for(uint16_t i = 0; i < len; i++) {
        crc ^= data[i];

        for(uint8_t j = 0; j < 8; j++) {
            if(crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

QByteArray makePacket(uint8_t id, uint8_t cmd, const QByteArray &payload){
    QByteArray body;

    body.append(static_cast<char>(id));
    body.append(static_cast<char>(cmd));
    body.append(static_cast<char>(payload.size()));
    body.append(payload);

    uint16_t crc = crc16(
        reinterpret_cast<const uint8_t*>(body.constData()),
        static_cast<uint16_t>(body.size())
        );

    QByteArray pkt;

    pkt.append(static_cast<char>(0xAA));
    pkt.append(static_cast<char>(0x55));

    pkt.append(body);

    pkt.append(static_cast<char>(crc & 0xFF));
    pkt.append(static_cast<char>((crc >> 8) & 0xFF));

    return pkt;
}
