#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <iostream>
#include <iomanip>
#include "Texto.h"
#include <vector>
#include "Posicion.h"
#include "Torre.h"

class Enemigo {
protected:
    Texto nombre;
    char simbolo;
    int nivel;
    int hp;
    int hpMax;
    int velocidad;
    int danio;
    int recompensa;
    int indiceCamino;
    Posicion pos;
    int turnosLento;
    bool llego;

    virtual int calcularPasos(int turno) const {
        (void)turno;
        return velocidad;
    }

public:
    Enemigo(const Texto& nombre, char simbolo, int nivel,
            int hpBase, int velocidad, int danioBase, int recompensaBase)
        : nombre(nombre), simbolo(simbolo), nivel(nivel),
          velocidad(velocidad), indiceCamino(0), turnosLento(0), llego(false) {
        hpMax = static_cast<int>(hpBase * (1.0 + 0.45 * (nivel - 1)));
        hp = hpMax;
        danio = static_cast<int>(danioBase * (1.0 + 0.15 * (nivel - 1)));
        recompensa = recompensaBase + nivel;
    }

    virtual ~Enemigo() = default;

    virtual void alIniciarTurno() {}

    virtual int recibirDanio(int cantidad) {
        if (cantidad < 0) cantidad = 0;
        int real = cantidad < hp ? cantidad : hp;
        hp -= real;
        return real;
    }

    virtual Texto getHabilidad() const = 0;


    void colocarEnInicio(const std::vector<Posicion>& camino) {
        indiceCamino = 0;
        pos = camino[0];
    }

    void avanzar(const std::vector<Posicion>& camino, int turno) {
        if (!estaActivo()) return;
        int pasos = calcularPasos(turno);
        if (turnosLento > 0) {
            pasos = pasos > 0 ? pasos - 1 : 0;
            turnosLento--;
        }
        indiceCamino += pasos;
        int ultimo = static_cast<int>(camino.size()) - 1;
        if (indiceCamino >= ultimo) {
            indiceCamino = ultimo;
            llego = true;
        }
        pos = camino[indiceCamino];
    }

    void atacar(Torre& torre) const {
        torre -= danio;
    }

    void ralentizar(int turnos) {
        if (turnos > turnosLento) turnosLento = turnos;
    }

    Enemigo& operator-(int cantidad) {
        recibirDanio(cantidad);
        return *this;
    }

    bool estaVivo() const { return hp > 0; }
    bool llegoAlCastillo() const { return llego; }
    bool estaActivo() const { return hp > 0 && !llego; }

    const Texto& getNombre() const { return nombre; }
    char getSimbolo() const { return simbolo; }
    int getNivel() const { return nivel; }
    int getHp() const { return hp; }
    int getHpMax() const { return hpMax; }
    int getVelocidad() const { return velocidad; }
    int getDanio() const { return danio; }
    int getRecompensa() const { return recompensa; }
    int getIndiceCamino() const { return indiceCamino; }
    const Posicion& getPosicion() const { return pos; }

    virtual void mostrar(std::ostream& os) const {
        os << std::left << std::setw(17) << nombre << std::right
           << " Nv" << std::setw(2) << nivel
           << "  HP " << std::setw(4) << hp << "/" << std::setw(4) << hpMax
           << "  Vel " << velocidad
           << "  Daño " << std::setw(3) << danio
           << "  en " << pos;
        if (turnosLento > 0) os << " [lento]";
    }

    friend std::ostream& operator<<(std::ostream& os, const Enemigo& e) {
        e.mostrar(os);   // llamada polimorfica
        return os;
    }
};

#endif
