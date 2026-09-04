#include "ejercicios.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
// Para numeros aleatorios
#include <ctime>
#include <cstdlib>


void ejercicio1()
{
    unsigned int n = 0; // numero entero positivo
    std::cout << "Ingrese un numero entero positivo: ";
    std::cin >> n;
    if (n < 1)
    {
        // Interrumpe el programa y lanza una excepcion de entrada invalida
        throw std::invalid_argument("Entrada invalida");
    }

    // vector de numeros
    std::vector<unsigned int> numeros {};

    char letra = ' ';
    std::cout << "Ingrese una letra: ";
    std::cin >> letra;

    switch (letra)
    {
        case 'g':
            // Usar el tiempo como semilla generadora
            // de numeros aleatorios
            srand((unsigned int) time(0)); // cast de time_t a uint
            // Almacenar los numeros generados
            for (size_t i = 0; i < n; i++)
            {
                numeros.push_back(rand() % n + 1);
            }
            break;

        case 'i':
            // El usuario ingresa valores
            while (n--)
            {
                unsigned int numTemporal;
                std::cin >> numTemporal;
                numeros.push_back(numTemporal);
            }
            break;

        default:
            // Interrumpe el programa
            throw std::invalid_argument("Entrada invalida");
    }
    
    // Ordenamiento y salida
    std::sort(numeros.begin(), numeros.end());
    for (const unsigned int& i : numeros)
    {
        std::cout << i << " ";
    }
    
}