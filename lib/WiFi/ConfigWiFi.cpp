#include "ConfigWiFi.h"

//  const char* ssid = "blanc";
//  const char* password = "tiempocompartido";
// const char* ssid = "Internet_Services_1418";
// const char* password = "meia242113";

  const char* ssid = "blanc";
  const char* password = "tiempocompartido";

void initWiFi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}