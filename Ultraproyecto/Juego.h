#ifndef JUEGO_H
#define JUEGO_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include "Texto.h"
#include "Consola.h"
#include "Mapa.h"
#include "Torre.h"
#include "Jugador.h"
#include "Oleada.h"
#include "Defensa.h"
#include "Arquero.h"
#include "Catapulta.h"
#include "Mago.h"
#include "RegistroPartida.h"
#include "GestorEstadisticas.h"

const int JUEGO_TOTAL_OLEADAS = 10;
const int JUEGO_ORO_INICIAL = 200;
const int JUEGO_VIDA_CASTILLO = 250;
const int JUEGO_MAX_EVENTOS = 7;
const int JUEGO_NUM_TIPOS_DEFENSA = 3;

class Juego {
private:
    Consola consola;
    FabricaEnemigos fabricaEnemigos;
    Mapa mapa;
    Torre torre;
    Jugador jugador;
    std::vector<Defensa*> defensas;
    std::vector<Texto> eventos;
    GestorEstadisticas& estadisticas;

    int oleadaActual;
    int oleadasSuperadas;
    int enemigosEliminados;
    int defensasColocadas;
    int retardoMs;
    Texto aviso;

    Defensa* crearDefensa(int tipo, const Posicion& pos) const {
        switch (tipo) {
            case 1:  return new Arquero(pos);
            case 2:  return new Catapulta(pos);
            default: return new Mago(pos);
        }
    }

    // Sin <ctime> no hay reloj para sembrar std::rand. La semilla se arma con el nombre
    // del comandante y la cantidad de partidas guardadas, asi cada partida cambia.
    void sembrarAzar() const {
        unsigned semilla = 7919u * static_cast<unsigned>(estadisticas.contarPartidas());
        const Texto& nombre = jugador.getNombre();
        for (int i = 0; i < nombre.size(); i++) {
            semilla = semilla * 31u + static_cast<unsigned char>(nombre[i]);
        }
        std::srand(semilla);
    }

    void mostrarIntroduccion() const {
        std::cout <<
            "\n  =================================================================\n"
            "                    ¡EL REINO DE ELDORIA ESTA EN PELIGRO!\n"
            "  =================================================================\n\n"
            "  Hordas de enemigos avanzan por el camino (#) desde la entrada (E)\n"
            "  hacia el castillo (T). Antes de cada oleada puedes construir y\n"
            "  mejorar defensas en las casillas de pasto (.).\n\n"
            "  - Ganas oro por cada enemigo eliminado y al superar cada oleada.\n"
            "  - Si un enemigo llega al castillo, le hace daño.\n"
            "  - Resiste " << JUEGO_TOTAL_OLEADAS << " oleadas para salvar el reino.\n\n";
    }

    void elegirVelocidad() {
        std::cout << "\n  Velocidad de la animacion de las batallas:\n"
                  << "    1. Normal\n    2. Rapida\n    3. Sin animacion (solo resultado)\n";
        int op = consola.leerEntero("  Opcion: ", 1, 3);
        const int RETARDOS[3] = {150, 50, 0};   // arreglo
        retardoMs = RETARDOS[op - 1];
    }

    void mostrarEstado() const {
        std::cout << "  " << torre << "\n"
                  << "  " << jugador << "   |   Enemigos eliminados: " << enemigosEliminados << "\n";
    }

    void mostrarDefensas() const {
        std::cout << "\n  Tus defensas:\n";
        if (defensas.empty()) {
            std::cout << "    (todavia no tienes defensas)\n";
            return;
        }
        for (std::size_t i = 0; i < defensas.size(); i++) {
            std::cout << "    [" << (i + 1) << "] " << *defensas[i] << '\n';
        }
    }

    void mostrarCatalogo() const {
        Posicion p;
        std::cout << "\n  Defensas disponibles:\n";
        for (int t = 1; t <= JUEGO_NUM_TIPOS_DEFENSA; t++) {
            Defensa* muestra = crearDefensa(t, p);
            std::cout << "    " << t << ". " << std::left << std::setw(10) << muestra->getNombre()
                      << std::right << " Costo " << std::setw(3) << muestra->getCosto()
                      << "  Daño " << std::setw(2) << muestra->getDanio()
                      << "  Alcance " << std::fixed << std::setprecision(1) << muestra->getAlcance()
                      << "  - " << muestra->getDescripcion() << '\n';
            delete muestra;
        }
    }

