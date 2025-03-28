#ifndef EVENT_HANDLERS_H
#define EVENT_HANDLERS_H

#include "SerialHandler.h"
#include "Display.h"

void handleCommand1A(const Packet &pkt);
void handleCommand5B(const Packet &pkt);
void globalPacketHandler(const Packet &pkt);

void handleStatusCommand(const char *input);
void handleBusStatusCommand(const char *input);
void handleReadMemCommand(const char *input);
void handleResetCommand(const char *input);

void handleReadIdentityCommand(const char *input);
void handleReadCodingDataCommand(const char *input);
void handleReadErrorsCommand(const char *input);
void handleReadErrorsCommand2(const Packet &pkt);

#endif // EVENT_HANDLERS_H
