#include "ConfigWiFi.h"
#include "DNS.h"
#include "Alarm.h"
#include "WebSocket.h"
#include "WebSPIFFS.h"


void setup() {
  Serial.begin(115200);

  ConfigPin();

  initWiFi();

  DNS();

  WebArchiveSPIFFS();

  initWebSocket();

  server.begin();
  timeClient.begin();
}

void loop() {
  ws.cleanupClients();
  timeClient.update();
  String currentTime = timeClient.getFormattedTime();
  Alarm();
  notifyClients();
  delay(1000); 
}


