#include "Alarm.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone
String targetTime = "00:00";
bool alarmActive = false;
bool alarmDuration = false;
bool alarmRepeat = false; 

unsigned long alarmStartTime = 0;
unsigned long alarmDurationMillis = 0;
unsigned long alarmIntervalMillis = 0;
unsigned int alarmRepeatCount = 1;
unsigned int currentRepetition = 0;
void ConfigPin(){
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LIGHTPIN, LOW);
}


void handleStopAlarm(AsyncWebServerRequest *request) {
  alarmActive = false; 
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

void handleSetAlarmRepetitions(AsyncWebServerRequest *request) {
    if (request->hasParam("repeat")) {
        String repeatStr = request->getParam("repeat")->value();
        alarmRepeatCount = repeatStr.toInt();
        Serial.println("Repeticiones: " + String(alarmRepeatCount) + " veces");
      }
    request->send(200, "text/plain", "Repeticiones configuradas");
}

void handleSetAlarmDuration(AsyncWebServerRequest *request) {
    if (request->hasParam("duration")) {
        String durationStr = request->getParam("duration")->value();
          alarmDurationMillis = (unsigned long)(durationStr.toFloat() * 60000);
        Serial.println("Duración seleccionada: " + durationStr + " minutos");
      }
    request->send(200, "text/plain", "Duración configurada");
}

void handleSetAlarmInterval(AsyncWebServerRequest *request) {
    if (request->hasParam("interval")) {
        String intervalStr = request->getParam("interval")->value();
        alarmIntervalMillis = (unsigned long)(intervalStr.toFloat() * 60000);
        Serial.println("Intervalo: " + intervalStr + " minutos");
      }
    request->send(200, "text/plain", "Intervalo configurado");
}

void Alarm(){
  String currentTime = timeClient.getFormattedTime().substring(0,5);
  
    if (!alarmActive && targetTime != "" && currentTime == targetTime) {
      alarmActive = true;
      alarmStartTime = millis();  // Registrar el tiempo de inicio de la alarma
      digitalWrite(LIGHTPIN, HIGH);  // Encender el LED
      Serial.println("Alarma activada");
  }

  // Verificar si la alarma está activa y ha pasado el tiempo de duración
  if (alarmActive && (millis() - alarmStartTime >= alarmDurationMillis)) {
      digitalWrite(LIGHTPIN, LOW);  // Apagar el LED
      alarmActive = false;  // Marcar la alarma como inactiva
      Serial.println("Alarma desactivada");
        if (currentRepetition < alarmRepeatCount) {
          // Si quedan repeticiones, ajustar el tiempo de la próxima alarma
          targetTime = ""; // Forzar a esperar al siguiente minuto para reiniciar
      } else {
          currentRepetition = 0;  // Resetear repeticiones
      }
  }
  }




