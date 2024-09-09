#include "WebSPIFFS.h"
#include "Alarm.h"


void saveTargetTime(String time) {
  File file = SPIFFS.open("/targetTime.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Error al abrir el archivo para escribir targetTime");
    return;
  }
  file.println(time);  // Guardar el valor
  file.close();
  Serial.println("targetTime guardado: " + time);
}

String readTargetTime() {
  File file = SPIFFS.open("/targetTime.txt", FILE_READ);
  if (!file) {
    Serial.println("Error al abrir el archivo para leer targetTime");
    return "00:00";  // Retornar valor predeterminado si no existe el archivo
  }
  String time = file.readStringUntil('\n');  // Leer el valor
  file.close();
  Serial.println("targetTime leído: " + time);
  return time;
}


void WebArchiveSPIFFS(){
if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  targetTime = readTargetTime();

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
  server.on("/set-time", HTTP_POST, [](AsyncWebServerRequest *request){
    handleSetTime(request);
    saveTargetTime(targetTime);
    request->send(200,"text/plain", "Alarma guardada en SPIFFS");
  });
  server.on("/stopAlarm", HTTP_POST, handleStopAlarm);
  
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/favicon.ico", "image/x-icon"); 
  });


// server.on("/alarm.mp3", HTTP_GET, [](AsyncWebServerRequest *request){
//     if(!SPIFFS.exists("/ringtone.mp3")){
//       return;
//         request->send(404, "text/plain", "Archivo no encontrado");
    
//     }
//       request->send(SPIFFS, "/ringtone.mp3", "audio/mpeg");
//   });
}
