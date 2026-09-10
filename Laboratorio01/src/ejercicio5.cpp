#include "ejercicios.hpp"
#include <string>
#include <iostream>
using namespace std;

class CitaMedica {
private:
    string fecha, hora, tipoAtencion;
    int numeroAsegurado;

public:
    CitaMedica(string f, string h, string t, int n) { 
        // se ejecuta solo al crear el objeto, ya no hace falta registrar()
        fecha = f;
        hora = h;
        tipoAtencion = t;
        numeroAsegurado = n;
    }

    void mostrar() { 
        // imprime los datos de la cita ya cargados
        cout << "\n Datos de la cita" << endl;
        cout << "Fecha: " << fecha << " | Hora: " << hora
             << "Atencion: " << tipoAtencion
             << "Asegurado: " << numeroAsegurado << endl;
    }

    bool esUrgencia() { 
        // chequea si el tipo de atencion es emergencia
        return tipoAtencion == "Emergencia";
    }
};

void ejercicio5() {
    string f, h, t;
    int n;

    cout << "Ingrese fecha (dd/mm/aaaa): ";
    cin >> f;

    cout << "Ingrese hora (hh:mm): ";
    cin >> h;

    cout << "Ingrese tipo de atencion (ejm. Consulta general / Emergencia): ";
    cin.ignore(); 
    // limpio el buffer para que getline no se coma el enter pendiente
    getline(cin, t);

    cout << "Ingrese numero de asegurado: ";
    cin >> n;

    CitaMedica c1(f, h, t, n); 
    // el objeto ya nace con los datos cargados, gracias al constructor

    c1.mostrar();

    if (c1.esUrgencia())
        cout << "Es una urgencia, prioridad alta." << endl;
    else
        cout << "Cita normal." << endl;

}