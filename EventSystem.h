#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "Packet.h"

typedef void (*EventHandler)(const Packet &pkt);
typedef void (*StringEventHandler)(const char *input);

void registerEvent(uint8_t sender, uint8_t receiver, uint8_t command, EventHandler handler);
void setGlobalEvent(EventHandler handler);
void triggerEvents(const Packet &pkt);

// Регистрация строкового события (по текстовому триггеру)
void registerStringEvent(const char *trigger, StringEventHandler handler);
void triggerStringEvents(char *input);

#endif
