
#include "ejercicios.hpp"
#include <iostream>
#include <exception>


/*
main xd
*/
int main (void)
{
    try
    {
        // Coloquen su ejercicio en forma de metodo void
        ejercicio1();
    } 
    catch (const std::exception& e)
    {
        std::cout << "Error -> " << e.what() << '\n';
    }
}