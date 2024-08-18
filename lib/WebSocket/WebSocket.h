#ifndef WebSocket_h
#define WebSocket_h

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> 


extern AsyncWebServer server;
extern AsyncWebSocket ws;

void notifyClients();
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void initWebSocket();
void handleSetTime(AsyncWebServerRequest *request);

#endif 