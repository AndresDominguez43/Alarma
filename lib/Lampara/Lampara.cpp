#include <Arduino.h>
#include <Lampara.h>

Lampara::Lampara(int salida) {
    _salida = salida;
    _valorAnalogico = 0;
    _startTime1 = 0;
    _rampaActiva = false;
}

void Lampara::apagar() {
    _valorAnalogico = 0;
    dacWrite(_salida, _valorAnalogico);
}

void Lampara::iniciarRampa(unsigned long duracionRampa) {
    _startTime1 = millis(); 
    _rampaActiva = true;  
    _duracionRampa = duracionRampa;
    _valorAnalogico = 0;  
}
void Lampara::rampa() {
  if (_rampaActiva) {
    unsigned long tiempoTranscurrido = millis() - _startTime1;

    if (tiempoTranscurrido < _duracionRampa) {
      _valorAnalogico = map(tiempoTranscurrido, 0, _duracionRampa,110, 255);
        if(_valorAnalogico>=255){
        _valorAnalogico = 255;
      }
      }
      dacWrite(_salida, _valorAnalogico);
      Serial.print("Valor Analógico: ");
      Serial.println(_valorAnalogico);
      
    }
}

bool Lampara::rampaActiva(){
    return _rampaActiva;
}