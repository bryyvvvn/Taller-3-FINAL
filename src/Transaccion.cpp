#include "../Include/Transaccion.h"
using namespace std;

Transaccion:: Transaccion(int _id, string _cuentaOrigen, string _cuentadestino, double _monto, string _ubicacion,time_t _hora)
            : id(_id),cuentaOrigen(_cuentaOrigen),cuentaDestino(_cuentadestino),monto(_monto),ubicacion(_ubicacion),hora(_hora){
                
            };

   
void Transaccion:: printTransaccion() const{
    cout << "ID: " << id << ", Cuenta Origen: " << cuentaOrigen << ", Cuenta Destino: " << cuentaDestino
         << ", Monto: " << monto << ", Ubicacion: " << ubicacion 
         << ", Fecha y hora: " << asctime(localtime(&hora));
}

int Transaccion:: getId() const{return id;}
string Transaccion:: getCuentaOrigen() const{return cuentaOrigen;}
string Transaccion::getCuentaDestino() const{return cuentaDestino;}
double Transaccion::getMonto() const{return monto;}
string Transaccion::getUbicacion() const{ return ubicacion;}
time_t Transaccion::getHora() const {return hora;}
string Transaccion::getRazonSospecha() const{return razonSospecha;}   

void Transaccion::setId(int _id) { id = _id; }
void Transaccion::setCuentaOrigen(const string& _cuentaorigen) { cuentaOrigen = _cuentaorigen; }
void Transaccion::setCuentaDestino(const string& _cuentadestino) { cuentaDestino = _cuentadestino; }
void Transaccion::setMonto(double _monto) { monto = _monto; }
void Transaccion::setUbicacion(const string& _ubicacion) { ubicacion = _ubicacion; }
void Transaccion::setHora(time_t _hora) { hora = _hora; }
void Transaccion::setRazonSospecha(string _razonSospecha){razonSospecha=_razonSospecha;}