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
