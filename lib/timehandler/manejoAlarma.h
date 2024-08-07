#ifndef manejoAlarma_h
#define manejoAlarma_h

#include <NTPClient.h>
//#include "WebSocket.h"

extern NTPClient timeClient;
extern String targetTime;

void startTimeClient();
void updateTime();
void checkAlarms();
void notifyClients();

#endif
