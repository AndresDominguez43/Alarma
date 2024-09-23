#include "PortalWiFi.h"
#include <WiFiManager.h>
#include <ESPmDNS.h>

DNSServer dnsServer;
WiFiManager wm;

const char* APSSID = "PortalCautivo";
const char* APPASSWORD = "proyecto123";

void configureMDNS(const char* hostname){
  if(!MDNS.begin(hostname)){
    Serial.println("Error al configurar MDNS");
    return;
  }
  Serial.println("mDNS: ");
  Serial.println("http://" + String(hostname) + ".local");
}


void setupWiFiPortal(){
  //wm.resetSettings();
  bool wifiConnected = wm.autoConnect(APSSID, APPASSWORD);
  if(!wifiConnected){
    Serial.println("No se pudo conectar a WiFi");
    ESP.restart();
  } else{
    Serial.print("Conectado a WiFi");
    configureMDNS("Alarma");
  }
}
