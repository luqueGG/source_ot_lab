#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class Equipo {
public:
    string marca;
    string color;
    string cpu;
    int cores = 0;
    string disco;
    string monitor;
    string ram;
    string impresora;
    vector<string> otros;
    double precio = 0;

    void Mostrar() const {
        cout << "Marca: " << marca << endl;
        cout << "Color: " << color << endl;
        cout << "CPU: " << cpu << " (" << cores << " cores)" << endl;
        cout << "Disco duro: " << disco << endl;
        cout << "Monitor: " << monitor << endl;
        cout << "RAM: " << ram << endl;
        if (!impresora.empty()) cout << "Impresora: " << impresora << endl;
        if (!otros.empty()) {
            cout << "Otros: ";
            for (size_t i = 0; i < otros.size(); i++) {
                cout << otros[i];
                if (i + 1 < otros.size()) cout << ", ";
            }
            cout << endl;
        }
        cout << fixed << setprecision(2);
        cout << "Precio total: $" << precio << endl;
    }
};

class IBuilder {
public:
    virtual ~IBuilder() {}
    virtual void Reset() = 0;
    virtual void SetMarca(const string& m) = 0;
    virtual void SetColor(const string& c) = 0;
    virtual void SetCPU(const string& cpu, int cores) = 0;
    virtual void SetDisco(const string& d) = 0;
    virtual void SetMonitor(const string& m) = 0;
    virtual void SetRAM(const string& r) = 0;
    virtual void SetImpresora(const string& i) = 0;
    virtual void AddOtro(const string& o) = 0;
};

class LaptopBuilder : public IBuilder {
private:
    Equipo* equipo;
    void Sumar(double p) { equipo->precio += p; }
public:
    LaptopBuilder() : equipo(nullptr) { Reset(); }
    ~LaptopBuilder() { delete equipo; }

    void Reset() override {
        delete equipo;
        equipo = new Equipo();
        Sumar(300);
    }
    void SetMarca(const string& m) override {
        equipo->marca = m;
        if (m == "Dell") Sumar(80);
        else if (m == "HP") Sumar(60);
        else if (m == "Lenovo") Sumar(50);
        else if (m == "Asus") Sumar(70);
    }
    void SetColor(const string& c) override {
        equipo->color = c;
        if (c != "Negro" && c != "Gris") Sumar(20);
    }
    void SetCPU(const string& cpu, int cores) override {
        equipo->cpu = cpu;
        equipo->cores = cores;
        Sumar(cores * 45);
    }
    void SetDisco(const string& d) override {
        equipo->disco = d;
        if (d == "SSD 256GB") Sumar(50);
        else if (d == "SSD 512GB") Sumar(90);
        else if (d == "SSD 1TB") Sumar(150);
        else if (d == "HDD 1TB") Sumar(55);
    }
    void SetMonitor(const string& m) override {
        equipo->monitor = m;
        if (m == "LED 14") Sumar(70);
        else if (m == "LED 15.6") Sumar(90);
        else if (m == "Plasma 17") Sumar(220);
    }
    void SetRAM(const string& r) override {
        equipo->ram = r;
        if (r == "8GB") Sumar(40);
        else if (r == "16GB") Sumar(80);
        else if (r == "32GB") Sumar(160);
    }
    void SetImpresora(const string& i) override {
        equipo->impresora = i;
        if (i == "Inyeccion de tinta") Sumar(90);
        else if (i == "Laser") Sumar(140);
        else if (i == "Multifuncional") Sumar(180);
    }
    void AddOtro(const string& o) override {
        equipo->otros.push_back(o);
        if (o == "Mouse") Sumar(10);
        else if (o == "Mochila") Sumar(25);
        else if (o == "Webcam HD") Sumar(30);
        else if (o == "Garantia extendida") Sumar(75);
    }
    Equipo* GetEquipo() {
        Equipo* r = equipo;
        equipo = nullptr;
        Reset();
        return r;
    }
};

class Director {
private:
    IBuilder* builder;
public:
    Director() : builder(nullptr) {}
    void SetBuilder(IBuilder* b) { builder = b; }

    void ConstruirBasico() {
        builder->SetMarca("Lenovo");
        builder->SetColor("Gris");
        builder->SetCPU("Intel Core i3", 2);
        builder->SetDisco("HDD 1TB");
        builder->SetMonitor("LED 14");
        builder->SetRAM("8GB");
    }
    void ConstruirOficina() {
        builder->SetMarca("HP");
        builder->SetColor("Negro");
        builder->SetCPU("AMD Ryzen 5", 4);
        builder->SetDisco("SSD 512GB");
        builder->SetMonitor("LED 15.6");
        builder->SetRAM("16GB");
        builder->SetImpresora("Laser");
        builder->AddOtro("Mouse");
        builder->AddOtro("Mochila");
    }
    void ConstruirPremium() {
        builder->SetMarca("Dell");
        builder->SetColor("Plateado");
        builder->SetCPU("Intel Core i9", 8);
        builder->SetDisco("SSD 1TB");
        builder->SetMonitor("Plasma 17");
        builder->SetRAM("32GB");
        builder->SetImpresora("Multifuncional");
        builder->AddOtro("Mouse");
        builder->AddOtro("Webcam HD");
        builder->AddOtro("Garantia extendida");
    }
};

void ClienteCode(Director& director) {
    LaptopBuilder* builder = new LaptopBuilder();
    director.SetBuilder(builder);

    cout << "===== EQUIPO BASICO =====" << endl;
    director.ConstruirBasico();
    Equipo* e = builder->GetEquipo();
    e->Mostrar();
    delete e;

    cout << endl << "===== EQUIPO OFICINA =====" << endl;
    director.ConstruirOficina();
    e = builder->GetEquipo();
    e->Mostrar();
    delete e;

    cout << endl << "===== EQUIPO PREMIUM =====" << endl;
    director.ConstruirPremium();
    e = builder->GetEquipo();
    e->Mostrar();
    delete e;

    cout << endl << "===== EQUIPO PERSONALIZADO =====" << endl;
    builder->SetMarca("Asus");
    builder->SetColor("Rojo");
    builder->SetCPU("AMD Ryzen 7", 6);
    builder->SetDisco("SSD 512GB");
    builder->SetMonitor("Plasma 17");
    builder->SetRAM("16GB");
    builder->AddOtro("Webcam HD");
    e = builder->GetEquipo();
    e->Mostrar();
    delete e;

    delete builder;
}

int main() {
    Director* director = new Director();
    ClienteCode(*director);
    delete director;
    return 0;
}