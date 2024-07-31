#include "manejoAlarma.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10800); // UTC-3 timezone

extern const int lightPin = 2;
String targetTime = "00:00";

void startTimeClient() {
    timeClient.begin();
}

void updateTime() {
    timeClient.update();
}

void checkAlarms() {
    String currentTime = timeClient.getFormattedTime();
    if (targetTime != "" && currentTime == targetTime) {
    digitalWrite(lightPin, HIGH);
    } else {
    digitalWrite(lightPin, LOW);
    }
}
