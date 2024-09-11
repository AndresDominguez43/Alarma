#include <Arduino.h>
#include <Lampara.h>
#include "Alarm.h"
Lampara::Lampara(int salida){

    _salida = salida;

}

void Lampara::encender(){
     _valorAnalogico++;

     dacWrite(_salida, _valorAnalogico);


}

void Lampara::apagar(){
     _valorAnalogico = 0;

     dacWrite(_salida, _valorAnalogico);

}