    void colocarDefensa() {
        mostrarCatalogo();
        int tipo = consola.leerEntero("\n  Tipo de defensa (0 = cancelar): ", 0, JUEGO_NUM_TIPOS_DEFENSA);
        if (tipo == 0) return;

        const int COSTOS[JUEGO_NUM_TIPOS_DEFENSA] = {ARQUERO_COSTO, CATAPULTA_COSTO, MAGO_COSTO};
        int costo = COSTOS[tipo - 1];
        if (!(jugador >= costo)) {
            aviso = "No tienes suficiente oro (necesitas " + textoDeEntero(costo) + ").";
            return;
        }

        int fila = consola.leerEntero("  Fila (0-" + textoDeEntero(MAPA_FILAS - 1) + "): ",
                                       0, MAPA_FILAS - 1);
        int columna = consola.leerEntero("  Columna (0-" + textoDeEntero(MAPA_COLUMNAS - 1) + "): ",
                                          0, MAPA_COLUMNAS - 1);
        Posicion pos(fila, columna);
        if (!mapa.esLibre(pos)) {
            aviso = "Esa casilla no esta libre: solo se puede construir sobre pasto (.) vacio.";
            return;
        }

        Defensa* nueva = crearDefensa(tipo, pos);
        jugador -= costo;
        defensas.push_back(nueva);
        mapa.agregarDefensa(nueva);
        defensasColocadas++;
        aviso = nueva->getNombre() + " construido en " + pos.aTexto() + ".";
    }

    void mejorarDefensa() {
        if (defensas.empty()) {
            aviso = "No tienes defensas para mejorar.";
            return;
        }
        int indice = consola.leerEntero("  Numero de defensa a mejorar (0 = cancelar): ",
                                         0, static_cast<int>(defensas.size()));
        if (indice == 0) return;

        Defensa& d = *defensas[indice - 1];
        if (!d.puedeMejorar()) {
            aviso = d.getNombre() + " ya esta en el nivel maximo.";
            return;
        }
        int costo = d.getCostoMejora();
        if (!(jugador >= costo)) {
            aviso = "No tienes suficiente oro (la mejora cuesta " + textoDeEntero(costo) + ").";
            return;
        }
        jugador -= costo;
        d += 1;
        aviso = d.getNombre() + " mejorado a nivel " + textoDeEntero(d.getNivel()) + ".";
    }

    void verProximaOleada() const {
        consola.limpiar();
        std::cout << "\n  ===== INFORME DE EXPLORADORES: OLEADA " << oleadaActual << " =====\n\n"
                  << "  Se acercan unos " << (4 + 2 * oleadaActual) << " enemigos de nivel "
                  << oleadaActual << ".";
        if (oleadaActual % 5 == 0) {
            std::cout << " ¡Y un TROLL JEFE de nivel " << (oleadaActual + 2) << "!";
        }
        std::cout << "\n\n  Tipos de enemigo que pueden aparecer:\n\n";

        int tipos = fabricaEnemigos.tiposDisponibles(oleadaActual);
        for (int t = 0; t < tipos; t++) {
            Enemigo* muestra = fabricaEnemigos.crear(t, oleadaActual);
            std::cout << "  " << muestra->getSimbolo() << "  " << *muestra << "\n"
                      << "     " << muestra->getHabilidad() << "\n\n";   // polimorfismo
            delete muestra;
        }
        consola.pausar();
    }

    bool fasePreparacion() {
        aviso = "";
        std::vector<Enemigo*> sinEnemigos;
        while (true) {
            consola.limpiar();
            std::cout << "\n  ========== PREPARACION - OLEADA " << oleadaActual << " de "
                      << JUEGO_TOTAL_OLEADAS << " ==========\n\n";
            mapa.dibujar(sinEnemigos);
            std::cout << '\n';
            mostrarEstado();
            mostrarDefensas();
            if (!aviso.empty()) {
                std::cout << "\n  >> " << aviso << '\n';
                aviso = "";
            }
            std::cout << "\n  1. Colocar defensa\n"
                      << "  2. Mejorar defensa\n"
                      << "  3. Ver enemigos de la proxima oleada\n"
                      << "  4. ¡Comenzar oleada!\n"
                      << "  5. Rendirse\n";
            int op = consola.leerEntero("  Opcion: ", 1, 5);
            switch (op) {
                case 1: colocarDefensa(); break;
                case 2: mejorarDefensa(); break;
                case 3: verProximaOleada(); break;
                case 4: return true;
                case 5: {
                    int seguro = consola.leerEntero("  ¿Seguro que deseas rendirte? (1 = si, 0 = no): ", 0, 1);
                    if (seguro == 1) return false;
                    break;
                }
            }
        }
    }

    void registrarEvento(const Texto& texto) {
        eventos.push_back(texto);
    }

    void recortarEventos() {
        if (eventos.size() > static_cast<std::size_t>(JUEGO_MAX_EVENTOS)) {
            eventos.erase(eventos.begin(), eventos.end() - JUEGO_MAX_EVENTOS);
        }
    }

    void dibujarBatalla(Oleada& oleada, int turno) const {
        consola.limpiar();
        std::cout << "\n  ========== OLEADA " << oleada.getNumero() << " de " << JUEGO_TOTAL_OLEADAS
                  << "  -  Turno " << turno << " ==========\n\n";
        mapa.dibujar(oleada.getEnemigos());
        std::cout << '\n';
        mostrarEstado();
        std::cout << "  En el mapa: " << oleada.getEnMapa()
                  << "   Por aparecer: " << oleada.getPorAparecer() << "\n\n";

        const std::vector<Enemigo*>& enemigos = oleada.getEnemigos();
        std::size_t limite = enemigos.size() < 5 ? enemigos.size() : 5;
        for (std::size_t i = 0; i < limite; i++) {
            std::cout << "    " << *enemigos[i] << '\n';
        }
        if (enemigos.size() > limite) {
            std::cout << "    ... y " << (enemigos.size() - limite) << " mas\n";
        }

        std::cout << "\n  Registro de batalla:\n";
        for (const Texto& e : eventos) {
            std::cout << "    " << e << '\n';
        }
    }

