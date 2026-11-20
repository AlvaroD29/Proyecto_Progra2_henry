#ifndef GOBLIN_H
#define GOBLIN_H

#include "Enemigo.h"

class Goblin : public Enemigo {
private:
    int golpesRecibidos;

public:
    Goblin(int nivel)
        : Enemigo("Goblin", 'g', nivel, 28, 2, 6, 5), golpesRecibidos(0) {}

    int recibirDanio(int cantidad) override {
        golpesRecibidos++;
        if (golpesRecibidos % 4 == 0) {
            return 0;
        }
        return Enemigo::recibirDanio(cantidad);
    }

    Texto getHabilidad() const override {
        return "Escurridizo: avanza 2 casillas por turno y esquiva 1 de cada 4 ataques.";
    }
};

#endif
