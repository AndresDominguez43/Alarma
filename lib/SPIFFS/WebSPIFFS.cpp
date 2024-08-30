#include "WebSPIFFS.h"
#include "Alarm.h"


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
  server.on("/set-time", HTTP_POST, handleSetTime);
  server.on("/stopAlarm", HTTP_POST, handleStopAlarm);
 server.on("/alarm.mp3", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!SPIFFS.exists("/ringtone.mp3")){
      return;
        request->send(404, "text/plain", "Archivo no encontrado");
    
    }
      request->send(SPIFFS, "/ringtone.mp3", "audio/mpeg");
  });
server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/favicon.ico", "image/x-icon"); 
});
}
