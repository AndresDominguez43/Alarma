#include "PortalWiFi.h"
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>

DNSServer dnsServer;
WiFiManager wm;

void startDNSServer(){
  dnsServer.start(53, "*", WiFi.softAPIP());
}
void startMDNS(const char* hostname){
  if(!MDNS.begin(hostname)){
    Serial.println("Error al configurar MDNS");
    return;
  }
  Serial.println("mDNS: ");
  Serial.println(hostname);
}

void initWiFiPortal(){
  
  // wm.resetSettings();
  bool res = wm.autoConnect("PortalCautivo", "proyecto123");
  if(!res){
    Serial.println("No se pudo conectar a WiFi");
    ESP.restart();
  } else{
    Serial.print("Conectado a WiFi");
    startMDNS("ServidorESP32");
  }
}
