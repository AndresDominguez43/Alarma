#include "Alarm.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone
String currentTime = timeClient.getFormattedTime();
String targetTime = "00:00";
void ConfigPin(){
    pinMode(LIGHTPIN, OUTPUT);
    digitalWrite(LIGHTPIN, LOW);
}

void Alarm(){
  if (targetTime != "" && currentTime == targetTime) {
    digitalWrite(LIGHTPIN, HIGH);
  } else {
    digitalWrite(LIGHTPIN, LOW);
  }
}
