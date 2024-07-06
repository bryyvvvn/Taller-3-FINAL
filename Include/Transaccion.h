#pragma once
#include <iostream>
using namespace std;

class Transaccion
{
    private:
        int id;
        string cuentaOrigen;    
        string cuentaDestino;
        double monto;
        string ubicacion;
        time_t hora;
        string razonSospecha;
    public:
        Transaccion(int _id, string _cuentaorigen, string _cuentadestino, double _monto, string _ubicacion,time_t hora);
       
        void printTransaccion() const;

        int getId() const; 
        string getCuentaOrigen() const;
        string getCuentaDestino() const;
        double getMonto() const;
        string getUbicacion() const;
        time_t getHora() const;
        string getRazonSospecha() const;
        void setId(int _id);
        void setCuentaOrigen(const string& _cuentaorigen);
        void setCuentaDestino(const string& _cuentadestino);
        void setMonto(double _monto);
        void setUbicacion(const string& _ubicacion);
        void setHora(time_t _hora);
        void setRazonSospecha(const string _razonSospecha);


};