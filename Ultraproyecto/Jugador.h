#ifndef JUGADOR_H
#define JUGADOR_H

#include <iostream>
#include "Texto.h"

class Jugador {
private:
    Texto nombre;
    int oro;
    int oroGastado;
    int oroGanado;

public:
    Jugador(const Texto& nombre = "Anonimo", int oroInicial = 200)
        : nombre(nombre), oro(oroInicial), oroGastado(0), oroGanado(0) {}

    Jugador& operator+=(int cantidad) {
        oro += cantidad;
        oroGanado += cantidad;
        return *this;
    }

    Jugador& operator-=(int cantidad) {
        oro -= cantidad;
        oroGastado += cantidad;
        return *this;
    }

    bool operator>=(int costo) const { return oro >= costo; }

    const Texto& getNombre() const { return nombre; }
    int getOro() const { return oro; }
    int getOroGastado() const { return oroGastado; }
    int getOroGanado() const { return oroGanado; }

    friend std::ostream& operator<<(std::ostream& os, const Jugador& j) {
        os << "Comandante " << j.nombre << " | Oro: " << j.oro;
        return os;
    }
};

#endif
