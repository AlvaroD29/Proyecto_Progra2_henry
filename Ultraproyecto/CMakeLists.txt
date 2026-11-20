cmake_minimum_required(VERSION 4.3)
project(untitled19)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled19 main.cpp
        Menu.h
        Juego.h
        Mapa.h
        Oleada.h
        Torre.h
        Jugador.h
        Posicion.h
        Enemigo.h
        FabricaEnemigos.h
        Goblin.h
        Orco.h
        Troll.h
        CaballeroOscuro.h
        Defensa.h
        Arquero.h
        Catapulta.h
        Mago.h
        RegistroPartida.h
        GestorEstadisticas.h
        Consola.h
        Texto.h)
