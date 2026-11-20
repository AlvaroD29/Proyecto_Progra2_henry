#ifndef FABRICA_ENEMIGOS_H
#define FABRICA_ENEMIGOS_H

#include "Enemigo.h"
#include "Goblin.h"
#include "Orco.h"
#include "Troll.h"
#include "CaballeroOscuro.h"

// Crea enemigos segun su tipo. Antes estas dos funciones eran miembros estaticos de
// Oleada; ahora son metodos normales de esta clase y cada usuario tiene su propia fabrica.
class FabricaEnemigos {
public:
    int tiposDisponibles(int n) const {
        if (n >= 5) return 4;
        if (n >= 3) return 3;
        return 2;
    }

    Enemigo* crear(int tipo, int nivel) const {
        switch (tipo) {
            case 0:  return new Goblin(nivel);
            case 1:  return new Orco(nivel);
            case 2:  return new Troll(nivel);
            default: return new CaballeroOscuro(nivel);
        }
    }
};

#endif
