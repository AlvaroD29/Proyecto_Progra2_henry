#ifndef DEFENSA_H
#define DEFENSA_H

#include <iostream>
#include <iomanip>
#include <vector>
#include "Texto.h"
#include "Posicion.h"
#include "Enemigo.h"

const int DEFENSA_NIVEL_MAXIMO = 5;

class Defensa {
protected:
    Texto nombre;
    char simbolo;
    Posicion pos;
    int nivel;
    int danio;
    int danioBase;
    double alcance;
    int costo;
    int danioTotal;
    int bajas;
    bool enRango(const Enemigo* e) const {
        return e->estaActivo() && (pos - e->getPosicion()) <= alcance;
    }

    Enemigo* buscarObjetivo(const std::vector<Enemigo*>& enemigos) const {
        Enemigo* mejor = nullptr;
        for (Enemigo* e : enemigos) {
            if (enRango(e) && (mejor == nullptr ||
                               e->getIndiceCamino() > mejor->getIndiceCamino())) {
                mejor = e;
            }
        }
        return mejor;
    }

    int golpear(Enemigo* e, int cantidad, std::vector<Texto>& eventos) {
        int antes = e->getHp();
        *e - cantidad;
        int real = antes - e->getHp();
        danioTotal += real;

        Texto msg = nombre + " " + pos.aTexto() + " -> " + e->getNombre();
        if (real == 0) {
            msg += ": ¡esquivo el ataque!";
        } else {
            msg += ": -" + textoDeEntero(real) + " HP";
        }
        if (!e->estaVivo()) {
            msg += "  [ELIMINADO +" + textoDeEntero(e->getRecompensa()) + " oro]";
            bajas++;
        }
        eventos.push_back(msg);
        return real;
    }

public:
    Defensa(const Texto& nombre, char simbolo, const Posicion& pos,
            int danio, double alcance, int costo)
        : nombre(nombre), simbolo(simbolo), pos(pos), nivel(1),
          danio(danio), danioBase(danio), alcance(alcance), costo(costo),
          danioTotal(0), bajas(0) {}

    virtual ~Defensa() = default;

    virtual int atacar(std::vector<Enemigo*>& enemigos, std::vector<Texto>& eventos) = 0;

    virtual Texto getDescripcion() const = 0;

    Defensa& operator+=(int niveles) {
        for (int i = 0; i < niveles && nivel < DEFENSA_NIVEL_MAXIMO; i++) {
            nivel++;
            danio += danioBase * 2 / 5;   // +40% del daño base
            alcance += 0.5;
        }
        return *this;
    }

    Defensa& operator++() {
        return *this += 1;
    }

    bool puedeMejorar() const { return nivel < DEFENSA_NIVEL_MAXIMO; }
    int getCostoMejora() const { return costo * nivel * 3 / 5; }

    const Texto& getNombre() const { return nombre; }
    char getSimbolo() const { return simbolo; }
    const Posicion& getPosicion() const { return pos; }
    int getNivel() const { return nivel; }
    int getDanio() const { return danio; }
    double getAlcance() const { return alcance; }
    int getCosto() const { return costo; }
    int getBajas() const { return bajas; }

    virtual void mostrar(std::ostream& os) const {
        os << std::left << std::setw(10) << nombre << std::right
           << " Nv" << nivel << " en " << pos
           << "  Daño " << std::setw(3) << danio
           << "  Alcance " << std::fixed << std::setprecision(1) << alcance
           << "  Bajas " << std::setw(3) << bajas;
        if (puedeMejorar()) {
            os << "  | Mejora: " << getCostoMejora() << " oro";
        } else {
            os << "  | NIVEL MAXIMO";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Defensa& d) {
        d.mostrar(os); 
        return os;
    }
};

#endif
