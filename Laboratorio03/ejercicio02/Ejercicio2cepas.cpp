#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Primera clase base: Características de la Cepa Delta
class CepaDelta 
{
    protected:
        bool altaPatogenicidad;

    public:
        CepaDelta(bool patogenicidad) : altaPatogenicidad(patogenicidad) {}
        
        virtual ~CepaDelta() = default;

        void infoDelta() const 
        {
            std::cout << "  - Linaje Delta: Alta severidad clínica y patogenicidad.\n";
        }
};

// Segunda clase base: Características de la Cepa Omicron
class CepaOmicron 
{
    protected:
        bool altaTransmisibilidad;

    public:
        CepaOmicron(bool transmisibilidad) : altaTransmisibilidad(transmisibilidad) {}
        
        virtual ~CepaOmicron() = default;

        void infoOmicron() const 
        {
            std::cout << "  - Linaje Omicron: Alta capacidad de evasión inmune y transmisibilidad.\n";
        }
};

// Clase derivada por HERENCIA MÚLTIPLE
class CepaHibrida : public CepaDelta, public CepaOmicron 
{
    private:
        std::string nombreVariante;

    public:
        // Constructor que inicializa ambas clases base
        CepaHibrida(std::string nombre, bool patogenicidad, bool transmisibilidad)
            : CepaDelta(patogenicidad), CepaOmicron(transmisibilidad), nombreVariante(std::move(nombre)) {}

        void analizarCepa() const 
        {
            std::cout << "Analizando variante hibrida: " << nombreVariante << "\n";
            infoDelta();
            infoOmicron();
            std::cout << "--------------------------------------------------\n";
        }
};

int main() 
{
    // Creación de un vector de cepas híbridas usando punteros inteligentes
    std::vector<std::unique_ptr<CepaHibrida>> variantes;

    variantes.push_back(std::make_unique<CepaHibrida>("XE (Combinación Delta/Omicron)", true, true));
    variantes.push_back(std::make_unique<CepaHibrida>("XAY", true, true));

    for (const auto& variante : variantes) 
    {
        variante->analizarCepa();
    }

    return 0;
}
