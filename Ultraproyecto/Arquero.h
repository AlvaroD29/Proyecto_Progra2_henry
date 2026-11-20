#ifndef ARQUERO_H
#define ARQUERO_H

#include "Defensa.h"

const int ARQUERO_COSTO = 50;

class Arquero : public Defensa {
public:
    Arquero(const Posicion& pos)
        : Defensa("Arquero", 'A', pos, 10, 3.0, ARQUERO_COSTO) {}

    int atacar(std::vector<Enemigo*>& enemigos, std::vector<Texto>& eventos) override {
        int flechas = (nivel >= 4) ? 2 : 1;
        int total = 0;
        for (int i = 0; i < flechas; i++) {
            Enemigo* objetivo = buscarObjetivo(enemigos);
            if (objetivo == nullptr) break;
            total += golpear(objetivo, danio, eventos);
        }
        return total;
    }

    Texto getDescripcion() const override {
        return "Un objetivo, largo alcance. Doble flecha desde nivel 4.";
    }
};

#endif
