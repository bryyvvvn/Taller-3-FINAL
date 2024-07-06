#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include "../Include/DetectorDeFraude.h"
#include "../Include/HistorialTransacciones.h"
#include "../Include/Transaccion.h"

//LEER ARCHIVO .TXT

time_t stringAFecha(const string& fechaString) {
    tm tm = {};
    istringstream ss(fechaString);
    char delimiter;

    
    ss >> tm.tm_year;   
    ss >> delimiter;    
    if (delimiter != '-') {
        cerr << "Error al leer el delimitador de fecha: " << delimiter << endl;
        return -1; 
    }
    ss >> tm.tm_mon;    
    ss >> delimiter;    
    if (delimiter != '-') {
        cerr << "Error al leer el delimitador de fecha: " << delimiter << endl;
        return -1;  
    }
    ss >> tm.tm_mday;

    
    tm.tm_year -= 1900; 
    tm.tm_mon--;        

    
    time_t fecha = mktime(&tm);
    if (fecha == -1) {
        cerr << "Error al convertir la estructura tm a time_t" << endl;
        return -1;
    }
    
    
    return fecha;
}
time_t stringAHora(const string& horaString) {
    tm tm = {};
    istringstream ss(horaString);

    char delimiter;

    
    ss >> tm.tm_hour;   
    ss >> delimiter;    
    if (delimiter != ':') {
        std::cerr << "Error al leer el delimitador de hora: " << delimiter << std::endl;
        return -1;  
    }
    ss >> tm.tm_min;    
    ss >> delimiter;    
    if (delimiter != ':') {
        std::cerr << "Error al leer el delimitador de hora: " << delimiter << std::endl;
        return -1;  
    }
    ss >> tm.tm_sec;    

    
    tm.tm_year = 70; 
    tm.tm_mon = 0;   
    tm.tm_mday = 1;  

    
    time_t hora = mktime(&tm);
    if (hora == -1) {
        std::cerr << "Error al convertir la estructura tm a time_t" << std::endl;
        return -1;
    }

    
    

    return hora;
}
void leerArchivoYAgregarTransacciones(const string& nombreArchivo, HistorialTransacciones& historial, DetectorDeFraude& detectorFraude) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << endl;
        return;
    }

    string linea;
    while (getline(archivo, linea)) {
        istringstream iss(linea);
        string token;

        int id;
        string cuentaOrigen, cuentaDestino, ubicacion;
        double monto;
        string fechaStr, horaStr;

       
        if (!getline(iss, token, ',')) {
            cerr << "Error al leer el ID de la línea: " << linea << endl;
            continue;
        }
        id = stoi(token);

        if (!getline(iss, cuentaOrigen, ',')) {
            cerr << "Error al leer la cuenta de origen de la línea: " << linea << endl;
            continue;
        }

        if (!getline(iss, cuentaDestino, ',')) {
            cerr << "Error al leer la cuenta de destino de la línea: " << linea << endl;
            continue;
        }

        if (!getline(iss, token, ',')) {
            cerr << "Error al leer el monto de la línea: " << linea << endl;
            continue;
        }
        monto = stod(token);

        if (!getline(iss, ubicacion, ',')) {
            cerr << "Error al leer la ubicación de la línea: " << linea << endl;
            continue;
        }

        if (!getline(iss, fechaStr, ',')) {
            cerr << "Error al leer la fecha de la línea: " << linea << endl;
            continue;
        }

        if (!getline(iss, horaStr, ',')) {
            cerr << "Error al leer la hora de la línea: " << linea << endl;
            continue;
        }

        
        time_t fecha = stringAFecha(fechaStr);
        time_t hora = stringAHora(horaStr);

        if (fecha == -1 || hora == -1) {
            cerr << "Error al convertir la fecha o hora: " << fechaStr << ", " << horaStr << endl;
            continue; 
        }

        
        tm tm_fecha = *localtime(&fecha);
        tm tm_hora = *localtime(&hora);

        
        tm tm_fechaYHora = tm_fecha;
        tm_fechaYHora.tm_hour = tm_hora.tm_hour;
        tm_fechaYHora.tm_min = tm_hora.tm_min;
        tm_fechaYHora.tm_sec = tm_hora.tm_sec;

       
        time_t fechaYHora = mktime(&tm_fechaYHora);
        if (fechaYHora == -1) {
            cerr << "Error al convertir la estructura tm a time_t" << endl;
            continue;
        }

        
        Transaccion transaccion(id, cuentaOrigen, cuentaDestino, monto, ubicacion, fechaYHora);
        historial.anadirTransaccion(transaccion);
        detectorFraude.anadirTransaccion(transaccion);
        
    }

    archivo.close();
}
string timeToStringFecha(time_t tiempo) {
    struct tm* tiempoInfo;
    char buffer[80];

    tiempoInfo = localtime(&tiempo);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tiempoInfo);
    return string(buffer);
}


