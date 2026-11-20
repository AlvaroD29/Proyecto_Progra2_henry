#ifndef CABALLERO_OSCURO_H
#define CABALLERO_OSCURO_H

#include "Enemigo.h"

class CaballeroOscuro : public Enemigo {
private:
    enum { ARMADURA = 35 };   // porcentaje de reduccion

public:
    CaballeroOscuro(int nivel)
        : Enemigo("Caballero Oscuro", 'k', nivel, 100, 1, 20, 12) {}

    int recibirDanio(int cantidad) override {
        int reducido = cantidad * (100 - ARMADURA) / 100;
        if (cantidad > 0 && reducido < 1) reducido = 1;
        return Enemigo::recibirDanio(reducido);
    }

    Texto getHabilidad() const override {
        return "Armadura: reduce en 35% el daño de todos los ataques.";
    }
};

#endif
