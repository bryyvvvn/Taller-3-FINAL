#pragma once
#include <iostream>
#include "Transaccion.h"
#include <vector>

class HistorialTransacciones{
    private:
        struct Nodo{
            Transaccion data;
            Nodo* izq;
            Nodo* der;
            int altura;
            Nodo(Transaccion t):data(t), izq(nullptr), der(nullptr), altura(1){}
        };
    Nodo* raiz;
    
    int altura(Nodo* nodo);
    int factorBalance(Nodo* nodo);
    void actualizarAltura(Nodo * nodo);
    Nodo* rotarDerecha(Nodo* y);
    Nodo* rotarIzquierda(Nodo* x);
    Nodo* balancear(Nodo* nodo);
    Nodo* insertar(Nodo* nodo, Transaccion t);
    Nodo* encontrar(Nodo* nodo, int id) const;
    void inOrder(Nodo* nodo) const;

    public:
        HistorialTransacciones();
        void añadirTransaccion(Transaccion t);
        Transaccion* getTransaccion(int id) const;
        void printTransacciones() const;
        vector<Transaccion> obtenerTransacciones()const;
        void obtenerTransaccionesRecursivo(Nodo* nodo, std::vector<Transaccion>& transacciones) const;

};