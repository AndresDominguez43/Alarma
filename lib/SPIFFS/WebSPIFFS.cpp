#include "WebSPIFFS.h"
#include "Alarm.h"


void saveValueToSPIFFS(const char* filename, String value) {
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Error al abrir el archivo para escribir: "+ String(filename));
    return;
  }
  file.println(value);  // Guardar el valor
  file.close();
  Serial.println("Guardado en" + String(filename) + ": " + value);
}

String readValue(const char* filename, String resetValue) {
  File file = SPIFFS.open(filename, FILE_READ);
  if (!file) {
    Serial.println("Error al abrir el archivo para leer: " + String(filename ));
    return resetValue;  // Retornar valor predeterminado si no existe el archivo
  }
  String value = file.readStringUntil('\n');  // Leer el valor
  file.close();
  Serial.println("Leído desde " + String(filename) + ": " + value);
  return value;
}


void WebArchiveSPIFFS(){
if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  targetTime = readValue("/targetTime.txt", "00:00");
  durationStr = readValue("/alarmDuration.txt", "0:5"); 
  repeatStr = readValue("/alarmRepeat.txt", "1");
  intervalStr = readValue("/alarmInterval.txt", "1"); 
  durationLampStr = readValue("/alarmInDurationLamp.txt", "00:00"); 
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
  server.on("/setDurationLamp", HTTP_GET, handleSetDurationLamp);
  server.on("/setAlarmDuration", HTTP_GET, handleSetAlarmDuration);
  server.on("/setAlarmRepeat", HTTP_GET, handleSetAlarmRepetitions);
  server.on("/setAlarmInterval", HTTP_GET, handleSetAlarmInterval);
  server.on("/setAlarm", HTTP_GET, stateAlarm);

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/favicon.ico", "image/x-icon"); 
  });


server.on("/ringtone.mp3", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/ringtone.mp3", "audio/mpeg");
  });
}
