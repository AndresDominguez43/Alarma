#include "ConfigWiFi.h"


// const char* ssid = "Internet_Services_1418";
// const char* password = "meia242113";

  const char* ssid = "Personal-E60-2.4GHz";
  const char* password = "8E786ABE60";

void initWiFi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}