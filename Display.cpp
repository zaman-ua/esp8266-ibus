#include "Display.h"
#include "Config.h"


GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

void initDisplay() {
  oled.init(SDA_PIN, SCL_PIN);
  oled.flipH(true);
  oled.flipV(true);
  oled.setScale(1);
  oled.clear();
}

void displayPacket(const Packet &pkt) {
  oled.clear();  // Очистка экрана перед выводом

  oled.setCursor(0, 2);
  oled.print("FROM ");
  oled.print(pkt.sender, HEX);
  oled.print(" TO ");
  oled.print(pkt.receiver, HEX);

  oled.setCursor(0, 3);
  oled.print("CMD ");
  oled.print(pkt.command, HEX);

  oled.setCursor(0, 4);
  oled.print("DATA ");
  oled.autoPrintln(true);

  for (uint8_t i = 0; i < pkt.length; i++) {
    oled.print(pkt.data[i], HEX);
    oled.print(" ");
  }
  oled.autoPrintln(false);

  oled.setCursor(0, 7);
  oled.print("CRC ");
  oled.println(pkt.crc, HEX);

  oled.update();
}

void updateCounter(int counter) {
  oled.setCursor(0, 0);
  oled.print(counter);
  oled.update();
}

const char* utf8To1251(const char* source) {
    static char buffer[256];  // Буфер для результата
    uint8_t n;
    int j = 0;
    
    for (int i = 0; source[i] != '\0' && j < sizeof(buffer) - 1; i++) {
        n = (uint8_t)source[i];

        if (n >= 0xC0) {
            if (n == 0xD0) {
                n = (uint8_t)source[++i];
                if (n == 0x81) n = 0xA8; // Ё
                else if (n >= 0x90 && n <= 0xBF) n += 0x30;
            } else if (n == 0xD1) {
                n = (uint8_t)source[++i];
                if (n == 0x91) n = 0xB8; // ё
                else if (n >= 0x80 && n <= 0x8F) n += 0x70;
            }
        }

        buffer[j++] = (char)n;
    }

    buffer[j] = '\0';  // Завершаем строку
    return buffer;
}
