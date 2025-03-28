#include "EventSystem.h"

const int MAX_EVENTS = 10;
const int MAX_STRING_EVENTS = 10;

struct Event {
    uint8_t sender;
    uint8_t receiver;
    uint8_t command;
    EventHandler handler;
};

struct StringEvent {
    char trigger[20];  // Строка, которая вызывает событие
    StringEventHandler handler;
};

Event eventList[MAX_EVENTS];
StringEvent stringEventList[MAX_STRING_EVENTS];

int eventCount = 0;
int stringEventCount = 0;

EventHandler globalHandler = nullptr;
StringEventHandler globalStringHandler = nullptr;

void registerEvent(uint8_t sender, uint8_t receiver, uint8_t command, EventHandler handler) {
    if (eventCount < MAX_EVENTS) {
        eventList[eventCount++] = {sender, receiver, command, handler};
    }
}

void setGlobalEvent(EventHandler handler) {
    globalHandler = handler;
}

void triggerEvents(const Packet &pkt) {
    if (!pkt.valid) return;
    
    if (globalHandler) globalHandler(pkt);

    for (int i = 0; i < eventCount; i++) {
        if (eventList[i].sender == pkt.sender &&
            eventList[i].receiver == pkt.receiver &&
            eventList[i].command == pkt.command) {
            eventList[i].handler(pkt);
        }
    }
}

void registerStringEvent(const char *trigger, StringEventHandler handler) {
    if (stringEventCount < MAX_STRING_EVENTS) {
        strncpy(stringEventList[stringEventCount].trigger, trigger, sizeof(stringEventList[stringEventCount].trigger) - 1);
        stringEventList[stringEventCount].trigger[sizeof(stringEventList[stringEventCount].trigger) - 1] = '\0';
        stringEventList[stringEventCount++].handler = handler;
    }
}

void triggerStringEvents(char *input) {
    // Убираем символ возврата каретки \r, если есть
    char *pos;
    if ((pos = strchr(input, '\r')) != NULL) {
        *pos = '\0';
    }

    // Вызываем глобальный обработчик, если он установлен
    if (globalStringHandler) globalStringHandler(input);

    // Проверяем события
    for (int i = 0; i < stringEventCount; i++) {
        if (strcmp(stringEventList[i].trigger, input) == 0) {
            stringEventList[i].handler(input);
        }
    }
}

