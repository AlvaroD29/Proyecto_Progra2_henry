#ifndef CATAPULTA_H
#define CATAPULTA_H

#include "Defensa.h"

class Catapulta : public Defensa {
private:
    enum { TURNOS_RECARGA = 1 };
    int recarga;

public:
    enum { COSTO = 90 };

    Catapulta(const Posicion& pos)
        : Defensa("Catapulta", 'C', pos, 22, 2.5, COSTO), recarga(0) {}

    int atacar(std::vector<Enemigo*>& enemigos, std::vector<Texto>& eventos) override {
        if (recarga > 0) {
            recarga--;
            return 0;
        }
        Enemigo* objetivo = buscarObjetivo(enemigos);
        if (objetivo == nullptr) return 0;

        const double RADIO_EXPLOSION = 1.5;
        Posicion impacto = objetivo->getPosicion();
        int total = golpear(objetivo, danio, eventos);
        for (Enemigo* e : enemigos) {
            if (e != objetivo && e->estaActivo() &&
                (impacto - e->getPosicion()) <= RADIO_EXPLOSION) {
                total += golpear(e, danio / 2, eventos);
                }
        }
        recarga = TURNOS_RECARGA;
        return total;
    }

    Texto getDescripcion() const override {
        return "Daño en area (50% a enemigos cercanos). Dispara cada 2 turnos.";
    }
};

#endif
