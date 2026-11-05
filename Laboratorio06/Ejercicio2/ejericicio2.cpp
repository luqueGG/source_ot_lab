
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <cmath>
#include <iomanip>

using namespace std;

// Representa cada registro del CSV
struct Punto {
    string origen;
    string destino;
    double x1, y1;
    double x2, y2;
};

// Lee los datos del archivo CSV
vector<Punto> leerCSV(const string& nombreArchivo) {
    vector<Punto> puntos;
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo CSV.\n";
        return puntos;
    }

    string linea;

    // Saltar la primera linea (encabezados)
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        vector<string> campos;

        while (getline(ss, campo, ',')) {
            campos.push_back(campo);
        }

        if (campos.size() >= 6) {
            Punto p;

            p.origen = campos[0];
            p.destino = campos[1];
            p.x1 = stod(campos[2]);
            p.y1 = stod(campos[3]);
            p.x2 = stod(campos[4]);
            p.y2 = stod(campos[5]);

            puntos.push_back(p);
        }
    }

    archivo.close();
    return puntos;
}

// Calcula las distancias Manhattan
void calcularManhattan(
    const vector<Punto>& puntos,
    vector<double>& resultados
) {
    for (size_t i = 0; i < puntos.size(); i++) {
        double dx = abs(puntos[i].x1 - puntos[i].x2);
        double dy = abs(puntos[i].y1 - puntos[i].y2);

        resultados[i] = dx + dy;
    }
}

// Calcula las distancias Euclidianas
void calcularEuclidiana(
    const vector<Punto>& puntos,
    vector<double>& resultados
) {
    for (size_t i = 0; i < puntos.size(); i++) {
        double dx = puntos[i].x1 - puntos[i].x2;
        double dy = puntos[i].y1 - puntos[i].y2;

        resultados[i] = sqrt(dx * dx + dy * dy);
    }
}

int main() {

    // 1. Leer el archivo CSV
    vector<Punto> puntos = leerCSV("distancias_manhattan_euclides.csv");

    if (puntos.empty()) {
        cout << "No hay datos para procesar.\n";
        return 1;
    }

    // 2. Crear vectores para almacenar los resultados
    vector<double> manhattan(puntos.size());
    vector<double> euclidiana(puntos.size());

    cout << "Iniciando calculos concurrentes...\n\n";

    // 3. Crear los dos Threads
    thread t1(calcularManhattan, cref(puntos), ref(manhattan));
    thread t2(calcularEuclidiana, cref(puntos), ref(euclidiana));

    // 4. Esperar a que ambos Threads terminen
    t1.join();
    t2.join();

    // 5. Mostrar los primeros 5 resultados
    cout << fixed << setprecision(4);

    cout << "\n===== RESULTADOS DE LAS DISTANCIAS =====\n";

    cout << left
         << setw(12) << "Origen"
         << setw(12) << "Destino"
         << setw(18) << "Manhattan"
         << setw(18) << "Euclidiana"
         << "\n";

    cout << string(60, '-') << "\n";

    size_t limite = min(puntos.size(), size_t(5));

    for (size_t i = 0; i < limite; i++) {
        cout << left
             << setw(12) << puntos[i].origen
             << setw(12) << puntos[i].destino
             << setw(18) << manhattan[i]
             << setw(18) << euclidiana[i]
             << "\n";
    }

    cout << "\nProcesamiento finalizado.\n";

    return 0;
}