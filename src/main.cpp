#include "PortalWiFi.h"
#include "Alarm.h"
#include "WebSocketServer.h"
#include "WebSPIFFS.h"
#include "Lampara.h"

Lampara Lampara1(25);

void setup() {
  Serial.begin(115200);
  setupWiFiPortal();
  initWebSocket();
  WebArchiveSPIFFS();
  ConfigPin();
  server.begin();
  timeClient.begin();
}

void loop() {
  ws.cleanupClients();
  timeClient.update();
  
  Alarm();
  if (initRamp) {
    if(!Lampara1.rampaActiva()){
          Lampara1.iniciarRampa(durationInMillis); 
    }
    Lampara1.rampa();
  }
  else {
      Lampara1.apagar();
  }
  notifyClients();
}


