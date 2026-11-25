#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <stdexcept>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <optional>
#include <algorithm>

using namespace std;

using Dinero = long long; // centavos
using Fecha = chrono::sys_days;

Fecha fecha(int a, unsigned m, unsigned d) {
    chrono::year_month_day f{chrono::year{a}, chrono::month{m}, chrono::day{d}};
    if (!f.ok()) throw invalid_argument("Fecha invalida");
    return Fecha{f};
}

string texto(Fecha f) {
    auto x = chrono::year_month_day{f};
    return to_string(int(x.year())) + "-" + to_string(unsigned(x.month())) + "-" + to_string(unsigned(x.day()));
}

void monto(Dinero d) { cout << "S/ " << fixed << setprecision(2) << d / 100.0; }


struct Movimiento {
    Fecha fecha;
    string concepto;
    Dinero saldo; // variacion del saldo disponible
    Dinero deuda; // variacion de la deuda
};


struct Cuenta {
    int id;
    string tipo;
    Fecha apertura;
    set<string> titulares;
    vector<Movimiento> movimientos;
    Dinero saldo(Fecha corte) const {
        Dinero s = 0; for (const auto& m : movimientos) if (m.fecha <= corte) s += m.saldo; return s;
    }
    Dinero deuda(Fecha corte) const {
        Dinero s = 0; for (const auto& m : movimientos) if (m.fecha <= corte) s += m.deuda; return s;
    }
};


struct Tarifa {
    double minimo, maximo; // porcentajes por operacion
    bool generaDeuda;
    set<string> tiposPermitidos; // vacio: todos
};


class RepositorioCuentas {

    friend class AccesoCliente;
    
    map<int, Cuenta> cuentas;
    
    set<string> clientes;
    set<string> tipos{"Ahorro", "Joven", "Pluss", "Empresarial", "Credito"};
    
    map<string, Tarifa> tarifas{
        {"Prestamo personal", {8, 10, true, {}}},
        {"Transferencia nacional", {0.5, 4, false, {}}},
        {"Compra con credito", {15, 25, true, {"Credito"}}}
    };
    
    int siguiente = 1;
    
    RepositorioCuentas() = default;
    RepositorioCuentas(const RepositorioCuentas&) = delete;
    RepositorioCuentas& operator=(const RepositorioCuentas&) = delete;

    static RepositorioCuentas& instancia() { static RepositorioCuentas r; return r; }

public:
    // Operaciones administrativas del prototipo; en produccion requieren autorizacion.
    static void registrarCliente(const string& id) {
        if (id.empty()) throw invalid_argument("Cliente invalido");
        instancia().clientes.insert(id);
    }
    static void registrarTipo(const string& tipo) {
        if (tipo.empty()) throw invalid_argument("Tipo invalido");
        instancia().tipos.insert(tipo);
    }
    static void registrarTarifa(const string& nombre, Tarifa t) {
        if (nombre.empty() || !isfinite(t.minimo) || !isfinite(t.maximo) || t.minimo < 0.5 || t.maximo > 25 || t.minimo > t.maximo)
            throw invalid_argument("Tarifa invalida");
        for (const auto& tipo : t.tiposPermitidos) if (!instancia().tipos.contains(tipo)) throw invalid_argument("Tipo desconocido");
        if (!instancia().tarifas.emplace(nombre, move(t)).second) throw invalid_argument("Tarifa existente");
    }
};


class AccesoCliente {
    string usuario;
    RepositorioCuentas& repo;
    explicit AccesoCliente(string u): usuario(move(u)), repo(RepositorioCuentas::instancia()) {}

    Cuenta& autorizada(int id) {
        auto it = repo.cuentas.find(id);
        if (it == repo.cuentas.end() || !it->second.titulares.contains(usuario)) throw runtime_error("Acceso denegado");
        return it->second;
    }
    
