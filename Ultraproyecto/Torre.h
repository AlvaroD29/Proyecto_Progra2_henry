#ifndef TORRE_H
#define TORRE_H

#include <iostream>
#include "Consola.h"

class Torre {
private:
    Texto nombre;
    int hp;
    int hpMax;
    int danioRecibido;

public:
    Torre(int hpMax = 250)
        : nombre("Castillo de Eldoria"), hp(hpMax), hpMax(hpMax), danioRecibido(0) {}

    Torre& operator-=(int danio) {
        if (danio < 0) danio = 0;
        int real = danio < hp ? danio : hp;
        hp -= real;
        danioRecibido += real;
        return *this;
    }

    operator bool() const { return hp > 0; }

    bool estaDestruida() const { return hp <= 0; }
    int getHp() const { return hp; }
    int getHpMax() const { return hpMax; }
    int getDanioRecibido() const { return danioRecibido; }

    friend std::ostream& operator<<(std::ostream& os, const Torre& t) {
        Consola consola;
        os << t.nombre << " " << consola.barra(t.hp, t.hpMax, 25)
           << " " << t.hp << "/" << t.hpMax << " HP";
        return os;
    }
};

#endif
