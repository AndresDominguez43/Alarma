#ifndef Alarm_h
#define Alarm_h
#include <NTPClient.h>  
#include <WiFiUdp.h>
#include "WebSocketServer.h"
#include <string.h>

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern String alarmTime;
extern String durationStr;
extern bool stateAlarm;
extern bool initRamp;
const int LIGHTPIN = 2;
void ConfigPin();
void Alarm();
void handleSetTime(AsyncWebServerRequest *request);
void handleStopAlarm(AsyncWebServerRequest *request);
void handleSetAlarmDuration(AsyncWebServerRequest *request);
void retardo(long intervalo);
extern unsigned long durationInMillis;
extern unsigned long alarmIntervalMillis;

#endif