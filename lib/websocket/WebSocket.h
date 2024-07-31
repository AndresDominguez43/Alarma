#ifndef WebSocket_h
#define WebSocket_h

#include <ESPAsyncWebServer.h>

extern AsyncWebSocket ws;

void initWebSocket();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void  handleWebSocketClients();
void notifyClients();
#endif