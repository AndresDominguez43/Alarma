#include "ConfigWiFi.h"
#include "dns.h"
#include "Alarm.h"
#include "WebSocket.h"
#include "WebSPIFFS.h"
#include "Lampara.h"

Lampara Lampara1(25);

void setup() {

  Serial.begin(115200);

  ConfigPin();

  initWiFi();

  setupDNS();

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
  delay(10); 
  

  if (alarmActive){
     Lampara1.encender();
  } else {
     Lampara1.apagar();
  }

}


