#ifndef Lampara_h
#define Lampara_h

#include <Arduino.h>


class Lampara{

    public:
        Lampara(int salida);
        void encender();
        void apagar();

    private:
        int _salida;
        int _valorAnalogico;
};

#endif