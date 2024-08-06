#include <WiFi.h>
#include <WebServer.h>
#include <WebSocket.h>
#include <manejoAlarma.h>

void setup(){
  Serial.begin(115200);
  configWiFi();
  configMDNS();
  initWebSocket();
  initWebServer();
  initWebServer();
  startTimeClient();
}

void loop(){
  handleWebSocketClients();
  updateTime();
  checkAlarms();
}