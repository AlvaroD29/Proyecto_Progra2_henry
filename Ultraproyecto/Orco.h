#ifndef ORCO_H
#define ORCO_H

#include "Enemigo.h"

class Orco : public Enemigo {
protected:
    int calcularPasos(int turno) const override {
        (void)turno;
        return (hp * 2 < hpMax) ? velocidad + 1 : velocidad;
    }

public:
    Orco(int nivel)
        : Enemigo("Orco", 'o', nivel, 65, 1, 12, 8) {}

    Texto getHabilidad() const override {
        return "Furia: con menos del 50% de vida avanza 2 casillas por turno.";
    }
};

#endif
