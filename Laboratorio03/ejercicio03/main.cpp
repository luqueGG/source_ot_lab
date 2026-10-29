#include <iostream>
using namespace std;

// Nodo del árbol
class Nodo {
public:
    char operador;
    int valor;

    // Punteros a los hijos
    Nodo* izquierda;
    Nodo* derecha;

    // Crea un nodo con un número
    Nodo(int valor) {
        this->valor = valor;
        operador = '\0';
        izquierda = nullptr;
        derecha = nullptr;
    }

    // Crea un nodo con un operador
    Nodo(char operador, Nodo* izquierda, Nodo* derecha) {
        this->operador = operador;
        this->izquierda = izquierda;
        this->derecha = derecha;
        valor = 0;
    }
};

// Calcula el resultado del árbol
int evaluar(Nodo* raiz) {
    if (raiz->operador == '\0')
        return raiz->valor;

    int izq = evaluar(raiz->izquierda);
    int der = evaluar(raiz->derecha);

    if (raiz->operador == '+')
        return izq + der;

    if (raiz->operador == '*')
        return izq * der;

    return 0;
}

// Libera la memoria del árbol
void eliminarArbol(Nodo* raiz) {
    if (raiz == nullptr)
        return;

    eliminarArbol(raiz->izquierda);
    eliminarArbol(raiz->derecha);

    delete raiz;
}

int main() {
    // Representa (3 + 5) * 2
    Nodo* raiz = new Nodo('*',
        new Nodo('+',
            new Nodo(3),
            new Nodo(5)),
        new Nodo(2));

    cout << "Resultado: " << evaluar(raiz) << endl;

    eliminarArbol(raiz);

    return 0;
}