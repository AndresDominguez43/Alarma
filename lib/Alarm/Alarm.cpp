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
unsigned long durationAlarmMillis = 0;

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

//  Verificar si la alarma está activa y ha pasado el tiempo de duración
  if (alarmActive){
    unsigned long  currentMillis = millis(); 
    unsigned long  elapsedMillis = currentMillis - alarmStartTime; //Calcula el tiempo transcurrido desde que arranco la alarma

        durationAlarmMillis = alarmDurationMillis - elapsedMillis;

      // Verificar si el tiempo restante ha llegado a cero
      if (durationAlarmMillis <= 0) {
          digitalWrite(LIGHTPIN, LOW);  // Apagar el LED
          alarmActive = false;  // Marcar la alarma como inactiva
          Serial.println("Alarma desactivada");

          // Verificar si quedan repeticiones
          if (currentRepetition < alarmRepeatCount) {
              currentRepetition++;
              targetTime = "";  // Forzar a esperar al siguiente ciclo para reiniciar
              alarmStartTime = millis();  // Reiniciar el tiempo de inicio para la próxima repetición
          } else {
              currentRepetition = 0;  // Resetear el conteo de repeticiones
          }
      } else {
          // Mostrar la duración restante de la alarma
          Serial.print("Tiempo restante de la alarma: ");
          Serial.println(durationAlarmMillis);
      }
  }
}




