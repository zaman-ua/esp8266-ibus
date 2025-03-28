#ifndef DISPLAY_H
#define DISPLAY_H

#include <GyverOLED.h>
#include "Packet.h"

extern GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

void initDisplay();
void displayPacket(const Packet &pkt);
void updateCounter(int counter);

#endif
