#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "Consola.h"
#include "Juego.h"
#include "GestorEstadisticas.h"

class Menu {
private:
    Consola consola;
    GestorEstadisticas estadisticas;

    void mostrarTitulo() const {
        std::cout <<
            "\n"
            "        |>>>                                    |>>>\n"
            "        |                                       |\n"
            "    _  _|_  _        DEFENSA DEL REINO      _  _|_  _\n"
            "   |;|_|;|_|;|                             |;|_|;|_|;|\n"
            "   \\\\.    .  /          ~ Eldoria ~         \\\\.    .  /\n"
            "    \\\\:  .  /                                \\\\:  .  /\n"
            "     ||:   |                                  ||:   |\n"
            "     ||:.  |                                  ||:.  |\n"
            "  ===========================================================\n\n";
    }

public:
    Menu() : estadisticas("historial_partidas.txt") {}

    void mostrar() {
        consola.configurar();
        int opcion = 0;
        do {
            consola.limpiar();
            mostrarTitulo();
            std::cout << "     1. Jugar\n"
                      << "     2. Ver estadisticas\n"
                      << "     3. Salir\n\n";
            opcion = consola.leerEntero("     Seleccione una opcion: ", 1, 3);

            switch (opcion) {
                case 1: {
                    Juego juego(estadisticas);
                    juego.jugar();
                    break;
                }
                case 2:
                    consola.limpiar();
                    estadisticas.mostrar();
                    consola.pausar();
                    break;
                case 3:
                    std::cout << "\n     ¡Hasta pronto, comandante!\n";
                    break;
            }
        } while (opcion != 3);
    }
};

#endif
