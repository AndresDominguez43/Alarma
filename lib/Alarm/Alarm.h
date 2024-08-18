#ifndef Alarm_h
#define Alarm_h
#include <NTPClient.h>  
#include <WiFiUdp.h>

#include <string.h>
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern String targetTime;

// bool LEDSTATE = 0;
const int LIGHTPIN = 2;
void ConfigPin();
void Alarm();

#endif