    bool faseBatalla() {
        Oleada oleada(oleadaActual);
        const std::vector<Posicion>& camino = mapa.getCamino();
        std::vector<Enemigo*>& enemigos = oleada.getEnemigos();
        eventos.clear();
        int turno = 0;
        int bajasOleada = 0;
        int oroOleada = 0;

        while (!oleada.terminada() && torre) {
            turno++;

            for (Enemigo* e : enemigos) {
                e->alIniciarTurno();
                e->avanzar(camino, turno);
                if (e->llegoAlCastillo() && e->estaVivo()) {
                    e->atacar(torre);
                    registrarEvento("¡" + e->getNombre() + " golpea el castillo! -" +
                                    textoDeEntero(e->getDanio()) + " HP");
                }
            }

            oleada.aparecerSiguiente(camino);

            for (Defensa* d : defensas) {
                d->atacar(enemigos, eventos);
            }
            recortarEventos();

            int oro = 0;
            int bajas = oleada.retirarBajas(oro);
            bajasOleada += bajas;
            oroOleada += oro;
            enemigosEliminados += bajas;
            jugador += oro;

            if (retardoMs > 0) {
                dibujarBatalla(oleada, turno);
                consola.esperar(retardoMs);
            }
        }

        dibujarBatalla(oleada, turno);
        if (!torre) {
            std::cout << "\n  *** ¡EL CASTILLO HA CAIDO! ***\n";
            consola.pausar();
            return false;
        }

        int bono = 40 + 10 * oleadaActual;
        jugador += bono;
        std::cout << "\n  *** ¡OLEADA " << oleadaActual << " SUPERADA! ***\n"
                  << "  Enemigos eliminados: " << bajasOleada
                  << "  |  Oro por bajas: " << oroOleada
                  << "  |  Bono de oleada: " << bono << '\n';
        consola.pausar();
        return true;
    }

    void finalizar(bool victoria, bool rendido) {
        consola.limpiar();
        if (victoria) {
            std::cout << "\n  =================================================\n"
                         "     ¡VICTORIA! El reino de Eldoria esta a salvo.\n"
                         "  =================================================\n\n";
        } else if (rendido) {
            std::cout << "\n  Te has rendido. Eldoria queda en manos del enemigo...\n\n";
        } else {
            std::cout << "\n  =================================================\n"
                         "     DERROTA. El castillo ha sido destruido.\n"
                         "  =================================================\n\n";
        }

        RegistroPartida registro(jugador.getNombre(), victoria, oleadasSuperadas,
                                 enemigosEliminados, torre.getDanioRecibido(),
                                 jugador.getOroGastado(), defensasColocadas, torre.getHp());
        std::cout << "  Oleadas superadas:   " << oleadasSuperadas << " / " << JUEGO_TOTAL_OLEADAS << '\n'
                  << "  Enemigos eliminados: " << enemigosEliminados << '\n'
                  << "  Daño recibido:       " << torre.getDanioRecibido() << '\n'
                  << "  Oro gastado:         " << jugador.getOroGastado() << '\n'
                  << "  Defensas colocadas:  " << defensasColocadas << '\n'
                  << "  PUNTAJE:             " << registro.getPuntaje() << '\n';

        if (estadisticas.guardar(registro)) {
            std::cout << "\n  Partida guardada en el historial.\n";
        } else {
            std::cout << "\n  No se pudo guardar el historial.\n";
        }
        consola.pausar();
    }

public:
    Juego(GestorEstadisticas& estadisticas)
        : torre(JUEGO_VIDA_CASTILLO), jugador("Anonimo", JUEGO_ORO_INICIAL), estadisticas(estadisticas),
          oleadaActual(0), oleadasSuperadas(0), enemigosEliminados(0),
          defensasColocadas(0), retardoMs(150) {}

    ~Juego() {
        for (Defensa* d : defensas) delete d;
    }

    Juego(const Juego&) = delete;
    Juego& operator=(const Juego&) = delete;

    void jugar() {
        consola.limpiar();
        mostrarIntroduccion();
        Texto nombre = consola.leerLinea("  Nombre del comandante: ");
        if (nombre.empty()) nombre = "Anonimo";
        jugador = Jugador(nombre, JUEGO_ORO_INICIAL);
        sembrarAzar();
        elegirVelocidad();

        bool victoria = true;
        bool rendido = false;
        for (oleadaActual = 1; oleadaActual <= JUEGO_TOTAL_OLEADAS; oleadaActual++) {
            if (!fasePreparacion()) {
                rendido = true;
                victoria = false;
                break;
            }
            if (!faseBatalla()) {
                victoria = false;
                break;
            }
            oleadasSuperadas = oleadaActual;
        }
        finalizar(victoria, rendido);
    }
};

#endif
