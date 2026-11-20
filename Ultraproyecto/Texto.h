#ifndef TEXTO_H
#define TEXTO_H

#include <iostream>
const int CAPACIDAD_TEXTO = 192;
class Texto {
private:
    char datos[CAPACIDAD_TEXTO];
    int largo;

    bool esEspacio(char c) const { return c == ' ' || c == '\t'; }
public:
    Texto() : largo(0) { datos[0] = '\0'; }

    Texto(const char* texto) : largo(0) {
        datos[0] = '\0';
        if (texto != nullptr) {
            while (*texto != '\0' && largo < CAPACIDAD_TEXTO - 1) {
                datos[largo++] = *texto++;
            }
            datos[largo] = '\0';
        }
    }
    int size() const { return largo; }
    bool empty() const { return largo == 0; }
    const char* c_str() const { return datos; }
    char operator[](int i) const { return datos[i]; }
    char& operator[](int i) { return datos[i]; }
    char back() const { return datos[largo - 1]; }

    void push_back(char c) {
        if (largo < CAPACIDAD_TEXTO - 1) {
            datos[largo++] = c;
            datos[largo] = '\0';
        }
    }

    void pop_back() {
        if (largo > 0) datos[--largo] = '\0';
    }

    Texto& operator+=(const Texto& otro) {
        for (int i = 0; i < otro.largo; i++) push_back(otro.datos[i]);
        return *this;
    }

    friend Texto operator+(const Texto& a, const Texto& b) {
        Texto resultado = a;
        resultado += b;
        return resultado;
    }

    bool operator==(const Texto& otro) const {
        if (largo != otro.largo) return false;
        for (int i = 0; i < largo; i++) {
            if (datos[i] != otro.datos[i]) return false;
        }
        return true;
    }

    bool operator!=(const Texto& otro) const { return !(*this == otro); }

    Texto substr(int inicio, int cantidad) const {
        Texto resultado;
        for (int i = inicio; i < largo && i < inicio + cantidad; i++) {
            resultado.push_back(datos[i]);
        }
        return resultado;
    }

    bool aEntero(int& valor) const {
        int i = 0;
        while (i < largo && esEspacio(datos[i])) i++;
        bool negativo = false;
        if (i < largo && (datos[i] == '+' || datos[i] == '-')) {
            negativo = (datos[i] == '-');
            i++;
        }
        int digitos = 0;
        long long acumulado = 0;
        while (i < largo && datos[i] >= '0' && datos[i] <= '9') {
            acumulado = acumulado * 10 + (datos[i] - '0');
            if (acumulado > 2147483647LL) return false;
            digitos++;
            i++;
        }
        if (digitos == 0) return false;
        while (i < largo && esEspacio(datos[i])) i++;
        if (i != largo) return false;
        valor = negativo ? -static_cast<int>(acumulado) : static_cast<int>(acumulado);
        return true;
    }

    int dividir(char separador, Texto salida[], int maximo) const {
        int cantidad = 0;
        Texto actual;
        for (int i = 0; i <= largo; i++) {
            if (i == largo || datos[i] == separador) {
                if (cantidad < maximo) salida[cantidad] = actual;
                cantidad++;
                actual = Texto();
            } else {
                actual.push_back(datos[i]);
            }
        }
        return cantidad;
    }

    friend std::ostream& operator<<(std::ostream& os, const Texto& t) {
        return os << t.datos;   
    }
};


inline Texto textoDeEntero(int numero) {
    Texto resultado;
    long long n = numero;        
    bool negativo = n < 0;
    if (negativo) n = -n;
    char invertido[24];
    int cantidad = 0;
    do {
        invertido[cantidad++] = static_cast<char>('0' + n % 10);
        n /= 10;
    } while (n > 0);
    if (negativo) resultado.push_back('-');
    while (cantidad > 0) resultado.push_back(invertido[--cantidad]);
    return resultado;
}

inline Texto textoRepetir(char c, int veces) {
    Texto resultado;
    for (int i = 0; i < veces; i++) resultado.push_back(c);
    return resultado;
}

inline bool textoLeerLinea(std::istream& entrada, Texto& destino) {
    char buffer[CAPACIDAD_TEXTO];
    destino = Texto();
    entrada.getline(buffer, CAPACIDAD_TEXTO);
    if (entrada.fail()) {
        if (entrada.eof()) return false;  
        entrada.clear();                 
        entrada.ignore(1000000, '\n');
    }
    destino = Texto(buffer);
    if (!destino.empty() && destino.back() == '\r') destino.pop_back();
    return true;
}

#endif
