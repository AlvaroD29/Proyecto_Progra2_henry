#ifndef POSICION_H
#define POSICION_H

#include <iostream>
#include "Texto.h"

class Posicion {
private:
    int fila;
    int columna;

    // Raiz cuadrada por el metodo de Newton (sin <cmath>).
    // Se parte de un valor >= a la raiz y se baja hasta que deje de disminuir.
    double raizCuadrada(double valor) const {
        if (valor <= 0.0) return 0.0;
        double x = valor >= 1.0 ? valor : 1.0;
        while (true) {
            double siguiente = 0.5 * (x + valor / x);
            if (siguiente >= x) return x;
            x = siguiente;
        }
    }

public:
    Posicion(int fila = 0, int columna = 0) : fila(fila), columna(columna) {}

    int getFila() const { return fila; }
    int getColumna() const { return columna; }

    double operator-(const Posicion& otra) const {
        double df = fila - otra.fila;
        double dc = columna - otra.columna;
        return raizCuadrada(df * df + dc * dc);
    }

    bool operator==(const Posicion& otra) const {
        return fila == otra.fila && columna == otra.columna;
    }

    bool operator!=(const Posicion& otra) const {
        return !(*this == otra);
    }

    Texto aTexto() const {
        return "(" + textoDeEntero(fila) + "," + textoDeEntero(columna) + ")";
    }

    friend std::ostream& operator<<(std::ostream& os, const Posicion& p) {
        os << "(" << p.fila << "," << p.columna << ")";
        return os;
    }
};

#endif
