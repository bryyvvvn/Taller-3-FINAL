#include "../Include/HistorialTransacciones.h"
#include <vector>
using namespace std;

HistorialTransacciones:: HistorialTransacciones(): raiz(nullptr){}
//Estructura y funcionamiento del arbol AVl en general.

int HistorialTransacciones::altura(Nodo* nodo){
    return nodo ? nodo->altura: 0;
}

int HistorialTransacciones:: factorBalance(Nodo* nodo){
    return nodo ? altura(nodo->izq)- altura(nodo->der):0;
}

void HistorialTransacciones::actualizarAltura(Nodo *nodo){
    if(nodo){
        nodo->altura= 1 +max(altura(nodo->izq),altura(nodo->der));
        
    }
}

HistorialTransacciones::Nodo* HistorialTransacciones::rotarDerecha(Nodo* y){
   
    Nodo* x = y->izq;
    Nodo* T2 = x->der;
    x->der = y;
    y->izq = T2;
    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

HistorialTransacciones::Nodo* HistorialTransacciones::rotarIzquierda(Nodo* x){
    
    Nodo* y = x->der;
    Nodo* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}
HistorialTransacciones:: Nodo* HistorialTransacciones::balancear (Nodo *nodo){
    if(!nodo){return nullptr;}

  
    actualizarAltura(nodo);
    int fb= factorBalance(nodo);
   
    
    if(fb>1){
        if (factorBalance(nodo->izq)<0){
            nodo->izq = rotarIzquierda(nodo->izq);

        }
        return rotarDerecha(nodo);
    }
    if(fb<-1){
        if(factorBalance(nodo->der) >0){
            nodo->der=rotarDerecha(nodo->der);
        }
        return rotarIzquierda(nodo);
    }
    return nodo;
}

HistorialTransacciones::Nodo* HistorialTransacciones::insertar(Nodo* nodo,Transaccion t){
    if(!nodo){ 
      
        return new Nodo(t);
    }
    if(t.getId()< nodo->data.getId()){
       
        nodo->izq= insertar(nodo->izq,t);
    }else{
        
        nodo->der=insertar(nodo->der,t);

    }
    return balancear(nodo);
}
void HistorialTransacciones::inOrder(Nodo*nodo) const{
    if(nodo!=nullptr){
        inOrder(nodo->izq);
       
        nodo->data.printTransaccion();
        inOrder(nodo->der);
        
    }
}
HistorialTransacciones::Nodo* HistorialTransacciones::encontrar(Nodo* nodo, int id) const{
    if(!nodo) return nullptr;
    if(id< nodo->data.getId()) return encontrar(nodo->izq,id);
    if(id> nodo->data.getId()) return encontrar(nodo->der,id);
    return nodo;
}


    vector<Transaccion> HistorialTransacciones::obtenerTransacciones() const {
    std::vector<Transaccion> transacciones;
    obtenerTransaccionesRecursivo(raiz, transacciones);
    return transacciones;
}


void HistorialTransacciones::obtenerTransaccionesRecursivo(Nodo* nodo, std::vector<Transaccion>& transacciones) const {
    if (nodo) {
        obtenerTransaccionesRecursivo(nodo->izq, transacciones);
        transacciones.push_back(nodo->data);
        obtenerTransaccionesRecursivo(nodo->der, transacciones);
    }
}
void HistorialTransacciones::añadirTransaccion(Transaccion t){
    raiz= insertar(raiz,t);
    
}  

Transaccion* HistorialTransacciones::getTransaccion(int id) const{
    Nodo* nodo=encontrar(raiz,id);
    return nodo ? &nodo->data:nullptr;
}

void HistorialTransacciones::printTransacciones() const{
    inOrder(raiz);
}
