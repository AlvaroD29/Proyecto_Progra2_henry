#ifndef MAPA_H
#define MAPA_H

#include <iostream>
#include <iomanip>
#include <vector>
#include "Texto.h"
#include "Posicion.h"
#include "Defensa.h"
#include "Enemigo.h"

const int MAPA_FILAS = 10;
const int MAPA_COLUMNAS = 24;
const char MAPA_PASTO = '.';
const char MAPA_CAMINO = '#';
const char MAPA_ENTRADA = 'E';
const char MAPA_CASTILLO = 'T';

class Mapa {
private:
    char cuadricula[MAPA_FILAS][MAPA_COLUMNAS];
    std::vector<Posicion> camino;
    std::vector<Defensa*> defensas;

    void trazarTramo(const Posicion& desde, const Posicion& hasta) {
        int f = desde.getFila();
        int c = desde.getColumna();
        int df = (hasta.getFila() > f) - (hasta.getFila() < f);
        int dc = (hasta.getColumna() > c) - (hasta.getColumna() < c);
        if (camino.empty()) camino.push_back(desde);
        while (f != hasta.getFila() || c != hasta.getColumna()) {
            f += df;
            c += dc;
            camino.push_back(Posicion(f, c));
        }
    }

public:
    Mapa() {
        for (int f = 0; f < MAPA_FILAS; f++)
            for (int c = 0; c < MAPA_COLUMNAS; c++)
                cuadricula[f][c] = MAPA_PASTO;

        const int NUM_PUNTOS = 6;
        Posicion puntos[NUM_PUNTOS] = {
            Posicion(1, 0), Posicion(1, 20), Posicion(4, 20),
            Posicion(4, 3), Posicion(7, 3), Posicion(7, 23)
        };
        for (int i = 0; i + 1 < NUM_PUNTOS; i++) {
            trazarTramo(puntos[i], puntos[i + 1]);
        }
        for (const Posicion& p : camino) {
            cuadricula[p.getFila()][p.getColumna()] = MAPA_CAMINO;
        }
        cuadricula[camino.front().getFila()][camino.front().getColumna()] = MAPA_ENTRADA;
        cuadricula[camino.back().getFila()][camino.back().getColumna()] = MAPA_CASTILLO;
    }

    const std::vector<Posicion>& getCamino() const { return camino; }

    bool estaDentro(const Posicion& p) const {
        return p.getFila() >= 0 && p.getFila() < MAPA_FILAS &&
               p.getColumna() >= 0 && p.getColumna() < MAPA_COLUMNAS;
    }

    bool esLibre(const Posicion& p) const {
        if (!estaDentro(p)) return false;
        if (cuadricula[p.getFila()][p.getColumna()] != MAPA_PASTO) return false;
        for (const Defensa* d : defensas) {
            if (d->getPosicion() == p) return false;
        }
        return true;
    }

    void agregarDefensa(Defensa* defensa) {
        defensas.push_back(defensa);
    }

    void dibujar(const std::vector<Enemigo*>& enemigos) const {
        char vista[MAPA_FILAS][MAPA_COLUMNAS];
        for (int f = 0; f < MAPA_FILAS; f++)
            for (int c = 0; c < MAPA_COLUMNAS; c++)
                vista[f][c] = cuadricula[f][c];

        for (const Defensa* d : defensas) {
            vista[d->getPosicion().getFila()][d->getPosicion().getColumna()] = d->getSimbolo();
        }
        for (const Enemigo* e : enemigos) {
            if (e->estaActivo()) {
                vista[e->getPosicion().getFila()][e->getPosicion().getColumna()] = e->getSimbolo();
            }
        }

        std::cout << "      ";
        for (int c = 0; c < MAPA_COLUMNAS; c++) std::cout << (c / 10) << ' ';
        std::cout << "\n      ";
        for (int c = 0; c < MAPA_COLUMNAS; c++) std::cout << (c % 10) << ' ';
        std::cout << "\n    +" << textoRepetir('-', MAPA_COLUMNAS * 2 + 1) << "+\n";
        for (int f = 0; f < MAPA_FILAS; f++) {
            std::cout << "  " << std::setw(2) << f << "| ";
            for (int c = 0; c < MAPA_COLUMNAS; c++) {
                std::cout << vista[f][c] << ' ';
            }
            std::cout << "|\n";
        }
        std::cout << "    +" << textoRepetir('-', MAPA_COLUMNAS * 2 + 1) << "+\n";
        std::cout << "  Mapa:     # camino  E entrada  T castillo  . pasto (aqui van las defensas)\n"
                  << "  Defensas: A arquero  C catapulta  M mago\n"
                  << "  Enemigos: g goblin  o orco  t troll  k caballero oscuro\n";
    }
};

#endif
