#include <Arduino.h>
#include <Lampara.h>
#include "Alarm.h"

Lampara::Lampara(int salida){

    _salida = salida;

}

void Lampara::encender(){
     _valorAnalogico = 4095;

     dacWrite(_salida, _valorAnalogico);


}

void Lampara::apagar(){
     _valorAnalogico = 0;

     dacWrite(_salida, _valorAnalogico);

}

void Lampara::rampa(){
     _valorAnalogico++;
     Serial.println(_valorAnalogico);
     dacWrite(_salida, _valorAnalogico);
     if(_valorAnalogico >=250){
          _valorAnalogico = 250;
     }
}