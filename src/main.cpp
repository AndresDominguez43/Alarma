#include <WiFi.h>
#include <WebServer.h>
#include <WebSocket.h>
#include <manejoAlarma.h>
#include <LittleFS.h>

const char *filename = "index.html";

void setup(){

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