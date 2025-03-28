#include "EventHandlers.h"
#include "Config.h"
#include <Arduino.h>

void handleCommand1A(const Packet &pkt) {
    Serial.println(">>> Event: CMD 0x1A received!");
}

void handleCommand5B(const Packet &pkt) {
    Serial.println(">>> Event: CMD 0x5B received!");
}

void globalPacketHandler(const Packet &pkt) {
    Serial.print("Packet: FROM 0x");
    Serial.print(pkt.sender, HEX);
    Serial.print(" TO 0x");
    Serial.print(pkt.receiver, HEX);
    Serial.print(" CMD 0x");
    Serial.print(pkt.command, HEX);
    Serial.print(" DATA ");

    for (uint8_t i = 0; i < pkt.length; i++) {
        Serial.print("0x");
        Serial.print(pkt.data[i], HEX);
        Serial.print(" ");
    }

    Serial.print("CRC 0x");
    Serial.println(pkt.crc, HEX);

    displayPacket(pkt);
}



void handleResetCommand(const char *input) {
    Serial.println("RESET: Restarting system...");
    ESP.restart();
}



void handleStatusCommand(const char *input) {
    Serial.print("STATUS: System is running. ");

    uint8_t data[] = {0x00}; 
    sendPacket(DIA, LCM, DSREQ, data, sizeof(data));
}

void handleBusStatusCommand(const char *input) {
    Serial.print("bus ");

    uint8_t data[] = {0x00}; 
    sendPacket(DIA, LCM, BS, data, sizeof(data));
}

void handleReadMemCommand(const char *input) {
    Serial.print("read ");

    uint8_t data[] = {0x00}; 
    sendPacket(DIA, LCM, DRM, data, sizeof(data));
}





void handleReadIdentityCommand(const char *input) {
    Serial.print("Read Identity ");

    sendPacket(DIA, LCM, 0x00, nullptr, 0);
}

void handleReadCodingDataCommand(const char *input) {
    Serial.print("Read coding data ");

    uint8_t data[] = {0x00}; 
    sendPacket(DIA, LCM, 0x08, data, sizeof(data));
}


void handleReadErrorsCommand(const char *input) {
    Serial.print("Read Errors ");

    uint8_t data[] = {0x00}; 
    sendPacket(DIA, LCM, 0x04, data, sizeof(data));
}

void handleReadErrorsCommand2(const Packet &pkt) {
    // нужно проверить массив pkt.data на соответствие  {0x02}
    if (pkt.length == 1 && pkt.data[0] == 0x02) {
        Serial.print("read errors......");

        uint8_t data1[] = {0x01}; 
        sendPacket(DIA, LCM, 0x04, data1, sizeof(data1));
    }
}
