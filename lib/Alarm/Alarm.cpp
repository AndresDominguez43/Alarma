#include "Alarm.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone
String targetTime = "00:00";

bool alarmActive = false; // Estado de la alarma
unsigned long alarmStartTime = 0;
unsigned long alarmDurationMillis = 0;
unsigned long durationAlarmMillis = 0;

void ConfigPin(){
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LIGHTPIN, LOW);
}

void Alarm(){
  String currentTime = timeClient.getFormattedTime().substring(0,5);
  
  int targetHour = targetTime.substring(0, 2).toInt();
  int targetMinute = targetTime.substring(3, 5).toInt();

  int currentHour = currentTime.substring(0, 2).toInt();
  int currentMinute = currentTime.substring(3, 5).toInt();
  
    if (!alarmActive && targetTime != "" && currentHour == targetHour && currentMinute == targetMinute){
      alarmActive = true;
      alarmStartTime = millis();  // Registrar el tiempo de inicio de la alarma
      durationAlarmMillis= alarmDurationMillis; //se inicializa la variable con 0
      digitalWrite(LIGHTPIN, HIGH); 
      Serial.println("Alarma activada");
  }

  }


void handleStopAlarm(AsyncWebServerRequest *request) {
  alarmActive = false; // Apagar la alarma
  digitalWrite(LIGHTPIN, LOW);
  ws.textAll("STOP_ALARM"); // Enviar señal para detener la alarma a través de WebSocket
  request->send(200, "text/plain", "Alarm stopped");
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