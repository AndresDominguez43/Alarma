#include "Alarm.h"
#include "WebSPIFFS.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone

String alarmTime = "00:00";
String durationStr = "00:00";

bool stateAlarm = false;
bool initRamp = false;
unsigned long durationInMillis = 0;

void ConfigPin(){
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LIGHTPIN, LOW);
}


void handleSetTime(AsyncWebServerRequest *request) {
  if (request->hasParam("time", true)) {
    alarmTime = request->getParam("time", true)->value();
    Serial.println("Hora ajustada: " + alarmTime);
    saveValueToSPIFFS("/alarmTime.txt", alarmTime);
    request->send(200, "text/plain", "Time set to: " + alarmTime);
    stateAlarm = true;  
    
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
  stateAlarm = false;
  initRamp = false;
  digitalWrite(LIGHTPIN, LOW);
  Serial.println("Alarma desactivada");
  request->send(200, "text/plain", "Alarma desactivada");
}


void AlarmActivated(){
  stateAlarm = true;
  initRamp = true;
  digitalWrite(LIGHTPIN, HIGH);
}

void AlarmDisabled(){
  stateAlarm = false;
  initRamp = false;
  digitalWrite(LIGHTPIN, LOW);
  Serial.println("Alarma desactivada");
}

void Alarm(){
  String currentTime = timeClient.getFormattedTime();
  
  int alarmHour = alarmTime.substring(0, 2).toInt();
  int alarmMinute = alarmTime.substring(3, 5).toInt();
  int alarmSecond = 0;
  
  int currentHour = currentTime.substring(0, 2).toInt();
  int currentMinute = currentTime.substring(3, 5).toInt();
  int currentSecond = currentTime.substring(6, 8).toInt();
  
  if (stateAlarm && currentHour == alarmHour && currentMinute == alarmMinute && alarmSecond == currentSecond){
    AlarmActivated();
}
}
