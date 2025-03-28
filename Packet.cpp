#include "Packet.h"

uint8_t calculateCRC(const uint8_t *data, uint8_t length) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
    }
    return crc;
}

Packet processPacket(uint8_t *buffer, uint8_t bufferIndex) {
    Packet pkt;

    // Длина данных — это второй байт минус 3 (отнимаем адреса и crc)
    uint8_t dataLength = buffer[1] - 3;

    // Выводим полученный буфер
    // Serial.print("Raw buffer: ");
    // for (uint8_t i = 0; i < bufferIndex; i++) {
    //     Serial.print("0x");
    //     Serial.print(buffer[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println();

    // Разбор пакета
    pkt.sender = buffer[0];         // Адрес отправителя
    pkt.length = dataLength;        // Длина данных
    pkt.receiver = buffer[2];       // Адрес получателя
    pkt.command = buffer[3];       // Команда

    // Проверяем длину данных
    if (buffer[1] != (bufferIndex - 2)) {
      Serial.println("Length ERROR!");
      return pkt;  // Проверка, что длина данных совпадает
    }

    pkt.crc = buffer[bufferIndex - 1];  // CRC (последний байт)

    // Проверка контрольной суммы
    if (calculateCRC(buffer, bufferIndex - 1) != pkt.crc) {
        Serial.println("CRC ERROR!");
        return pkt;
    }

    // Считываем данные
    for (uint8_t i = 0; i < dataLength; i++) {
        pkt.data[i] = buffer[i + 4];  // Данные начинаются с 4-го байта
    }

    pkt.valid = true;  // Если все проверки прошли, то пакет валиден
    return pkt;
}

