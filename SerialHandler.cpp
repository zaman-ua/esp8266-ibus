#include "SerialHandler.h"
#include "Config.h"
#include "Packet.h"


EspSoftwareSerial::UART myPort;

void initSerial() {
    myPort.begin(9600, SWSERIAL_8E1, MYPORT_RX, MYPORT_TX, false);
    if (!myPort) {
        while (1) delay(1000);
    }
}

Packet readSerial() {
    static uint8_t buffer[255];
    static uint8_t bufferIndex = 0;
    static unsigned long lastByteTime = 0;
    const unsigned long timeout = 100;

    while (myPort.available()) {
        byte received = myPort.read();
        unsigned long now = millis();

        if (now - lastByteTime > timeout) {
            bufferIndex = 0; // Начинаем новый пакет
            memset(buffer, 0, sizeof(buffer)); // Очищаем буфер перед новым чтением
        }
        lastByteTime = now;

        if (bufferIndex < 255) {
            buffer[bufferIndex++] = received;
        }

        if (bufferIndex >= 2 && bufferIndex == buffer[1] + 2) {
            Packet pkt = processPacket(buffer, bufferIndex);
            bufferIndex = 0; // Готовимся к следующему пакету
            return pkt;
        }
    }
    return Packet();
}

void sendPacket(uint8_t sender, uint8_t receiver, uint8_t command, const uint8_t *data, uint8_t dataLength) {
    if (dataLength > 251) {
        Serial.println("Error: Data too long!");
        return;
    }

    uint8_t packetSize = dataLength + 5; //  length, receiver, command, crc
    uint8_t packet[packetSize];

    packet[0] = sender;
    packet[1] = dataLength + 3; // receiver, command, dataLength
    packet[2] = receiver;
    packet[3] = command;

    for (uint8_t i = 0; i < dataLength; i++) {
        packet[4 + i] = data[i];
    }

    uint8_t crc = calculateCRC(packet, sizeof(packet) - 1);
    packet[packetSize - 1] = crc;

    // Отправляем пакет
    myPort.write(packet, packetSize);

    // Отладка
    // Serial.print("Sent packet: ");
    // for (uint8_t i = 0; i < packetSize; i++) {
    //     Serial.print("0x");
    //     Serial.print(packet[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println();
}
