#include <iostream>

// Clase Nodo para el árbol genérico
template <typename T>
class NodoArbol {
public:
    T valor;
    NodoArbol* izquierdo;
    NodoArbol* derecho;

    NodoArbol(T val) : valor(val), izquierdo(nullptr), derecho(nullptr) {}
};

// Clase Plantilla para la estructura tipo árbol
template <typename T>
class ArbolBinario {
private:
    NodoArbol<T>* raiz;

    void insertarRecursivo(NodoArbol<T>*& nodo, T val) {
        if (nodo == nullptr) {
            nodo = new NodoArbol<T>(val);
        } else if (val < nodo->valor) {
            insertarRecursivo(nodo->izquierdo, val);
        } else if (val > nodo->valor) {
            insertarRecursivo(nodo->derecho, val);
        }
    }

    void inOrdenRecursivo(NodoArbol<T>* nodo) {
        if (nodo != nullptr) {
            inOrdenRecursivo(nodo->izquierdo);
            std::cout << nodo->valor << " ";
            inOrdenRecursivo(nodo->derecho);
        }
    }

public:
    ArbolBinario() : raiz(nullptr) {}

    void insertar(T val) {
        insertarRecursivo(raiz, val);
    }

    void mostrarInOrden() {
        inOrdenRecursivo(raiz);
        std::cout << std::endl;
    }
};

int main() {
    // 1. Árbol de enteros
    std::cout << "--- Arbol de Enteros ---" << std::endl;
    ArbolBinario<int> arbolInt;
    arbolInt.insertar(40);
    arbolInt.insertar(20);
    arbolInt.insertar(60);
    arbolInt.mostrarInOrden(); // Salida: 20 40 60

    // 2. Árbol de flotantes
    std::cout << "\n--- Arbol de Flotantes ---" << std::endl;
    ArbolBinario<double> arbolDouble;
    arbolDouble.insertar(5.5);
    arbolDouble.insertar(2.1);
    arbolDouble.insertar(8.9);
    arbolDouble.mostrarInOrden(); // Salida: 2.1 5.5 8.9

    // 3. Árbol de caracteres
    std::cout << "\n--- Arbol de Caracteres ---" << std::endl;
    ArbolBinario<char> arbolChar;
    arbolChar.insertar('M');
    arbolChar.insertar('A');
    arbolChar.insertar('Z');
    arbolChar.mostrarInOrden(); // Salida: A M Z

    return 0;
}
