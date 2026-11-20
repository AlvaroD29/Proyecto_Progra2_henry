#ifndef GESTOR_ESTADISTICAS_H
#define GESTOR_ESTADISTICAS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "Texto.h"
#include "RegistroPartida.h"

class GestorEstadisticas {
private:
    Texto rutaArchivo;

public:
    GestorEstadisticas(const Texto& ruta = "historial_partidas.txt")
        : rutaArchivo(ruta) {}

    // Agrega la partida al final del archivo de puntuaciones.
    bool guardar(const RegistroPartida& registro) const {
        std::ofstream archivo(rutaArchivo.c_str(), std::ios::app);
        if (!archivo) return false;
        registro.escribir(archivo);
        return static_cast<bool>(archivo);
    }

    std::vector<RegistroPartida> cargar() const {
        std::vector<RegistroPartida> registros;
        std::ifstream archivo(rutaArchivo.c_str());
        RegistroPartida r;
        while (archivo >> r) {
            registros.push_back(r);
        }
        return registros;
    }

    int contarPartidas() const {
        return static_cast<int>(cargar().size());
    }

    void mostrar() const {
        std::vector<RegistroPartida> registros = cargar();
        std::cout << "\n  ===================== HISTORIAL DE PARTIDAS =====================\n\n";
        if (registros.empty()) {
            std::cout << "  Aun no se ha jugado ninguna partida.\n";
            return;
        }

        std::cout << "  #   ";
        registros.front().imprimirEncabezado(std::cout);
        for (std::size_t i = 0; i < registros.size(); i++) {
            std::cout << "  " << std::left << std::setw(4) << (i + 1) << registros[i] << '\n';
        }

        int victorias = 0;
        int totalEnemigos = 0;
        for (const RegistroPartida& r : registros) {
            if (r.esVictoria()) victorias++;
            totalEnemigos += r.getEnemigosEliminados();
        }
        std::cout << "\n  Partidas jugadas: " << registros.size()
                  << "   Victorias: " << victorias
                  << "   Derrotas: " << (registros.size() - victorias)
                  << "   Enemigos eliminados en total: " << totalEnemigos << '\n';

        // Top 3 sin <algorithm>: se busca tres veces el mayor puntaje aun no usado.
        std::vector<bool> usado(registros.size(), false);
        std::cout << "\n  ---- TOP 3 PUNTAJES ----\n";
        for (int puesto = 1; puesto <= 3 && puesto <= static_cast<int>(registros.size()); puesto++) {
            int mejor = -1;
            for (std::size_t i = 0; i < registros.size(); i++) {
                if (!usado[i] && (mejor < 0 || registros[mejor] < registros[i])) {
                    mejor = static_cast<int>(i);
                }
            }
            usado[mejor] = true;
            std::cout << "  " << puesto << ". " << registros[mejor].getJugador()
                      << " - " << registros[mejor].getPuntaje() << " pts\n";
        }
    }
};

#endif