string timeToStringHora(time_t tiempo) {
    struct tm* tiempoInfo;
    char buffer[80];

    tiempoInfo = localtime(&tiempo);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", tiempoInfo);
    return string(buffer);
}

void actualizarArchivo (const HistorialTransacciones& historial ){
    ofstream archivo("ArchivosTxt/Transacciones.txt");
    if(!archivo.is_open()){
        cerr<<"No se pudo abrir el archivo"<<endl;
        return;
    }
    auto transacciones = historial.obtenerTransacciones();
    for (const auto& transaccion : transacciones) {
        string hora=timeToStringHora(transaccion.getHora());
        string fecha=timeToStringFecha(transaccion.getHora());
        archivo << transaccion.getId() << ','
                << transaccion.getCuentaOrigen() << ','
                << transaccion.getCuentaDestino() << ','
                << transaccion.getMonto() << ','
                << transaccion.getUbicacion() << ','
                << fecha << ','
                << hora<< endl;
    }

    archivo.close();
}

int generadorId(){
    int id;
    id=100000+ rand()%999999;
    return id;
}
//OPCION 1
void agregarTransacciones(HistorialTransacciones historial,DetectorDeFraude detectorFraude){
        int id=0;
        string destino,origen,ubicacion;
        double monto;
        id=generadorId();

        cout<<"-------------------------------------------------"<<endl;
        cout<<"Ingrese la cuenta de origen: "<<endl;
        cin>>origen;  
        cout<<"Ingrese la cuenta de destino: "<<endl;
        cin>>destino;
        do
        {
        cout<<"Ingrese el monto de la transaccion: "<<endl;
        cin>>monto;
        if(monto<1){cout<<"El monto tiene que ser mayor a 0.";}
        } while (monto<1);
        
        
        cout<<"Indique la ubicacion desde donde se realiza la transaccion: "<<endl;
        cin>>ubicacion;
        time_t hora = time(nullptr);
        Transaccion transaccion(id, origen, destino, monto, ubicacion, hora);
        historial.anadirTransaccion(transaccion);
        detectorFraude.anadirTransaccion(transaccion);
        cout<<"Transacción con ID: "<<id<<" agregada correctamente"<<endl;
        actualizarArchivo(historial);
}


