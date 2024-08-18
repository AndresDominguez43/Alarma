#include "dns.h"


void DNS(){
    if (!MDNS.begin("Alarma")) {  // http://alarma.local
        Serial.println("Error setting up MDNS responder!");
    } else {
        Serial.println("mDNS responder started");
        Serial.println("URL: http://Alarma.local ");
    }
}