#include <Arduino.h>
#include "SerialHandler.h"
#include "EventSystem.h"
#include "EventHandlers.h"
#include "Display.h"
#include "Config.h"

int counter = 0;

// Определяем структуру для меню
struct Menu {
  const char *title;
  Menu *parent;
  Menu *children[11];  // до 11 пунктов
  uint8_t childrenCount;
};

// Определяем пункты меню
Menu menuRoot = { "Main menu", nullptr, {}, 0 };
Menu menuSettings = { "Settings", &menuRoot, {}, 0 };
Menu menuDiagnostics = { "Diagnostic", &menuRoot, {}, 0 };
Menu menuPlayer = { "Player", &menuRoot, {}, 0 };
Menu menuCoding = { "Testing", &menuRoot, {}, 0 };

Menu menuLang = { "Language", &menuSettings, {}, 0 };
Menu menuClock = { "Clock", &menuSettings, {}, 0 };
Menu menuLight = { "Linghts", &menuSettings, {}, 0 };

Menu menuEngine = { "Engine", &menuDiagnostics, {}, 0 };
Menu menuGearbox = { "Transmission", &menuDiagnostics, {}, 0 };





const char title1[] = { 101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120 };
Menu menuOption1 = { title1, &menuCoding, {}, 0 };

const char title2[] = { 121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140 };
Menu menuOption2 = { title2, &menuCoding, {}, 0 };

const char title3[] = { 141,142,143,143,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160 };
Menu menuOption3= { title3, &menuCoding, {}, 0 };

const char title4[] = { 161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180 };
Menu menuOption4 = { title4, &menuCoding, {}, 0 };

const char title5[] = { 181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200 };
Menu menuOption5 = { title5, &menuCoding, {}, 0 };

const char title6[] = { 201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220 };
Menu menuOption6 = { title6, &menuCoding, {}, 0 };

const char title7[] = { 221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240 };
Menu menuOption7 = { title7, &menuCoding, {}, 0 };

const char title8[] = { 241,242,243,245,246,247,248,249,250,251,252,253,254,255 };
Menu menuOption8 = { title8, &menuCoding, {}, 0 };


// Связываем меню
void setupMenu() {
  menuRoot.children[0] = &menuSettings;
  menuRoot.children[1] = &menuDiagnostics;
  menuRoot.children[2] = &menuPlayer;
  menuRoot.children[3] = &menuCoding;
  menuRoot.childrenCount = 4;

  menuSettings.children[0] = &menuLang;
  menuSettings.children[1] = &menuClock;
  menuSettings.children[2] = &menuLight;
  menuSettings.childrenCount = 3;

  menuDiagnostics.children[0] = &menuEngine;
  menuDiagnostics.children[1] = &menuGearbox;
  menuDiagnostics.childrenCount = 2;

  menuCoding.children[0] = &menuOption1;
  menuCoding.children[1] = &menuOption2;
  menuCoding.children[2] = &menuOption3;
  menuCoding.children[3] = &menuOption4;
  menuCoding.children[4] = &menuOption5;
  menuCoding.children[5] = &menuOption6;
  menuCoding.children[6] = &menuOption7;
  menuCoding.children[7] = &menuOption8;
  menuCoding.childrenCount = 8;
}

// Текущее меню
Menu *currentMenu = &menuRoot;

void displayMenu(Menu *menu) {

  //clear
  uint8_t data[] = { 0xF0, 0x00, 0x00, 0x20 };
  sendPacket(0x68, 0x3B, 0xA5, data, sizeof(data));

  //clear
  uint8_t data1[] = { 0xF0, 0x00, 0x00, 0x20 , 0x20 };
  sendPacket(0x68, 0x3B, 0x21, data1, sizeof(data1));

  for (uint8_t i = 0; i < menu->childrenCount; i++) {
    sendUartCommand(0x40 + i, menu->children[i]->title);
  }
  if (menu->parent) {
    sendUartCommand(0x50, "Назад");
  }
  SendMenuTitile(menu->title);
}

