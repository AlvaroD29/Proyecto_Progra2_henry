#ifndef CONSOLA_H
#define CONSOLA_H

#include <iostream>
#include <cstdlib>
#include "Texto.h"

class Consola {
public:
    void configurar() const {
#ifdef _WIN32
        std::system("chcp 65001 > nul");   // consola en UTF-8 (tildes y ¡ ¿)
#endif
    }

    void limpiar() const {
#ifdef _WIN32
        int r = std::system("cls");
#else
        int r = std::system("clear");
#endif
        (void)r;
    }

    // Pausa en milisegundos. Sin <chrono> ni <thread> se delega en el sistema
    // operativo a traves de std::system (cstdlib).
    void esperar(int milisegundos) const {
        if (milisegundos <= 0) return;
        std::cout.flush();
#ifdef _WIN32
        Texto orden = "powershell -NoProfile -Command Start-Sleep -Milliseconds " +
                      textoDeEntero(milisegundos);
#else
        int resto = milisegundos % 1000;
        Texto decimales = textoDeEntero(resto);
        while (decimales.size() < 3) decimales = "0" + decimales;
        Texto orden = "sleep " + textoDeEntero(milisegundos / 1000) + "." + decimales;
#endif
        int r = std::system(orden.c_str());
        (void)r;
    }

    Texto leerLinea(const Texto& mensaje) const {
        std::cout << mensaje;
        Texto linea;
        if (!textoLeerLinea(std::cin, linea)) {
            std::cout << "\nFin de la entrada. Saliendo del juego...\n";
            std::exit(0);
        }
        return linea;
    }

    int leerEntero(const Texto& mensaje, int minimo, int maximo) const {
        while (true) {
            Texto linea = leerLinea(mensaje);
            int valor = 0;
            if (linea.aEntero(valor) && valor >= minimo && valor <= maximo) {
                return valor;
            }
            std::cout << "  Valor invalido. Ingrese un numero entre "
                      << minimo << " y " << maximo << ".\n";
        }
    }

    void pausar() const {
        leerLinea("\nPresione ENTER para continuar...");
    }

    Texto barra(int actual, int maximo, int ancho) const {
        if (maximo <= 0) maximo = 1;
        if (actual < 0) actual = 0;
        if (actual > maximo) actual = maximo;
        int llenos = actual * ancho / maximo;
        return "[" + textoRepetir('#', llenos) + textoRepetir('-', ancho - llenos) + "]";
    }
};

#endif
