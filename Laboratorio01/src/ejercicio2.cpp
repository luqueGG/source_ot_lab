
#include "ejercicios.hpp"

#include <iostream>
#include <iomanip>


void ejercicio2()
{
    std::cout << "\nEjercicio 2\n";

    std::cout << std::left
              << std::setw(15) << "Nombre"
              << std::setw(8)  << "Edad"
              << std::setw(12) << "Puntos"
              << '\n';

    std::cout << std::setw(15) << "S. Ohtani"
              << std::setw(8)  << 32
              << std::setw(12) << "1.045/9.8"
              << '\n';

    std::cout << std::setw(15) << "A. Judge"
              << std::setw(8)  << 34
              << std::setw(12) << "1.145/10.1"
              << '\n';
}

