#include <WiFi.h>
#include <WebServer.h>
#include <WebSocket.h>
#include <manejoAlarma.h>
#include <LittleFS.h>


void setup(){
  Serial.begin(115200);
  delay(3000);

  LittleFS.begin();

  File file = LittleFS.open("index.html", "r");

  if (!file) {
    Serial.println("could not open file for reading");
  } else {

    while (file.available()) {
      char buf[32];

      size_t bytesRead = file.readBytes(buf, sizeof(buf) - 1);
      buf[bytesRead] = '\0';

      Serial.print(buf);

      yield();
    }

    file.close();
  }

  LittleFS.end();
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