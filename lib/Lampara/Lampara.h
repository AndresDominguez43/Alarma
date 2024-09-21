#ifndef Lampara_h
#define Lampara_h

#include <Arduino.h>


class Lampara{

    public:
        Lampara(int salida);
        void encender();
        void apagar();
        void rampa();
        void iniciarRampa(unsigned long duracion);
        bool rampaActiva();
    private:
        int _salida;
        uint16_t _valorAnalogico;
        bool _rampaActiva;
        unsigned long _startTime1;
        unsigned long _duracionRampa;

};

#endif