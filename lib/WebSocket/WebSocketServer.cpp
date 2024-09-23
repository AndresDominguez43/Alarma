#include "WebSocketServer.h"
#include "Alarm.h"
#include <NTPClient.h>  //Esta inclusion arregla el error al incluir Alarm.h  

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
unsigned long previousTime = 0;
const long interval = 1000;

void notifyClients() {
  unsigned long currentTime = millis();
  if(currentTime - previousTime >interval){
    previousTime = currentTime;
    ws.textAll(timeClient.getFormattedTime());
  }
  
}

void processWebSocketMessage(void *arg, uint8_t *data, size_t len) {  //Maneja los mensajes recibidos de una conexion websocket en el esp32.
  AwsFrameInfo *info = (AwsFrameInfo*)arg; //Crea puntero de tipo AwsFrameInfo para acceder a los mensajes WS
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {  
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {  //Compara el mensaje entrante con la cadena toggle
      notifyClients();  
    } 
  }
}

void initWebSocket() {
  server.addHandler(&ws);
}
