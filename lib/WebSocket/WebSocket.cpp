#include "WebSocket.h"
#include "Alarm.h"
#include <NTPClient.h>  //Esta inclusion arregla el error al incluir Alarm.h  

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyClients() {
  ws.textAll(timeClient.getFormattedTime());
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

void initWebSocket() {
  server.addHandler(&ws);
}
