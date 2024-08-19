#include "dns.h"

const char* DOMAIN_NAME = "ServidorESP";

void setupDNS() {
    if(!MDNS.begin(DOMAIN_NAME)){
        Serial.println("Error MDNS");
    }else {
        Serial.println("Start DNS: http://" + String(DOMAIN_NAME)+ ".local");
    }
}