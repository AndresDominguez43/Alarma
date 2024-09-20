#include "Alarm.h"
#include "WebSPIFFS.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone

String targetTime = "00:00";
//String durationStr = "00:00";
unsigned long tiempoAnterior = 0;
const long intervalo = 10;
bool intervaloAlarma = 0;

bool alarmActive = false;
//bool alarmDuration = false;
bool alarmStop = false;
bool alarmON = false;

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


void handleSetTime(AsyncWebServerRequest *request) {
  if (request->hasParam("time", true)) {
    targetTime = request->getParam("time", true)->value();
    // Serial.println("Time set to: " + targetTime); // Mostrar el valor de targetTime en el monitor serial
    saveValueToSPIFFS("/targetTime.txt", targetTime);
    request->send(200, "text/plain", "Time set to: " + targetTime);
    alarmStop = false;  
    
  } else {
    request->send(400, "text/plain", "Time parameter missing");
  }
}


// void handleSetAlarmDuration(AsyncWebServerRequest *request) {
//     if (request->hasParam("duration")) {
//         durationStr = request->getParam("duration")->value();
//           alarmDurationMillis = (unsigned long)(durationStr.toFloat() * 60000);
//           saveValueToSPIFFS("/alarmDuration.txt",durationStr);
//         Serial.println("Duración seleccionada: " + durationStr + " minutos");
//       }
//     request->send(200, "text/plain", "Duración configurada");
// }


void handleStopAlarm(AsyncWebServerRequest *request) {
  alarmActive=false;
  alarmStop = true;
  alarmON = false;
  currentRepetition = 0;
  digitalWrite(LIGHTPIN, LOW);
  Serial.println("Alarma desactivada");
  //ws.textAll("STOP_ALARM"); // Enviar señal para detener la alarma a través de WebSocket
  request->send(200, "text/plain", "Alarma desactivada");
}

// void stateAlarm(AsyncWebServerRequest *request) {   //esta funcion es para que funcione correctamente las peticiones http del audio
//   if (request->hasParam("state")) {
//     String state = request->getParam("state")->value();
    
//     // Guardar el estado de la alarma en SPIFFS
//     if (state == "1") {
//       saveValueToSPIFFS("/alarmState.txt", "1");
//       request->send(200, "text/plain", "Alarma activa");
//     } else if (state == "0") {
//       saveValueToSPIFFS("/alarmState.txt", "0");
//       request->send(200, "text/plain", "Alarma desactiva");
//     } else {
//       request->send(400, "text/plain", "Valor de estado invalido");
//     }
//   } else {
//     request->send(400, "text/plain", "State parameter missing");
//   }
// }

void AlarmActive(){
  alarmStartTime = millis(); //Guardamos el tiempo de inicio de la alarma
  durationAlarmMillis = alarmDurationMillis;
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
      // if(alarmActive){
      //   unsigned long elapsedTime = millis() - alarmStartTime; //Calcula el tiempo que paso desde que se activo la alarma.
        
      //   if(alarmDurationMillis>elapsedTime){
      //     durationAlarmMillis = alarmDurationMillis - elapsedTime; //Decrementa el tiempo restante
      //   } else{
      //     durationAlarmMillis = 0;
      //     AlarmDesactive();

          
      //   }
        // Serial.print("Falta: ");
        // Serial.println(durationAlarmMillis/1000); //Muestar cuanto queda prendida la alarma en segundos
      }

  
  void retardo(long intervalo) {
    unsigned long tiempoActual = millis();   // Obtenemos el tiempo actual
    // Si ha pasado más de 'intervalo' milisegundos
    if (tiempoActual - tiempoAnterior >= intervalo) {
      tiempoAnterior = tiempoActual;         // Actualizamos el tiempo anterior
      intervaloAlarma=1;  // Cambiamos el estado del LED
    }
  }

