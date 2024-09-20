#include "PortalWiFi.h"
#include "Alarm.h"
#include "WebSocket.h"
#include "WebSPIFFS.h"
#include "Lampara.h"

Lampara Lampara1(25);

void setup() {
  Serial.begin(115200);
  initWiFiPortal();
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

  retardo(30);
  if(intervaloAlarma){
  if (alarmActive){
    Lampara1.rampa();
    intervaloAlarma = 0;
  } else {
    Lampara1.apagar();
    intervaloAlarma = 0;
  }
  }

  notifyClients();
}


