#include "WebSocket.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "manejoAlarma.h"

AsyncWebSocket ws("/ws");
AsyncWebServer server(80);

void notifyClients(){
     ws.textAll(timeClient.getFormattedTime()); // Asegúrate de incluir el header adecuado para timeClient
}

void initWebSocket() {
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
        case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
    break;
    }
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
        notifyClients();
        }
    }
}

void handleWebSocketClients(){
    ws.cleanupClients();    // Limpia y maneja las conexiones WebSocket activas
}