//OPCION 2
void modificarTransacciones(HistorialTransacciones historial,DetectorDeFraude detectorFraude){
    int idBuscada=-1;
    int opcion=-1;
    Transaccion* transaccionElegida;
    cout<<"-------------------------------------------------"<<endl;
    cout<<"Transacciones: "<<endl;
    historial.printTransacciones();

    do{
    cout<<"Indique el ID de la transaccion que modificara: "<<endl;
    cin>>idBuscada;
    transaccionElegida=historial.getTransaccion(idBuscada);
    
    if(transaccionElegida==nullptr){
        cout<<"-------------------------------------------------"<<endl;
        cout<<"No existe la transaccion especificada"<<endl;
        cout<<"-------------------------------------------------"<<endl;
        }

    }while(transaccionElegida==nullptr);

    cout<<"-------------------------------------------------"<<endl;
    cout<<"¿Que desea modificar?"<<endl;
    cout<<"1.ID."<<endl;
    cout<<"2.Cuenta de Origen."<<endl;
    cout<<"3.Cuenta de Destino."<<endl;
    cout<<"4.Monto"<<endl;
    cout<<"5.Ubicacion"<<endl;

    do{
    cout<<"-------------------------------------------------"<<endl;
    cout<<"Indique una opcion: ";
    cout<<"-------------------------------------------------"<<endl;
    cin>>opcion;
    }while(opcion<1||opcion>5);

    int idNueva;
    string cuentaOrigenNueva;
    string cuentaDestinoNueva;
    double montoNuevo;
    string ubicacionNueva;

    switch (opcion)
    {
    case 1:
        
        cout<<"Ingrese la ID nueva";
        cin>>idNueva;
        transaccionElegida->setId(idNueva);
        cout<<"Se ha actualizado la ID"<<endl;
        break;
    case 2:
        
        cout<<"Ingrese la nueva cuenta de origen";
        cin>>cuentaOrigenNueva;
        transaccionElegida->setCuentaOrigen(cuentaOrigenNueva);
        cout<<"Se ha actualizado la cuenta de origen"<<endl;
        break;

    case 3:
        cout<<"Ingrese la nueva cuenta de destino";
        cin>>cuentaDestinoNueva;
        transaccionElegida->setCuentaDestino(cuentaDestinoNueva);
        cout<<"Se ha actualizado la cuenta de destino"<<endl;
        break;
    case 4:
    do{
        cout<<"Ingrese el nuevo monto: ";
        cin>>montoNuevo;
        if(montoNuevo<1){cout<<"El monto tiene que ser mayor a 0.";}
        }while(montoNuevo<1);

        transaccionElegida->setMonto(montoNuevo);
        cout<<"Se ha actualizado el monto"<<endl;
        break;

    case 5:
        cout<<"Ingrese la nueva ubicacion ";
        cin>>ubicacionNueva;
        transaccionElegida->setUbicacion(ubicacionNueva);
        cout<<"Se ha actualizado la ubicacion"<<endl;
        break;              
    
    default:
        break;
    }
    detectorFraude.eliminarTransaccion(idBuscada);
    detectorFraude.anadirTransaccion(*transaccionElegida);
    actualizarArchivo(historial);
}
//OPCION 3.1
void montoMaximo(){
    double monto;
    
    do{
    cout<<"¿Cual es el monto ha superar para considerar una transaccion sospechosa? ";
    cin>>monto;
    if(monto<1){cout<<"El monto tiene que ser mayor a 0.";}
    }while(monto<1);
    DetectorDeFraude::setMontoEspecifico(monto);
    cout<<"-------------------------------------------------"<<endl;
    cout<<"Restricciones actualizadas."<<endl;

}
//OPCION 3.2
void cantidadMaximaTransacciones(){
    int cantTransacciones;
    int periodoTiempo;
    do{
    cout<<"¿Cual es la cantidad de transacciones en un cierto periodo de tiempo que se consideria sospechoso?: ";
    cin>>cantTransacciones;
    if(cantTransacciones<2){cout<<"El valor tiene que ser mayor a 1."<<endl;}
    }while(cantTransacciones<2);
    do{
    cout<<"¿De cuantas horas sería ese periodo de tiempo?: ";
    cin>>periodoTiempo;
    if(periodoTiempo<1){cout<<"El valor tiene que ser mayor a 0."<<endl;}
    }while(periodoTiempo<1);
    DetectorDeFraude::setcantTransaccionesPocoTiempo(cantTransacciones);
    DetectorDeFraude::setPeriodoTiempoTransacciones(periodoTiempo);
    cout<<"-------------------------------------------------"<<endl;
    cout<<"Restricciones actualizadas."<<endl;
}

