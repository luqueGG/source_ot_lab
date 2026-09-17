#include <iostream>
#include <string>
using namespace std;

// Se encarga de leer la cadena de texto y separar numeros y operadores
class Analizador {
private:
    string expresion;
    double numeros[6];
    char operadores[5];
    int cantNumeros;
    int cantOperadores;

public:
    Analizador(string exp) {
        expresion = exp;
        cantNumeros = 0;
        cantOperadores = 0;
    }

    // Recorre la cadena caracter por caracter y va armando los numeros
    void analizar() {
        string numeroActual = "";
        for (int i = 0; i < expresion.length(); i++) {
            char c = expresion[i];
            if (c == '+' || c == '-') {
                // Al encontrar un operador, cierra el numero actual
                numeros[cantNumeros] = stod(numeroActual);
                cantNumeros++;
                operadores[cantOperadores] = c;
                cantOperadores++;
                numeroActual = "";
            } else {
                numeroActual += c;
            }
        }
        // Guarda el ultimo numero que queda despues del ultimo operador
        numeros[cantNumeros] = stod(numeroActual);
        cantNumeros++;
    }

    double* getNumeros() {
        return numeros;
    }

    char* getOperadores() {
        return operadores;
    }

    int getCantNumeros() {
        return cantNumeros;
    }

    int getCantOperadores() {
        return cantOperadores;
    }
};

// Es el nucleo de la calculadora, hace las operaciones matematicas
class Calculadora {
public:
    // Recibe los numeros y operadores ya separados y calcula el resultado final
    double operar(double* numeros, char* operadores, int cantOperadores) {
        double resultado = numeros[0];
        for (int i = 0; i < cantOperadores; i++) {
            if (operadores[i] == '+') {
                resultado += numeros[i + 1];
            } else if (operadores[i] == '-') {
                resultado -= numeros[i + 1];
            }
        }
        return resultado;
    }
};

// Coordina el flujo: manda a analizar, valida y luego calcula
class Procesador {
private:
    Calculadora calculadora;

public:
    void procesar(string expresion) {
        Analizador analizador(expresion);
        analizador.analizar();

        // Validacion del limite de numeros que soporta el programa
        if (analizador.getCantNumeros() > 6) {
            cout << "Error: el programa solo acepta como maximo 6 numeros" << endl;
            return;
        }

        double resultado = calculadora.operar(analizador.getNumeros(), analizador.getOperadores(), analizador.getCantOperadores());
        cout << expresion << " = " << resultado << endl;
    }
};

int main() {
    Procesador procesador;

    // Pruebas con los casos del enunciado
    procesador.procesar("10+37");
    procesador.procesar("45+14-42");
    procesador.procesar("1+2+3+4+5+6");

    return 0;
}