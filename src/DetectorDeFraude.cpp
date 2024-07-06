#include "../Include/DetectorDeFraude.h"
using namespace std;


//ABB
DetectorDeFraude::DetectorDeFraude() : raiz(nullptr){}
double DetectorDeFraude::montoEspecifico=(999999999);
int DetectorDeFraude::periodoTiempoTransacciones = -9999999;
int DetectorDeFraude::periodoTiempoUbicaciones = -99999999;
int DetectorDeFraude::cantTransaccionesPocoTiempo = -1;
int DetectorDeFraude::cantUbicacionesPocoTiempo = -1;


void DetectorDeFraude::insertar(Nodo*& nodo, Transaccion t){
    if(!nodo){
        nodo=new Nodo(t);

    }else if (t.getId()< nodo->data.getId()){
        insertar(nodo->izq,t);
   
    } else{
        insertar(nodo->der,t);
    }

}

void DetectorDeFraude::añadirTransaccion(Transaccion t) {
    insertar(raiz,t);
}

void DetectorDeFraude::inOrder(Nodo* nodo) const{
    if(nodo){
        inOrder(nodo->izq);
        nodo->data.printTransaccion();
        inOrder(nodo->der);
    }
}
DetectorDeFraude::Nodo* DetectorDeFraude::eliminarNodo(Nodo* nodo, int id) {
    if (nodo == nullptr) {
        return nodo;
    }

    if (id < nodo->data.getId()) {
        nodo->izq = eliminarNodo(nodo->izq, id);
    } else if (id > nodo->data.getId()) {
        nodo->der = eliminarNodo(nodo->der, id);
    } else {
        if (nodo->izq == nullptr) {
            Nodo* temp = nodo->der;
            delete nodo;
            return temp;
        } else if (nodo->der == nullptr) {
            Nodo* temp = nodo->izq;
            delete nodo;
            return temp;
        }

        Nodo* temp = obtenerMinimo(nodo->der);
        nodo->data = temp->data;
        nodo->der = eliminarNodo(nodo->der, temp->data.getId());
    }
    return nodo;
}

DetectorDeFraude::Nodo* DetectorDeFraude::obtenerMinimo(Nodo* nodo) {
    Nodo* actual = nodo;
    while (actual && actual->izq != nullptr) {
        actual = actual->izq;
    }
    return actual;
}
void DetectorDeFraude::eliminarTransaccion(int id) {
    raiz = eliminarNodo(raiz, id);
}

void DetectorDeFraude::printTransacciones() const{
    inOrder(raiz);
}

bool DetectorDeFraude::transaccionesFrecuentes(Nodo* nodo, const string& cuentaOrigen, time_t ultimaTransaccion, int& contador, string& ultimaCuenta, time_t& ultimaHora) const {
    if (nodo == nullptr) {
        return false;
    }

    if (transaccionesFrecuentes(nodo->izq, cuentaOrigen, ultimaTransaccion, contador, ultimaCuenta, ultimaHora)) {
        return true;
    }

    if (nodo->data.getCuentaOrigen() == cuentaOrigen) {
        if (difftime(ultimaTransaccion, nodo->data.getHora()) <= periodoTiempoTransacciones * 3600) {
            contador++;
            if (contador >= cantTransaccionesPocoTiempo) {
                return true;
            }
        }
        ultimaCuenta = nodo->data.getCuentaOrigen();
        ultimaHora = nodo->data.getHora();
    }

    return transaccionesFrecuentes(nodo->der, cuentaOrigen, ultimaTransaccion, contador, ultimaCuenta, ultimaHora);
}

bool DetectorDeFraude::ubicacionesDistintasEnCortoTiempo(Nodo* nodo, string& ultimaCuenta, string& ultimaUbicacion, time_t& ultimaHora) const {
    if (nodo == nullptr) {
        return false;
    }

    if (ubicacionesDistintasEnCortoTiempo(nodo->izq, ultimaCuenta, ultimaUbicacion, ultimaHora)) {
        return true;
    }

    string cuentaActual = nodo->data.getCuentaOrigen();
    string ubicacionActual = nodo->data.getUbicacion();
    time_t tiempoActual = nodo->data.getHora();

    if (cuentaActual == ultimaCuenta) {
        if (ubicacionActual != ultimaUbicacion) {
            if (difftime(tiempoActual, ultimaHora) <= periodoTiempoUbicaciones) {
                return true;
            }
        }
    }

    ultimaCuenta = cuentaActual;
    ultimaUbicacion = ubicacionActual;
    ultimaHora = tiempoActual;

    return ubicacionesDistintasEnCortoTiempo(nodo->der, ultimaCuenta, ultimaUbicacion, ultimaHora);
}

vector<Transaccion> DetectorDeFraude::detectarSospechas(Nodo* nodo) const {
    vector<Transaccion> transaccionesSospechosas;
    if (!nodo) return transaccionesSospechosas;

    vector<Transaccion> izqSospechas = detectarSospechas(nodo->izq);
    vector<Transaccion> derSospechas = detectarSospechas(nodo->der);

    transaccionesSospechosas.insert(transaccionesSospechosas.end(), izqSospechas.begin(), izqSospechas.end());
    if (nodo->data.getMonto() > montoEspecifico) {
        nodo->data.setRazonSospecha("Monto especificado superado.");
        transaccionesSospechosas.push_back(nodo->data);
    } else {
        string ultimaCuenta = " ";
        time_t ultimaHora = 0;
        int contador = 0;
        if (transaccionesFrecuentes(nodo, nodo->data.getCuentaOrigen(), nodo->data.getHora(), contador, ultimaCuenta, ultimaHora)) {
            nodo->data.setRazonSospecha("Corto periodo de tiempo entre esta transaccion y la anterior.");
            transaccionesSospechosas.push_back(nodo->data);
        } else {
            string ultimaUbicacion = "";
            ultimaHora = 0;
            if (ubicacionesDistintasEnCortoTiempo(nodo, ultimaCuenta, ultimaUbicacion, ultimaHora)) {
                nodo->data.setRazonSospecha("Transaccion realizada en ubicaciones distintas en muy poco tiempo.");
                transaccionesSospechosas.push_back(nodo->data);
            }
        }
    }
    transaccionesSospechosas.insert(transaccionesSospechosas.end(), derSospechas.begin(), derSospechas.end());

    return transaccionesSospechosas;
}

vector<Transaccion> DetectorDeFraude::getSospechas() const{

    return detectarSospechas(raiz);
}
//SETTERS RESTRICCIONES
void DetectorDeFraude::setMontoEspecifico(double _montoEspecifico) { 
    montoEspecifico = _montoEspecifico; }
   
void DetectorDeFraude::setPeriodoTiempoTransacciones(int _periodoTiempoTransacciones){
    periodoTiempoTransacciones=_periodoTiempoTransacciones;}
void DetectorDeFraude::setPeriodoTiempoUbicaciones(int _periodoTiempoUbicaciones){
    periodoTiempoUbicaciones=_periodoTiempoUbicaciones;}
void DetectorDeFraude::setcantTransaccionesPocoTiempo(int _cantTransaccionesPocoTiempo){
    cantTransaccionesPocoTiempo=_cantTransaccionesPocoTiempo;}     
void DetectorDeFraude::setcantUbicacionesPocoTiempo(int _cantUbicacionesPocoTiempo){
    cantUbicacionesPocoTiempo=_cantUbicacionesPocoTiempo;}     

        
        
