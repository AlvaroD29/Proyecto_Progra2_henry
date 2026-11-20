#ifndef REGISTRO_PARTIDA_H
#define REGISTRO_PARTIDA_H

#include <iostream>
#include <iomanip>
#include "Texto.h"

const int REGISTRO_CAMPOS = 8;

// Una linea del historial tiene el formato:
//   jugador;resultado;oleadas;bajas;danioRecibido;oroGastado;defensas;puntaje
// (el formato anterior, con la fecha como segundo campo, tambien se sigue leyendo)
class RegistroPartida {
private:
    Texto jugador;
    Texto resultado;
    int oleadasSuperadas;
    int enemigosEliminados;
    int danioRecibido;
    int oroGastado;
    int defensasColocadas;
    int puntaje;

    Texto limpiarTexto(Texto texto) const {
        for (int i = 0; i < texto.size(); i++) {
            char& ch = texto[i];
            if (ch == ';' || ch == '\n' || ch == '\r') ch = ' ';
        }
        return texto;
    }

    bool desdeLinea(const Texto& linea) {
        Texto campos[REGISTRO_CAMPOS + 1];
        int cantidad = linea.dividir(';', campos, REGISTRO_CAMPOS + 1);
        if (cantidad != REGISTRO_CAMPOS && cantidad != REGISTRO_CAMPOS + 1) return false;

        int saltar = cantidad - REGISTRO_CAMPOS;   // 1 si la linea trae la fecha (formato anterior)
        int numeros[6];
        for (int i = 0; i < 6; i++) {
            if (!campos[2 + saltar + i].aEntero(numeros[i])) return false;
        }
        jugador = campos[0];
        resultado = campos[1 + saltar];
        oleadasSuperadas = numeros[0];
        enemigosEliminados = numeros[1];
        danioRecibido = numeros[2];
        oroGastado = numeros[3];
        defensasColocadas = numeros[4];
        puntaje = numeros[5];
        return true;
    }

public:
    RegistroPartida()
        : oleadasSuperadas(0), enemigosEliminados(0), danioRecibido(0),
          oroGastado(0), defensasColocadas(0), puntaje(0) {}

    RegistroPartida(const Texto& jugador, bool victoria, int oleadas, int enemigos,
                    int danio, int oro, int defensas, int vidaRestante)
        : jugador(limpiarTexto(jugador)),
          resultado(victoria ? "VICTORIA" : "DERROTA"),
          oleadasSuperadas(oleadas), enemigosEliminados(enemigos), danioRecibido(danio),
          oroGastado(oro), defensasColocadas(defensas) {
        puntaje = enemigos * 10 + oleadas * 100 + vidaRestante + (victoria ? 1000 : 0);
    }

    // Escribe el registro en una sola linea (se usa con un ofstream).
    void escribir(std::ostream& os) const {
        os << jugador << ';' << resultado << ';' << oleadasSuperadas << ';'
           << enemigosEliminados << ';' << danioRecibido << ';' << oroGastado << ';'
           << defensasColocadas << ';' << puntaje << '\n';
    }

    bool esVictoria() const { return resultado == "VICTORIA"; }
    int getPuntaje() const { return puntaje; }
    int getEnemigosEliminados() const { return enemigosEliminados; }
    const Texto& getJugador() const { return jugador; }

    bool operator<(const RegistroPartida& otro) const {
        return puntaje < otro.puntaje;
    }

    void imprimirEncabezado(std::ostream& os) const {
        os << std::left << std::setw(14) << "Jugador"
           << std::setw(10) << "Resultado" << std::right
           << std::setw(8) << "Oleadas" << std::setw(9) << "Bajas"
           << std::setw(9) << "DañoRec" << std::setw(8) << "Oro"
           << std::setw(9) << "Defensas" << std::setw(9) << "Puntaje" << '\n';
    }

    friend std::ostream& operator<<(std::ostream& os, const RegistroPartida& r) {
        os << std::left << std::setw(14) << r.jugador.substr(0, 13)
           << std::setw(10) << r.resultado << std::right
           << std::setw(8) << r.oleadasSuperadas << std::setw(9) << r.enemigosEliminados
           << std::setw(8) << r.danioRecibido << std::setw(8) << r.oroGastado
           << std::setw(9) << r.defensasColocadas << std::setw(9) << r.puntaje;
        return os;
    }

    // Lee del flujo la siguiente linea valida (las lineas dañadas se saltan).
    friend std::istream& operator>>(std::istream& is, RegistroPartida& r) {
        Texto linea;
        while (textoLeerLinea(is, linea)) {
            if (r.desdeLinea(linea)) return is;
        }
        return is;
    }
};

#endif
