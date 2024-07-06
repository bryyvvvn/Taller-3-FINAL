#pragma once
#include <iostream>
#include <vector>
#include "Transaccion.h"
#include "HistorialTransacciones.h"

class DetectorDeFraude{
    private:
        struct Nodo{
            Transaccion data;
            Nodo* izq;
            Nodo* der;
            Nodo(Transaccion t) : data(t), izq(nullptr), der(nullptr){}

        };

        Nodo* raiz;
        static double montoEspecifico;
        static int periodoTiempoTransacciones;
        static int periodoTiempoUbicaciones;
        static int cantTransaccionesPocoTiempo;
        static int cantUbicacionesPocoTiempo;

        void insertar(Nodo*& nodo, Transaccion t);
        void inOrder(Nodo* nodo) const;
        vector<Transaccion> detectarSospechas(Nodo* nodo) const;
        Nodo* eliminarNodo(Nodo* nodo, int id);
        Nodo* obtenerMinimo(Nodo* nodo);
        bool transaccionesFrecuentes(Nodo* nodo, const string& cuentaOrigen, time_t ultimaTransaccion, int& contador, string& ultimaCuenta, time_t& ultimaHora) const;
        bool ubicacionesDistintasEnCortoTiempo(Nodo* nodo, string& ultimaCuenta, string& ultimaUbicacion, time_t& ultimaHora) const;
    public:
        DetectorDeFraude();
        void anadirTransaccion (Transaccion t);
        void printTransacciones() const;
        void eliminarTransaccion(int id);
        bool esTransaccionFrecuente(Nodo* raiz, const string& cuentaOrigen, time_t ultimaTransaccion) const;
        vector<Transaccion> getSospechas() const;

        static void setMontoEspecifico(double _montoEspecifico);
        static void setPeriodoTiempoTransacciones(int _periodoTiempoTransacciones);
        static  void setPeriodoTiempoUbicaciones(int _periodoTiempoUbicaciones);
        static void setcantTransaccionesPocoTiempo(int _cantTransaccionesPocoTiempo);
        static void setcantUbicacionesPocoTiempo(int _cantUbicacionesPocoTiempo);
        
        
};