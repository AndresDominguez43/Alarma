#include "Alarm.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone
String targetTime = "00:00";
bool alarmActive = false; // Estado de la alarma


void ConfigPin(){
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LIGHTPIN, LOW);
}

void Alarm(){
  String currentTime = timeClient.getFormattedTime();
  if (targetTime != "" && currentTime == targetTime) {
    digitalWrite(LIGHTPIN, HIGH);
    alarmActive = true; // Alarma activa cuando llega la hora
    ws.textAll("ALARM"); // Enviar señal de alarma a través de WebSocket
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