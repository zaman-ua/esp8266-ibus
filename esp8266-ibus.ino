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


// Максимум 17 символов
Menu menuOption1 = { utf8To1251("АБВГДЕЁЖЗИЙКЛМН"), &menuCoding, {}, 0 };
Menu menuOption2 = { utf8To1251("ОПРСТУФХЦЧШЩЪЫЬ"), &menuCoding, {}, 0 };
Menu menuOption3= { utf8To1251("ЭЮЯабвгдеёжзийк"), &menuCoding, {}, 0 };
Menu menuOption4 = { utf8To1251("лмнопрстуфхцчшщ"), &menuCoding, {}, 0 };
Menu menuOption5 = { utf8To1251("ъыьэюя"), &menuCoding, {}, 0 };


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
  menuCoding.childrenCount = 5;
}

// Текущее меню
Menu *currentMenu = &menuRoot;

void displayMenu(Menu *menu) {
  // очистка меню
  uint8_t data1[] = { 0xF0, 0x00, 0x20 };
  sendPacket(0x68, 0x3B, 0x21, data1, sizeof(data1));

  // очистка заголовка
  uint8_t data[] = { 0xF0, 0x00, 0x20 };
  sendPacket(0x68, 0x3B, 0xA5, data, sizeof(data));

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
}
