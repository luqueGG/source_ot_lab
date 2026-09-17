#include <iostream>

using namespace std;

// Estructura para cada nodo de la lista
struct Nodo {
    int dato;
    Nodo* siguiente;
    Nodo* anterior;

    // Constructor del nodo
    Nodo(int val) : dato(val), siguiente(nullptr), anterior(nullptr) {}
};

// Clase para manejar la lista doblemente enlazada
class ListaDoble {
private:
    Nodo* cabeza; // Puntero al primer nodo
    Nodo* cola;   // Puntero al último nodo

public:
    // Constructor de la lista
    ListaDoble() : cabeza(nullptr), cola(nullptr) {}

    // Destructor para liberar memoria y evitar fugas (memory leaks)
    ~ListaDoble() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            Nodo* siguienteNodo = actual->siguiente;
            delete actual;
            actual = siguienteNodo;
        }
    }

    // Insertar un elemento al final de la lista
    void insertarAlFinal(int val) {
        Nodo* nuevo = new Nodo(val);
        if (cabeza == nullptr) { // Si la lista está vacía
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }

    // Mostrar la lista desde el principio hasta el final
    void mostrarAdelante() {
        Nodo* actual = cabeza;
        cout << "Lista (adelante): ";
        while (actual != nullptr) {
            cout << actual->dato << " <-> ";
            actual = actual->siguiente;
        }
        cout << "NULL" << endl;
    }

    // Mostrar la lista desde el final hasta el principio
    void mostrarAtras() {
        Nodo* actual = cola;
        cout << "Lista (atras): ";
        while (actual != nullptr) {
            cout << actual->dato << " <-> ";
            actual = actual->anterior;
        }
        cout << "NULL" << endl;
    }
};

int main() {
    ListaDoble miLista;

    // Insertando elementos
    miLista.insertarAlFinal(10);
    miLista.insertarAlFinal(20);
    miLista.insertarAlFinal(30);

    // Probando recorridos
    miLista.mostrarAdelante(); // Salida: 10 <-> 20 <-> 30 <-> NULL
    miLista.mostrarAtras();    // Salida: 30 <-> 20 <-> 10 <-> NULL

    return 0;
}