//OPCION 3.3
void distintasUbicacionesEnPeriodosDeTiempo(){
    int ubicaciones;
    int periodoTiempo;  
    cout<<"¿Cuantas ubicaciones en un periodo de tiempo se consideraria sospechoso?: ";
    cin>>ubicaciones;
    cout<<"¿De cuantas horas sería ese periodo de tiempo?: ";
    cin>>periodoTiempo;
    DetectorDeFraude::setcantUbicacionesPocoTiempo(ubicaciones);
    DetectorDeFraude::setPeriodoTiempoUbicaciones(periodoTiempo);
    cout<<"-------------------------------------------------"<<endl;
    cout<<"Restricciones actualizadas."<<endl;
}
//OPCION 3
void agregarRestricciones(){
    int opcion=-1;
    cout<<"-------------------------------------------------"<<endl;
    cout<<"¿Que tipo de restriccion quiere agregar?"<<endl;
    cout<<"1.Monto maximo a depositar."<<endl;
    cout<<"2.Cantidad maxima de transacciones en un periodo de tiempo."<<endl;
    cout<<"3.Transacciones de ubicaciones distintas en un periodo de tiempo"<<endl;
    do{
    cout<<"-------------------------------------------------"<<endl;
    cout<<"Ingrese una opcion: ";
    cin>>opcion;
    cout<<"-------------------------------------------------"<<endl;
    if(opcion<1||opcion>3){
       
        cout<<"Ingrese una opcion valida."<<endl;
        
    }
    }while(opcion<1||opcion>3);
    switch (opcion)
    {
    case 1:
        montoMaximo();
        break;
    case 2:
        cantidadMaximaTransacciones();
        break;
    case 3:
        distintasUbicacionesEnPeriodosDeTiempo();
        break;
    default:
        break;
    }

    
}



//OPCION 4

void mostrarSospechas(const DetectorDeFraude& detectorFraude) {
    vector<Transaccion> sospechas = detectorFraude.getSospechas();
    
    
    if (sospechas.empty()) {
        cout << "No se han detectado transacciones sospechosas." << endl;
    } else {
        cout << "Transacciones sospechosas detectadas:" << endl;
        for (const auto& transaccion : sospechas) {
          
               cout<<"Razon sospecha:"<<transaccion.getRazonSospecha()<<" ";
               transaccion.printTransaccion();       
           
        }
    }
}

int main(int argc, char const *argv[])

{
    HistorialTransacciones historial;
    DetectorDeFraude detectorFraude;

    string nombreArchivo=("ArchivosTxt/Transacciones.txt");
    leerArchivoYAgregarTransacciones(nombreArchivo,historial,detectorFraude);
   

    //MENU
    int opcion=-1;
    do{
    cout<<"---------------------MENU-------------------"<<endl;
    cout<<"1.Ingresar una nueva transaccion"<<endl;
    cout<<"2.Modificar transaccion"<<endl;
    cout<<"3.Anadir restricciones"<<endl;
    cout<<"4.Mostrar lista de transacciones sospechosas"<<endl;
    cout<<"5.Mostrar historial de transacciones."<<endl;
    cout<<"0.Salir"<<endl;
    cout<<"-------------------------------------------------"<<endl;
    cout <<"Ingrese una opcion: ";  
    cin>> opcion;
    switch (opcion)
    {
    case 1:
        agregarTransacciones(historial,detectorFraude);
        break;
    case 2:
        modificarTransacciones(historial,detectorFraude);
        break;
    case 3:
        agregarRestricciones();
        break;
    case 4:
        mostrarSospechas(detectorFraude);
        break;
    case 5:
        historial.printTransacciones();
        break;
    case 0:
        cout<<"Saliendo..."<<endl;
        break;
    default:
        cout<<"-------------------------------------------------"<<endl;
        cout<<"Ingrese una opcion Valida"<<endl;
        break;
    }
    }while(opcion!=0);
    return 0;
}
