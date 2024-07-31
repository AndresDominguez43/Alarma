#ifndef WebServer_h
#define WebServer_h

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ESPmDNS.h>

extern AsyncWebServer server;

void initWebServer();
void configWiFi();
void configMDNS();

#endif
