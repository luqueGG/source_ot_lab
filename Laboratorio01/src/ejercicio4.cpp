#include <iostream>
#include <string>
using namespace std;

struct CitaMedica {
    string fecha, hora, tipoAtencion;
    int numeroAsegurado;

    // Constructor
    CitaMedica(string f, string h, string t, int n) {
        fecha = f;
        hora = h;
        tipoAtencion = t;
        numeroAsegurado = n;
    }

    // Método 1: mostrar los datos
    void mostrar() {
        cout << "\nDatos de la cita" << endl;
        cout << "Fecha: " << fecha << endl;
        cout << "Hora: " << hora << endl;
        cout << "Atencion: " << tipoAtencion << endl;
        cout << "Numero de asegurado: " << numeroAsegurado << endl;
    }

    // Método 2: verificar si es una emergencia
    bool esUrgencia() {
        return tipoAtencion == "Emergencia";
    }

    // Método 3: cambiar el tipo de atención
    void cambiarAtencion(string nuevoTipo) {
        tipoAtencion = nuevoTipo;
    }

    // Método 4: cambiar la hora de la cita
    void cambiarHora(string nuevaHora) {
        hora = nuevaHora;
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
    getline(cin, t);

    cout << "Ingrese numero de asegurado: ";
    cin >> n;

    // Crear objeto
    CitaMedica c1(f, h, t, n);

    // Usar los métodos
    c1.mostrar();

    if (c1.esUrgencia())
        cout << "Es una urgencia, prioridad alta." << endl;
    else
        cout << "Cita normal." << endl;

    // Ejemplo de uso del método cambiarHora
    c1.cambiarHora("15:30");

    cout << "\nDespues de modificar la hora:" << endl;
    c1.mostrar();
}