    static void validar(Cuenta& c, Fecha f, Dinero importe) {
        if (f < c.apertura) throw invalid_argument("Fecha anterior a apertura");
        if (importe <= 0) throw invalid_argument("Importe debe ser positivo");
        if (!c.movimientos.empty() && f < c.movimientos.back().fecha) throw invalid_argument("No se permiten operaciones retroactivas");
    }

public:
    AccesoCliente(const AccesoCliente&) = delete;
    AccesoCliente& operator=(const AccesoCliente&) = delete;
    static AccesoCliente& instancia(const string& id) {
        if (!RepositorioCuentas::instancia().clientes.contains(id)) throw runtime_error("Cliente no registrado");
        static map<string, unique_ptr<AccesoCliente>> accesos;
        if (!accesos.contains(id)) accesos.emplace(id, unique_ptr<AccesoCliente>(new AccesoCliente(id)));
        return *accesos.at(id);
    }
    int abrirCuenta(const string& tipo, Fecha apertura, set<string> cotitulares, bool consentimiento) {
        if (!consentimiento || !repo.tipos.contains(tipo)) throw invalid_argument("Tipo o consentimiento invalido");
        cotitulares.insert(usuario);
        for (const auto& u : cotitulares) if (!repo.clientes.contains(u)) throw invalid_argument("Titular no registrado");
        int id = repo.siguiente++;
        repo.cuentas.emplace(id, Cuenta{id, tipo, apertura, move(cotitulares), {}});
        return id;
    }
    void depositar(int id, Dinero importe, Fecha f) {
        auto& c = autorizada(id); validar(c,f,importe);
        c.movimientos.push_back({f,"Deposito",importe,0});
    }
private:
    void aplicar(int id, const string& nombre, Dinero importe, double tasa, Fecha f, bool consentimiento) {
        auto& c = autorizada(id); validar(c,f,importe);
        const auto& t = repo.tarifas.at(nombre);
        if (!consentimiento || !isfinite(tasa) || tasa < t.minimo || tasa > t.maximo || (!t.tiposPermitidos.empty() && !t.tiposPermitidos.contains(c.tipo))) throw invalid_argument("Operacion no autorizada o tasa invalida");
        Dinero cargo = llround(importe * tasa / 100.0);
        if (t.generaDeuda) {
            // Prestamo: abono de capital. Compra: deuda sin abono.
            c.movimientos.push_back({f,nombre, nombre == "Prestamo personal" ? importe : 0, importe+cargo});
        } else {
            if (c.saldo(f) < importe+cargo) throw runtime_error("Fondos insuficientes");
            c.movimientos.push_back({f,nombre,-importe-cargo,0});
        }
    }

public:
    void operar(int id, const string& nombre, Dinero importe, double tasa, Fecha f, bool consentimiento) {
        if (nombre == "Prestamo personal") throw invalid_argument("Use prestamo con calificacion");
        aplicar(id,nombre,importe,tasa,f,consentimiento);
    }
    void prestamo(int id, Dinero importe, int calificacion, Fecha f, bool consentimiento) {
        if (calificacion < 0 || calificacion > 100) throw invalid_argument("Calificacion invalida");
        double tasa = 10.0 - 2.0 * calificacion / 100.0;
        aplicar(id,"Prestamo personal",importe,tasa,f,consentimiento);
    }
    void pagarDeuda(int id, Dinero importe, Fecha f) {
        auto& c = autorizada(id); validar(c,f,importe);
        if (importe > c.deuda(f) || importe > c.saldo(f)) throw runtime_error("Pago superior a deuda o saldo disponible");
        c.movimientos.push_back({f,"Pago de deuda",-importe,-importe});
    }
    void liquidar(int id, Fecha f) { auto& c = autorizada(id); auto d = c.deuda(f); if (d > 0) pagarDeuda(id,d,f); }
    void estadoCuenta(Fecha corte, optional<string> tipo = nullopt) const {
        cout << "\nEstado de " << usuario << " al " << texto(corte) << " | " << tipo.value_or("Global") << '\n';
        Dinero saldo=0, deuda=0;
        for (const auto& [id,c] : repo.cuentas) {
            if (c.apertura > corte || !c.titulares.contains(usuario) || (tipo && c.tipo != *tipo)) continue;
            cout << "Cuenta " << id << " - " << c.tipo << (c.titulares.size()>1 ? " (mancomunada)" : "") << '\n';
            for (const auto& m : c.movimientos) if (m.fecha <= corte) { cout << "  " << texto(m.fecha) << " " << m.concepto << " | saldo: "; monto(m.saldo); cout << " | deuda: "; monto(m.deuda); cout << '\n'; }
            saldo += c.saldo(corte); deuda += c.deuda(corte);
        }
        cout << "Saldo total: "; monto(saldo); cout << " | Deuda total: "; monto(deuda); cout << '\n';
    }
};
int main() {
    try {
        RepositorioCuentas::registrarCliente("Ana");
        RepositorioCuentas::registrarCliente("Luis");
        auto& ana = AccesoCliente::instancia("Ana");
        auto& luis = AccesoCliente::instancia("Luis");
        cout << boolalpha << "Mismo cliente, misma instancia: " << (&ana == &AccesoCliente::instancia("Ana")) << '\n';
        cout << "Clientes distintos, instancias distintas: " << (&ana != &luis) << '\n';
        auto f1=fecha(2026,11,1), f2=fecha(2026,11,10), f3=fecha(2026,11,20);
        int ahorro=ana.abrirCuenta("Ahorro",f1,{"Luis"},true);
        int credito=ana.abrirCuenta("Credito",f1,{},true);
        ana.depositar(ahorro,200000,f1);
        luis.operar(ahorro,"Transferencia nacional",10000,2,f2,true);
        ana.prestamo(ahorro,100000,100,f2,true);
        ana.pagarDeuda(ahorro,50000,f3);
        ana.liquidar(ahorro,f3);
        ana.operar(credito,"Compra con credito",20000,20,f2,true);
        ana.depositar(credito,24000,f3);
        ana.liquidar(credito,f3);
        ana.estadoCuenta(f2);
        ana.estadoCuenta(f3,"Credito");
        luis.estadoCuenta(f3);
        RepositorioCuentas::registrarTipo("Digital");
        RepositorioCuentas::registrarTarifa("Pago de servicios",{0.5,2,false,{"Digital"}});
        int digital=luis.abrirCuenta("Digital",f3,{},true);
        luis.depositar(digital,10000,f3);
        luis.operar(digital,"Pago de servicios",2000,1,f3,true);
        luis.estadoCuenta(f3);
        try { luis.depositar(credito,100,f3); } catch(const exception& e) { cout << "Prueba de acceso: " << e.what() << '\n'; }
    } catch(const exception& e) { cerr << e.what() << '\n'; return 1; }
}
