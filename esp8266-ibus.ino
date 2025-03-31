#include <Arduino.h>
#include "SerialHandler.h"
#include "EventSystem.h"
#include "EventHandlers.h"
#include "Display.h"
#include "Config.h"


int counter = 0;

void sendUartCommand(uint8_t command, const char *text) {
    size_t textLen = strlen(text);
    size_t dataSize = 3 + textLen;
    uint8_t data[dataSize];
    
    data[0] = 0xF0;
    data[1] = 0x00;
    data[2] = command;
    memcpy(&data[3], text, textLen);
    
    sendPacket(0xC8, 0x3B, 0x21, data, dataSize);
}

class MenuItem {
public:
    const char *title;
    MenuItem *parent;
    std::vector<MenuItem *> children;
    
    MenuItem(const char *title, MenuItem *parent = nullptr) 
        : title(title), parent(parent) {}
    
    void addChild(MenuItem *child) {
        children.push_back(child);
    }

    // Метод для изменения заголовка
    void setTitle(const char *newTitle) {
        title = newTitle;
    }
};


class MenuManager {
private:
    MenuItem *currentMenu;
public:
    MenuManager(MenuItem *root) : currentMenu(root) {}
    
    void displayMenu() {
        uint8_t data1[] = { 0xF0, 0x00, 0x20 };
        sendPacket(0x68, 0x3B, 0x21, data1, sizeof(data1));
        sendPacket(0x68, 0x3B, 0xA5, data1, sizeof(data1));
        
        for (size_t i = 0; i < currentMenu->children.size(); i++) {
            sendUartCommand(0x40 + i, currentMenu->children[i]->title);
        }
        if (currentMenu->parent) {
            sendUartCommand(0x50, "Назад");
        }
        SendMenuTitile(currentMenu->title);
    }
    
    void navigate(int index) {
        if (index >= 0 && index < currentMenu->children.size()) {
            currentMenu = currentMenu->children[index];
            displayMenu();
        } else if (index == 16 && currentMenu->parent) {
            currentMenu = currentMenu->parent;
            displayMenu();
        }
    }
};

void SendMenuTitile(const char *text) {
    size_t textLen = strlen(text);
    size_t dataSize = 3 + textLen;
    uint8_t data[dataSize];
    
    data[0] = 0xF0;
    data[1] = 0x01;
    data[2] = 0x00;
    memcpy(&data[3], text, textLen);
    
    sendPacket(0xC8, 0x3B, 0xA5, data, dataSize);
}

// Создание меню
MenuItem menuRoot("Main menu");
MenuItem menuSettings("Settings", &menuRoot);
MenuItem menuDiagnostics("Diagnostic", &menuRoot);
MenuItem menuPlayer("Player", &menuRoot);
MenuItem menuCoding("Testing", &menuRoot);

MenuItem menuLang("Language", &menuSettings);
MenuItem menuClock("Clock", &menuSettings);
MenuItem menuLight("Lights", &menuSettings);

MenuItem menuEngine("Engine", &menuDiagnostics);
MenuItem menuGearbox("Transmission", &menuDiagnostics);


MenuItem menuOption1(utf8To1251("АБВГДЕЁЖЗИЙКЛМН"));
MenuItem menuOption2(utf8To1251("ОПРСТУФХЦЧШЩЪЫЬ"));
MenuItem menuOption3(utf8To1251("ЭЮЯабвгдеёжзийк"));
MenuItem menuOption4(utf8To1251("лмнопрстуфхцчшщ"));
MenuItem menuOption5(utf8To1251("ъыьэюя"));

void setupMenu() {
    menuRoot.addChild(&menuSettings);
    menuRoot.addChild(&menuDiagnostics);
    menuRoot.addChild(&menuPlayer);
    menuRoot.addChild(&menuCoding);
    
    menuSettings.addChild(&menuLang);
    menuSettings.addChild(&menuClock);
    menuSettings.addChild(&menuLight);
    
    menuDiagnostics.addChild(&menuEngine);
    menuDiagnostics.addChild(&menuGearbox);

    menuCoding.addChild(&menuOption1);
    menuCoding.addChild(&menuOption2);
    menuCoding.addChild(&menuOption3);
    menuCoding.addChild(&menuOption4);
    menuCoding.addChild(&menuOption5);
}

MenuManager menuManager(&menuRoot);

void handlePressCallMenu(const Packet &pkt) {
    if (pkt.length == 1 && pkt.data[0] == 0x30) {
        Serial.println("PressCallMenu");
        menuManager.displayMenu();
    }
}

void handlePressPressMenu(const Packet &pkt) {
    if (pkt.length == 3 && pkt.data[0] == 0xF0 && pkt.data[1] == 0x00) {
        int input = pkt.data[2];
        Serial.print("Press Menu ");
        Serial.println(input);
        menuManager.navigate(input);
    }
}

void handlemmCommand(const char *input) {
    Serial.println("mm ");
    uint8_t data[] = { 0xF0, 0x00, 0x20 };
    sendPacket(0x68, 0x3B, 0xA5, data, sizeof(data));
    uint8_t data1[] = { 0xF0, 0x00, 0x20 };
    sendPacket(0x68, 0x3B, 0x21, data1, sizeof(data1));
}

void setup() {
    Serial.begin(115200);
    initSerial();
    initDisplay();
    setupMenu();

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
}

void loop() {
    Packet pkt = readSerial();
    if (pkt.valid) {
        triggerEvents(pkt);
    }
    if (Serial.available()) {
        char input[20] = { 0 };
        int len = Serial.readBytesUntil('\n', input, sizeof(input) - 1);
        input[len] = '\0';
        triggerStringEvents(input);
    }
    updateCounter(counter++);


    // Преобразуем counter в строку и соединяем с текстом
    static char newTitle[17];  // Буфер для строки
    snprintf(newTitle, sizeof(newTitle), "Diagnostic: %d", counter);
    menuDiagnostics.setTitle(newTitle);
}
