#ifndef OLEADA_H
#define OLEADA_H

#include <cstdlib>
#include <vector>
#include "Enemigo.h"
#include "Goblin.h"
#include "Orco.h"
#include "Troll.h"
#include "CaballeroOscuro.h"
#include "FabricaEnemigos.h"

const int OLEADA_NUM_TIPOS = 4;

class Oleada {
private:
    FabricaEnemigos fabrica;
    int numero;
    std::vector<Enemigo*> pendientes;   // enemigos que aun no entran al mapa
    std::vector<Enemigo*> enemigos;     // enemigos que ya estan en el mapa
    std::size_t siguiente;              // indice del proximo enemigo a aparecer

    void generar() {
        int cantidad = 4 + 2 * numero;
        int tipos = fabrica.tiposDisponibles(numero);
        for (int i = 0; i < cantidad; i++) {
            pendientes.push_back(fabrica.crear(std::rand() % tipos, numero));
        }
        if (numero % 5 == 0) {
            pendientes.push_back(new Troll(numero + 2));
        }
    }

public:
    Oleada(int numero) : numero(numero), siguiente(0) {
        generar();
    }

    ~Oleada() {
        for (Enemigo* e : pendientes) delete e;   // los ya aparecidos quedan en nullptr
        for (Enemigo* e : enemigos) delete e;
    }

    Oleada(const Oleada&) = delete;
    Oleada& operator=(const Oleada&) = delete;

    bool aparecerSiguiente(const std::vector<Posicion>& camino) {
        if (siguiente >= pendientes.size()) return false;
        Enemigo* e = pendientes[siguiente];
        pendientes[siguiente] = nullptr;
        siguiente++;
        e->colocarEnInicio(camino);
        enemigos.push_back(e);
        return true;
    }

    int retirarBajas(int& oroGanado) {
        int eliminados = 0;
        for (auto it = enemigos.begin(); it != enemigos.end();) {
            Enemigo* e = *it;
            if (!e->estaVivo()) {
                eliminados++;
                oroGanado += e->getRecompensa();
                delete e;
                it = enemigos.erase(it);
            } else if (e->llegoAlCastillo()) {
                delete e;
                it = enemigos.erase(it);
            } else {
                ++it;
            }
        }
        return eliminados;
    }

    std::vector<Enemigo*>& getEnemigos() { return enemigos; }
    int getNumero() const { return numero; }
    int getTotal() const { return static_cast<int>(pendientes.size()); }
    int getPorAparecer() const { return static_cast<int>(pendientes.size() - siguiente); }
    int getEnMapa() const { return static_cast<int>(enemigos.size()); }
    bool terminada() const { return getPorAparecer() == 0 && enemigos.empty(); }
};

#endif
