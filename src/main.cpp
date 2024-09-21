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
  if (alarmActive) {
    if(!Lampara1.rampaActiva()){
          Lampara1.iniciarRampa(durationInMillis); 
    }
      }else {
          Lampara1.apagar();
      }
  intervaloAlarma = 0;
  if (alarmActive){
      Lampara1.rampa();
  }
  notifyClients();
}


