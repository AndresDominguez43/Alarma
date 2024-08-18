#include <WiFi.h>
#include <AsyncTCP.h> //Websocket
#include <ESPAsyncWebServer.h> //WebSocket
#include <SPIFFS.h>
#include "ConfigWiFi.h"
#include "DNS.h"
#include "Alarm.h"

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

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.println("Nuevo cliente WebSocket conectado");
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.println("Cliente WebSocket desconectado");
  } else if (type == WS_EVT_DATA) {
    String message = "";
    for (size_t i = 0; i < len; i++) {
      message += (char)data[i];
    }
    Serial.println("Mensaje recibido: " + message);
    // Envía una respuesta de vuelta al cliente
    client->text("Mensaje recibido: " + message);
  }
}


void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}


void handleSetTime(AsyncWebServerRequest *request) {
  if (request->hasParam("time", true)) {
    targetTime = request->getParam("time", true)->value();
    Serial.println("Time set to: " + targetTime); // Mostrar el valor de targetTime en el monitor serial
    request->send(200, "text/plain", "Time set to: " + targetTime);
  } else {
    request->send(400, "text/plain", "Time parameter missing");
  }
}
void WebArchiveSPIFFS(){
if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file = SPIFFS.open("/index.html");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS,"/index.html","text/html");
  });
  
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS,"/script.js","application/javascript");
  });
  
  server.on("/style.css", HTTP_GET,[](AsyncWebServerRequest *request){
    request->send(SPIFFS,"/style.css","text/css");
  });
  
}
void setup() {
  Serial.begin(115200);
  ConfigPin();
  initWiFi();
  DNS();
  WebArchiveSPIFFS();
  server.on("/set-time", HTTP_POST, handleSetTime);

  initWebSocket();
  server.begin();
  timeClient.begin();
}

void loop() {
  ws.cleanupClients();
  timeClient.update();
  Alarm();
  notifyClients();
  delay(1000); 
}


