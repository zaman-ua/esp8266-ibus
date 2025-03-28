#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <SoftwareSerial.h>
#include "Packet.h"

extern EspSoftwareSerial::UART myPort;

void initSerial();
Packet readSerial();
void sendPacket(uint8_t sender, uint8_t receiver, uint8_t command, const uint8_t *data, uint8_t dataLength);

#endif
