#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Clase base
class Persona 
{
    protected:
        std::string nombre;
        int edad;

    public:
        Persona(std::string nombre, int edad)
        : nombre(std::move(nombre)), edad(edad) {}

        virtual ~Persona() = default;

        virtual void presentarse() const 
        {
            std::cout << "Soy " << nombre << ", tengo " << edad << " years.\n";
        }
};



class Arquitecto : public Persona 
{
    public:
        Arquitecto(std::string nombre, int edad) : Persona(std::move(nombre), edad) {}

        void presentarse() const override 
        {
            std::cout << "Soy " << nombre << ", tengo " << edad
                      << " years y trabajo como arquitecto/a diseñando edificios.\n";
        }
};

class Doctor : public Persona 
{
    public:
        Doctor(std::string nombre, int edad) : Persona(std::move(nombre), edad) {}

        void presentarse() const override 
        {
            std::cout << "Soy " << nombre << ", tengo " << edad
                  << " years y trabajo como doctor/a atendiendo pacientes.\n";
        }
};

class Enfermera : public Persona 
{
    public:
        Enfermera(std::string nombre, int edad) : Persona(std::move(nombre), edad) {}

        void presentarse() const override 
        {
            std::cout << "Soy " << nombre << ", tengo " << edad
                  << " years y trabajo como enfermero/a cuidando pacientes.\n";
        }
};


class Bombero : public Persona 
{
    public:
        Bombero(std::string nombre, int edad) : Persona(std::move(nombre), edad) {}

        void presentarse() const override {
            std::cout << "Soy " << nombre << ", tengo " << edad
                  << " years y trabajo como bombero/a apagando incendios.\n";
        }
};



/*
 * hice RAII
 * cada objeto se crea con make_unique para prevenir duplicados
 * cada objeto se elimina de forma automatica, como el destructor de Persona
 * es VIRTUAl, significa que un puntero Persona accedera a un objeto derivado
 * y llamar al destructor.
 * */
int main() 
{
    std::vector<std::unique_ptr<Persona>> personas;

    personas.push_back(std::make_unique<Arquitecto>("Luque", 34));
    personas.push_back(std::make_unique<Doctor>("Yandel", 41));
    personas.push_back(std::make_unique<Bombero>("Diego", 37));
    personas.push_back(std::make_unique<Enfermera>("Marvin", 29));

    for (const auto& p : personas) 
    {
        p->presentarse();     
    }

    return 0;
}
