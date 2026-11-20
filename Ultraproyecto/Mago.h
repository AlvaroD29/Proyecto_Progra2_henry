#ifndef MAGO_H
#define MAGO_H

#include "Defensa.h"

const int MAGO_TURNOS_LENTITUD = 2;
const int MAGO_COSTO = 110;

class Mago : public Defensa {
public:
    Mago(const Posicion& pos)
        : Defensa("Mago", 'M', pos, 7, 2.5, MAGO_COSTO) {}

    int atacar(std::vector<Enemigo*>& enemigos, std::vector<Texto>& eventos) override {
        int total = 0;
        for (Enemigo* e : enemigos) {
            if (enRango(e)) {
                total += golpear(e, danio, eventos);
                e->ralentizar(MAGO_TURNOS_LENTITUD);
            }
        }
        return total;
    }

    Texto getDescripcion() const override {
        return "Daña a todos los enemigos en alcance y los ralentiza.";
    }
};

#endif
