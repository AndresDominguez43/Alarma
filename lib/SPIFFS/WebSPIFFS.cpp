#include "WebSPIFFS.h"
#include "Alarm.h"


void saveValueToSPIFFS(const char* filename, String value) {
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Error al abrir el archivo para escribir: "+ String(filename));
    return;
  }
  file.println(value);  
  file.close();
  Serial.println("Guardado en" + String(filename) + ": " + value);
}

String readValueFromSPIFFS(const char* filename, String resetValue) {
  File file = SPIFFS.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Error al abrir el archivo para leer: " + String(filename ));
    return resetValue;  
  }
  String value = file.readStringUntil('\n'); 
  file.close();
  Serial.println("Leído desde " + String(filename) + ": " + value);
  return value;
}


void WebArchiveSPIFFS(){
if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  alarmTime = readValueFromSPIFFS("/alarmTime.txt", "00:00");
  durationStr = readValueFromSPIFFS("/alarmDuration.txt", "No leido"); 
  
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


  server.on("/stopAlarm", HTTP_POST, handleStopAlarm);
  server.on("/set-time",HTTP_POST,handleSetTime);
  server.on("/setAlarmDuration", HTTP_GET, handleSetAlarmDuration);


  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/favicon.ico", "image/x-icon"); 
  });
}

