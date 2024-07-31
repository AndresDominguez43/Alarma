#include "WebServer.h"
#include "manejoAlarma.h"
#include <ESPmDNS.h>

const char* ssid = "Internet_Services_1418";
const char* password = "meia242113";
extern String targetTime;

void configWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void configMDNS() {
  if (!MDNS.begin("Alarma")) {  // http://alarma.local
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
  }
}
void handleRoot(AsyncWebServerRequest *request){
  request->send(LittleFS,"/index.html","text/html");
}

void handleNotFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "Not found");
}

void initWebServer() {
  if(!LittleFS.begin()){
    Serial.println("Ocurrio un error mientras se montaba LittleFS");
    return;
  }
  server.on("/", HTTP_GET, handleRoot);
   server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/script.js", "application/javascript");
  });

  server.on("/set-time", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("time", true)) {
      targetTime = request->getParam("time", true)->value();
      Serial.println("Time set to: " + targetTime); // Mostrar el valor de targetTime en el monitor serial
      request->send(200, "text/plain", "Time set to: " + targetTime);
    } else {
      request->send(400, "text/plain", "Time parameter missing");
    }
  });
  server.onNotFound(handleNotFound);
  server.begin();
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