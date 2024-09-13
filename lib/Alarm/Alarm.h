#ifndef Alarm_h
#define Alarm_h
#include <NTPClient.h>  
#include <WiFiUdp.h>
#include "WebSocket.h"
#include <string.h>

extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern String targetTime;
extern String durationStr;
extern String repeatStr;
extern String intervalStr;

extern bool alarmActive;
const int LIGHTPIN = 2;
void ConfigPin();
void Alarm();
void handleSetTime(AsyncWebServerRequest *request);
void handleStopAlarm(AsyncWebServerRequest *request);
void handleSetAlarmDuration(AsyncWebServerRequest *request);
void handleSetAlarmRepetitions(AsyncWebServerRequest *request);
void handleSetAlarmInterval(AsyncWebServerRequest *request);
extern unsigned long alarmDurationMillis;
extern unsigned long alarmIntervalMillis;
extern unsigned int alarmRepeatCount;
#endif