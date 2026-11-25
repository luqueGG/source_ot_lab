#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iomanip>

enum class Sector { Trepadoras, ArbolesNormales, Ornamentales, Bonsais };

std::string nombreSector(Sector s) {
    switch (s) {
        case Sector::Trepadoras: return "Trepadoras";
        case Sector::ArbolesNormales: return "Arboles normales";
        case Sector::Ornamentales: return "Ornamentales";
        case Sector::Bonsais: return "Bonsais";
    }
    return "";
}

class Planta {
protected:
    std::string nombre;
    std::string especie;
    double altura;
    double precio;
    std::string riego;
    Sector sector;
public:
    Planta(std::string n, std::string e, double a, double p, std::string r, Sector s)
        : nombre(std::move(n)), especie(std::move(e)), altura(a), precio(p), riego(std::move(r)), sector(s) {}
    virtual ~Planta() = default;
    virtual std::unique_ptr<Planta> clonar() const = 0;
    virtual void mostrar() const {
        std::cout << std::left << std::setw(18) << nombre
                  << std::setw(24) << especie
                  << std::right << std::setw(7) << std::fixed << std::setprecision(1) << altura << " cm"
                  << std::setw(9) << std::setprecision(2) << precio << " S/"
                  << "  riego: " << riego;
    }
    void setNombre(const std::string& n) { nombre = n; }
    void setEspecie(const std::string& e) { especie = e; }
    void setAltura(double a) { altura = a; }
    void setPrecio(double p) { precio = p; }
    void setRiego(const std::string& r) { riego = r; }
    Sector getSector() const { return sector; }
    const std::string& getNombre() const { return nombre; }
};

class Trepadora : public Planta {
    std::string soporte;
    double longitudMax;
public:
    Trepadora(std::string n, std::string e, double a, double p, std::string r, std::string sop, double lm)
        : Planta(std::move(n), std::move(e), a, p, std::move(r), Sector::Trepadoras), soporte(std::move(sop)), longitudMax(lm) {}
    std::unique_ptr<Planta> clonar() const override { return std::make_unique<Trepadora>(*this); }
    void setSoporte(const std::string& s) { soporte = s; }
    void setLongitudMax(double l) { longitudMax = l; }
    void mostrar() const override {
        Planta::mostrar();
        std::cout << "  soporte: " << soporte << "  long.max: " << std::setprecision(0) << longitudMax << " cm\n";
    }
};

class ArbolNormal : public Planta {
    double alturaMaxima;
    std::string tipoCopa;
    bool frutal;
public:
    ArbolNormal(std::string n, std::string e, double a, double p, std::string r, double am, std::string tc, bool f)
        : Planta(std::move(n), std::move(e), a, p, std::move(r), Sector::ArbolesNormales), alturaMaxima(am), tipoCopa(std::move(tc)), frutal(f) {}
    std::unique_ptr<Planta> clonar() const override { return std::make_unique<ArbolNormal>(*this); }
    void setAlturaMaxima(double a) { alturaMaxima = a; }
    void setTipoCopa(const std::string& t) { tipoCopa = t; }
    void setFrutal(bool f) { frutal = f; }
    void mostrar() const override {
        Planta::mostrar();
        std::cout << "  alt.max: " << std::setprecision(0) << alturaMaxima << " cm  copa: " << tipoCopa
                  << "  frutal: " << (frutal ? "si" : "no") << "\n";
    }
};

class Ornamental : public Planta {
    std::string colorFlor;
    std::string temporada;
    bool interior;
public:
    Ornamental(std::string n, std::string e, double a, double p, std::string r, std::string cf, std::string t, bool i)
        : Planta(std::move(n), std::move(e), a, p, std::move(r), Sector::Ornamentales), colorFlor(std::move(cf)), temporada(std::move(t)), interior(i) {}
    std::unique_ptr<Planta> clonar() const override { return std::make_unique<Ornamental>(*this); }
    void setColorFlor(const std::string& c) { colorFlor = c; }
    void setTemporada(const std::string& t) { temporada = t; }
    void setInterior(bool i) { interior = i; }
    void mostrar() const override {
        Planta::mostrar();
        std::cout << "  flor: " << colorFlor << "  temporada: " << temporada
                  << "  interior: " << (interior ? "si" : "no") << "\n";
    }
};

