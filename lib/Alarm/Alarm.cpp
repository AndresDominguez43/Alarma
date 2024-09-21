#include "Alarm.h"
#include "WebSPIFFS.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone

String targetTime = "00:00";
String durationStr = "00:00";
const long intervalo = 10;
bool intervaloAlarma = 0;

bool alarmActive = false;
bool alarmDuration = false;
bool alarmStop = false;


unsigned long durationInMillis = 0;
unsigned int alarmRepeatCount = 1;
unsigned int currentRepetition = 0;
unsigned long durationAlarmMillis = 0;

void ConfigPin(){
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LIGHTPIN, LOW);
}


void handleSetTime(AsyncWebServerRequest *request) {
  if (request->hasParam("time", true)) {
    targetTime = request->getParam("time", true)->value();
    saveValueToSPIFFS("/targetTime.txt", targetTime);
    request->send(200, "text/plain", "Time set to: " + targetTime);
    alarmStop = false;  
    
  } else {
    request->send(400, "text/plain", "Time parameter missing");
  }
}

void handleSetAlarmDuration(AsyncWebServerRequest *request) {
    if (request->hasParam("duration")) {
        durationStr = request->getParam("duration")->value();
        Serial.println("Duración seleccionada: " + durationStr + " minutos");
          durationInMillis = (unsigned long)(durationStr.toFloat() * 60000);
          saveValueToSPIFFS("/alarmDuration.txt",durationStr);
      }
    request->send(200, "text/plain", "Duración configurada");
}


void handleStopAlarm(AsyncWebServerRequest *request) {
  alarmActive=false;
  alarmStop = true;
  digitalWrite(LIGHTPIN, LOW);
  Serial.println("Alarma desactivada");
  request->send(200, "text/plain", "Alarma desactivada");
}


void AlarmActive(){
  alarmActive = true;
  ws.textAll("ON");
  digitalWrite(LIGHTPIN, HIGH);
}

void AlarmDesactive(){
  alarmActive = false;
  digitalWrite(LIGHTPIN, LOW);
  ws.textAll("OFF");
  Serial.println("Alarma desactivada");
}

void Alarm(){
  String currentTime = timeClient.getFormattedTime();
  
  int targetHour = targetTime.substring(0, 2).toInt();
  int targetMinute = targetTime.substring(3, 5).toInt();
  int targetSecond = 0;
  
  int currentHour = currentTime.substring(0, 2).toInt();
  int currentMinute = currentTime.substring(3, 5).toInt();
  int currentSecond = currentTime.substring(6, 8).toInt();
  
  if (!alarmStop && currentHour == targetHour && currentMinute == targetMinute && targetSecond == currentSecond){
      if(!alarmActive){
        AlarmActive();
      }
  }

}
  void retardo(long intervalo){
    unsigned long tiempoActual = millis();  
    if (tiempoActual - tiempoAnterior >= intervalo) {
      tiempoAnterior = tiempoActual;  
      intervaloAlarma=1;  
    }
}
