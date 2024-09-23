#ifndef WebSocketServer_h
#define WebSocketServer_h

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> 

extern AsyncWebServer server;
extern AsyncWebSocket ws;

void notifyClients();
void processWebSocketMessage(void *arg, uint8_t *data, size_t len);
void initWebSocket();

#endif 