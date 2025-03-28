#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>

struct Packet {
    uint8_t sender;
    uint8_t length;
    uint8_t receiver;
    uint8_t command;
    uint8_t data[251];
    uint8_t crc;
    bool valid = false;
};

Packet processPacket(uint8_t *buffer, uint8_t bufferIndex);
uint8_t calculateCRC(const uint8_t *data, uint8_t length);

#endif