class Bonsai : public Planta {
    std::string estilo;
    int edadAnios;
    std::string maceta;
public:
    Bonsai(std::string n, std::string e, double a, double p, std::string r, std::string es, int ed, std::string m)
        : Planta(std::move(n), std::move(e), a, p, std::move(r), Sector::Bonsais), estilo(std::move(es)), edadAnios(ed), maceta(std::move(m)) {}
    std::unique_ptr<Planta> clonar() const override { return std::make_unique<Bonsai>(*this); }
    void setEstilo(const std::string& e) { estilo = e; }
    void setEdadAnios(int e) { edadAnios = e; }
    void setMaceta(const std::string& m) { maceta = m; }
    void mostrar() const override {
        Planta::mostrar();
        std::cout << "  estilo: " << estilo << "  edad: " << edadAnios << " anios  maceta: " << maceta << "\n";
    }
};

class CatalogoPrototipos {
    std::map<std::string, std::unique_ptr<Planta>> prototipos;
public:
    void registrar(const std::string& clave, std::unique_ptr<Planta> p) { prototipos[clave] = std::move(p); }
    std::unique_ptr<Planta> crear(const std::string& clave) const {
        auto it = prototipos.find(clave);
        if (it == prototipos.end()) throw std::runtime_error("Prototipo no registrado: " + clave);
        return it->second->clonar();
    }
    std::vector<std::string> claves() const {
        std::vector<std::string> v;
        for (const auto& par : prototipos) v.push_back(par.first);
        return v;
    }
};

class Vivero {
    std::map<Sector, std::vector<std::unique_ptr<Planta>>> sectores;
public:
    Vivero() {
        sectores[Sector::Trepadoras];
        sectores[Sector::ArbolesNormales];
        sectores[Sector::Ornamentales];
        sectores[Sector::Bonsais];
    }
    void agregar(std::unique_ptr<Planta> p) {
        Sector s = p->getSector();
        sectores[s].push_back(std::move(p));
    }
    void mostrar() const {
        for (const auto& par : sectores) {
            std::cout << "\n== Sector: " << nombreSector(par.first) << " (" << par.second.size() << ") ==\n";
            for (const auto& p : par.second) { std::cout << "  "; p->mostrar(); }
        }
    }
};

int main() {
    CatalogoPrototipos catalogo;
    catalogo.registrar("trepadora", std::make_unique<Trepadora>("Jazmin", "Jasminum officinale", 80, 25.0, "moderado", "enrejado", 400));
    catalogo.registrar("arbol", std::make_unique<ArbolNormal>("Molle", "Schinus molle", 150, 60.0, "bajo", 900, "redondeada", false));
    catalogo.registrar("ornamental", std::make_unique<Ornamental>("Rosa", "Rosa chinensis", 50, 15.0, "frecuente", "roja", "primavera", false));
    catalogo.registrar("bonsai", std::make_unique<Bonsai>("Ficus bonsai", "Ficus retusa", 35, 180.0, "moderado", "informal vertical", 5, "ceramica"));

    Vivero vivero;

    vivero.agregar(catalogo.crear("trepadora"));

    auto t2 = catalogo.crear("trepadora");
    t2->setNombre("Buganvilia");
    t2->setEspecie("Bougainvillea spectabilis");
    t2->setPrecio(30.0);
    t2->setAltura(100);
    if (auto* t = dynamic_cast<Trepadora*>(t2.get())) { t->setSoporte("pergola"); t->setLongitudMax(600); }
    vivero.agregar(std::move(t2));

    auto a1 = catalogo.crear("arbol");
    a1->setNombre("Palto");
    a1->setEspecie("Persea americana");
    a1->setPrecio(80.0);
    if (auto* a = dynamic_cast<ArbolNormal*>(a1.get())) { a->setFrutal(true); a->setAlturaMaxima(1000); }
    vivero.agregar(std::move(a1));

    auto o1 = catalogo.crear("ornamental");
    o1->setNombre("Geranio");
    o1->setEspecie("Pelargonium hortorum");
    o1->setPrecio(8.0);
    if (auto* o = dynamic_cast<Ornamental*>(o1.get())) { o->setColorFlor("rosada"); o->setTemporada("todo el anio"); }
    vivero.agregar(std::move(o1));
    vivero.agregar(catalogo.crear("ornamental"));

    auto b1 = catalogo.crear("bonsai");
    b1->setNombre("Olmo chino bonsai");
    b1->setEspecie("Ulmus parvifolia");
    b1->setPrecio(250.0);
    if (auto* b = dynamic_cast<Bonsai*>(b1.get())) { b->setEstilo("escoba"); b->setEdadAnios(8); }
    vivero.agregar(std::move(b1));
    vivero.agregar(catalogo.crear("bonsai"));

    vivero.mostrar();

    try { catalogo.crear("cactus"); }
    catch (const std::exception& e) { std::cout << "\nError: " << e.what() << "\n"; }
    return 0;
}