void sendUartCommand(uint8_t command, const char *text) {
  size_t textLen = strlen(text);  // Определяем длину строки
  size_t dataSize = 3 + textLen;  // 3 байта заголовка + текст
  uint8_t data[dataSize];

  // Заполняем массив данными
  data[0] = 0xF0;
  data[1] = 0x00;
  data[2] = command;
  memcpy(&data[3], text, textLen);  // Копируем текст в массив

  sendPacket(0xC8, 0x3B, 0x21, data, dataSize);
}

void SendMenuTitile(const char *text) {
  size_t textLen = strlen(text);  // Определяем длину строки
  size_t dataSize = 3 + textLen;  // 3 байта заголовка + текст
  uint8_t data[dataSize];

  // Заполняем массив данными
  data[0] = 0xF0;
  data[1] = 0x01;
  data[2] = 0x00;
  memcpy(&data[3], text, textLen);  // Копируем текст в массив

  sendPacket(0xC8, 0x3B, 0xA5, data, dataSize);
}


void handlePressCallMenu(const Packet &pkt) {
  if (pkt.length == 1 && pkt.data[0] == 0x30) {
    Serial.println("PressCallMenu");

    displayMenu(currentMenu);
  }
}

void handlePressPressMenu(const Packet &pkt) {
  if (pkt.length == 3 && pkt.data[0] == 0xF0 && pkt.data[1] == 0x00) {
    int input = pkt.data[2];
    Serial.print("Press Menu ");
    Serial.println(input);


    if (input >= 0 && input <= 9) {
      uint8_t index = input;
      if (index < currentMenu->childrenCount) {
        currentMenu = currentMenu->children[index];
        displayMenu(currentMenu);
      }
    } else if (input == 16 && currentMenu->parent) {
      currentMenu = currentMenu->parent;
      displayMenu(currentMenu);
    }
  }
}






void handlemmCommand(const char *input) {
  Serial.println("mm ");


  //clear
  uint8_t data[] = { 0xF0, 0x00, 0x00, 0x20 };
  sendPacket(0x68, 0x3B, 0xA5, data, sizeof(data));

  //clear
  uint8_t data1[] = { 0xF0, 0x00, 0x20 , 0x20 };
  sendPacket(0x68, 0x3B, 0x21, data1, sizeof(data1));
}


void setup() {
  Serial.begin(115200);
  initSerial();
  initDisplay();

  // registerEvent(0x30, 0x80, 0x1A, handleCommand1A);
  // registerEvent(0xD0, 0xBF, 0x5B, handleCommand5B);
  setGlobalEvent(globalPacketHandler);

  registerStringEvent("status", handleStatusCommand);
  registerStringEvent("RESET", handleResetCommand);


  registerStringEvent("bus", handleBusStatusCommand);
  registerStringEvent("read", handleReadMemCommand);



  registerStringEvent("ident", handleReadIdentityCommand);
  registerStringEvent("rcode", handleReadCodingDataCommand);
  registerStringEvent("rerr", handleReadErrorsCommand);

  registerEvent(0xD0, 0x3F, 0xA0, handleReadErrorsCommand2);


  //registerEvent(0x50, 0xC8, 0x3B, handlePressCallMenu);
  registerEvent(0xF0, 0x68, 0x48, handlePressCallMenu);
  registerEvent(0x3B, 0x68, 0x31, handlePressPressMenu);

  registerStringEvent("mm", handlemmCommand);


  setupMenu();
}

void loop() {
  Packet pkt = readSerial();
  if (pkt.valid) {
    triggerEvents(pkt);
  }

  if (Serial.available()) {
    char input[20] = { 0 };  // Очищаем буфер
    int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
    input[len] = '\0';  // Гарантируем, что строка завершена
    triggerStringEvents(input);
  }



  updateCounter(counter++);

  // // D0 07 BF 5B 03 03 08 1C 27
  // uint8_t data[] = {0x03, 0x03, 0x08, 0x1C};
  // sendPacket(0xD0, 0xBF, 0x5B, data);



  // delay(3000);
}
