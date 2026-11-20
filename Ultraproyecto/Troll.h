#ifndef TROLL_H
#define TROLL_H

#include "Enemigo.h"

class Troll : public Enemigo {
protected:
    int calcularPasos(int turno) const override {
        return (turno % 3 == 0) ? 0 : velocidad;
    }

public:
    Troll(int nivel)
        : Enemigo("Troll", 't', nivel, 160, 1, 30, 15) {}

    void alIniciarTurno() override {
        if (estaVivo() && hp < hpMax) {
            int cura = hpMax / 50;          // 2% de su vida maxima
            if (cura < 1) cura = 1;
            hp = (hp + cura > hpMax) ? hpMax : hp + cura;
        }
    }

    Texto getHabilidad() const override {
        return "Regeneracion: recupera 2% de vida por turno; descansa 1 de cada 3 turnos.";
    }
};

#endif
