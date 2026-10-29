#include <iostream>
#include <string>
using namespace std;

template <class T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

template <class T>
class ListaEnlazada {
    Nodo<T>* cabeza;
    int tamano;
public:
    ListaEnlazada() : cabeza(nullptr), tamano(0) {}
    ListaEnlazada(const ListaEnlazada&) = delete;
    ListaEnlazada& operator=(const ListaEnlazada&) = delete;
    ~ListaEnlazada();
    void insertarInicio(T valor);
    void insertarFinal(T valor);
    bool eliminar(T valor);
    bool buscar(T valor) const;
    int obtenerTamano() const { return tamano; }
    void mostrar() const;
};

template <class T>
ListaEnlazada<T>::~ListaEnlazada() {
    while (cabeza != nullptr) {
        Nodo<T>* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

template <class T>
void ListaEnlazada<T>::insertarInicio(T valor) {
    Nodo<T>* nuevo = new Nodo<T>(valor);
    nuevo->siguiente = cabeza;
    cabeza = nuevo;
    tamano++;
}

template <class T>
void ListaEnlazada<T>::insertarFinal(T valor) {
    Nodo<T>* nuevo = new Nodo<T>(valor);
    if (cabeza == nullptr) {
        cabeza = nuevo;
    } else {
        Nodo<T>* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    tamano++;
}

template <class T>
bool ListaEnlazada<T>::eliminar(T valor) {
    Nodo<T>* actual = cabeza;
    Nodo<T>* anterior = nullptr;
    while (actual != nullptr && actual->dato != valor) {
        anterior = actual;
        actual = actual->siguiente;
    }
    if (actual == nullptr) return false;
    if (anterior == nullptr) cabeza = actual->siguiente;
    else anterior->siguiente = actual->siguiente;
    delete actual;
    tamano--;
    return true;
}

template <class T>
bool ListaEnlazada<T>::buscar(T valor) const {
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato == valor) return true;
        actual = actual->siguiente;
    }
    return false;
}

template <class T>
void ListaEnlazada<T>::mostrar() const {
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        cout << actual->dato << " -> ";
        actual = actual->siguiente;
    }
    cout << "NULL" << endl;
}

int main() {
    ListaEnlazada<int> edades;
    edades.insertarFinal(18);
    edades.insertarFinal(25);
    edades.insertarFinal(21);
    edades.insertarInicio(30);
    edades.mostrar();
    edades.eliminar(25);
    edades.mostrar();
    cout << "Tamano: " << edades.obtenerTamano() << endl;
    cout << "Existe 21: " << (edades.buscar(21) ? "si" : "no") << endl;

    ListaEnlazada<string> palabras;
    palabras.insertarFinal("sistemas");
    palabras.insertarFinal("objetos");
    palabras.insertarFinal("plantillas");
    palabras.mostrar();
    palabras.eliminar("objetos");
    palabras.mostrar();
    cout << "Tamano: " << palabras.obtenerTamano() << endl;

    return 0;
}