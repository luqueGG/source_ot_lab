#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

// Enumeraciones para tipado fuerte
enum class Moneda { PEN, USD };
enum class TipoProducto { AHORRO, JOVEN, PLUSS, EMPRESARIAL, TARJETA_CREDITO, PRESTAMO_PERSONAL };

std::string monedaToString(Moneda m) {
    return (m == Moneda::PEN) ? "S/" : "$";
}

// -------------------------------------------------------------------------
// 1. ESTRUCTURAS DE MOVIMIENTOS Y REGLAS EXTENSIBLES (Lambdas)
// -------------------------------------------------------------------------
struct Transaccion {
    std::string fecha; // Formato YYYY-MM-DD para comparación cronológica
    std::string descripcion;
    double monto;
    double interesOComision;
};

// Permite a la entidad crear nuevas tasas o reglas según el mercado
struct ReglaFinanciera {
    std::string nombreRegla;
    double tasaPorcentual;
    std::function<double(double, Moneda)> calculadorPersonalizado;

    double aplicar(double monto, Moneda moneda) const {
        if (calculadorPersonalizado) {
            return calculadorPersonalizado(monto, moneda);
        }
        return monto * (tasaPorcentual / 100.0);
    }
};

// -------------------------------------------------------------------------
// 2. CLASE BASE ABSTRACTA (Herencia, Polimorfismo, Constructores, Sobrecarga)
// -------------------------------------------------------------------------
class ProductoFinanciero {
protected:
    std::string numeroCuenta;
    Moneda moneda;
    double saldo; // En cuentas es saldo a favor; en créditos/préstamos es deuda actual
    double tasaBase;
    bool esMancomunada;
    std::vector<Transaccion> historial;
    std::vector<ReglaFinanciera> reglasActivas; // Reglas aceptadas por el cliente

public:
    ProductoFinanciero(std::string num, Moneda mon, double saldoInicial, double tasa, bool mancomunada = false)
        : numeroCuenta(std::move(num)), moneda(mon), saldo(saldoInicial), tasaBase(tasa), esMancomunada(mancomunada) {
        if (tasa < 0.45 || tasa > 26.50) {
            throw std::invalid_argument("La tasa base debe estar entre 0.45% y 26.50%");
        }
    }

    virtual ~ProductoFinanciero() = default;

    // Método virtual puro para polimorfismo
    virtual double calcularProyeccionInteres() const = 0;
    virtual std::string getTipoNombre() const = 0;

    // Agregar nuevas opciones/productos aceptados por el cliente
    void suscribirReglaMercado(const ReglaFinanciera& regla, bool consentimientoCliente) {
        if (consentimientoCliente) {
            reglasActivas.push_back(regla);
        }
    }

    // Transferencia nacional con tasas diferenciadas (PEN: 0.5%-4%, USD: 0.21%-0.35%)
    virtual void transferirNacional(ProductoFinanciero& destino, double monto, double tasaAplicada, const std::string& fecha) {
        if (moneda == Moneda::PEN && (tasaAplicada < 0.5 || tasaAplicada > 4.0)) {
            throw std::invalid_argument("Tasa transferencia PEN fuera de rango (0.5% - 4%)");
        }
        if (moneda == Moneda::USD && (tasaAplicada < 0.21 || tasaAplicada > 0.35)) {
            throw std::invalid_argument("Tasa transferencia USD fuera de rango (0.21% - 0.35%)");
        }

        double comision = monto * (tasaAplicada / 100.0);
        if (saldo < (monto + comision)) {
            throw std::runtime_error("Saldo insuficiente para cubrir monto y comisión.");
        }

        saldo -= (monto + comision);
        historial.push_back({fecha, "Transferencia enviada a " + destino.numeroCuenta, -monto, comision});

        destino.saldo += monto;
        destino.historial.push_back({fecha, "Transferencia recibida de " + numeroCuenta, monto, 0.0});
    }

    // Sobrecarga de operador += para abonos / depósitos
    ProductoFinanciero& operator+=(double monto) {
        if (monto > 0) {
            saldo += monto;
            historial.push_back({"2026-11-24", "Abono / Depósito directo", monto, 0.0});
        }
        return *this;
    }

    // Estado de cuenta con corte a una fecha determinada
    void imprimirEstadoCuenta(const std::string& fechaCorte) const {
        std::cout << "  [" << getTipoNombre() << "] Nro: " << numeroCuenta
                  << (esMancomunada ? " (MANCOMUNADA)" : " (INDIVIDUAL)")
                  << " | Moneda: " << monedaToString(moneda)
                  << " | Saldo/Deuda Actual: " << std::fixed << std::setprecision(2) << saldo
                  << " | Tasa: " << tasaBase << "%\n";
        std::cout << "    Movimientos hasta " << fechaCorte << ":\n";

        bool hayMovs = false;
        for (const auto& t : historial) {
            if (t.fecha <= fechaCorte) {
                std::cout << "      - " << t.fecha << " | " << t.descripcion
                          << " | Monto: " << monedaToString(moneda) << t.monto
                          << " (Int/Com: " << t.interesOComision << ")\n";
                hayMovs = true;
            }
        }
        if (!hayMovs) std::cout << "      (Sin movimientos registrados hasta la fecha de corte)\n";
    }

    std::string getNumero() const { return numeroCuenta; }
    double getSaldo() const { return saldo; }
};

// -------------------------------------------------------------------------
// 3. CLASES DERIVADAS (Especialización de Cuentas, Tarjetas y Préstamos)
// -------------------------------------------------------------------------

// Cuentas Pasivas: Ahorro, Joven, Pluss, Empresarial
class CuentaBancaria : public ProductoFinanciero {
private:
    TipoProducto subtipo;
public:
    CuentaBancaria(std::string num, TipoProducto tipo, Moneda mon, double saldoIni, double tasa, bool mancomunada = false)
        : ProductoFinanciero(std::move(num), mon, saldoIni, tasa, mancomunada), subtipo(tipo) {}

    double calcularProyeccionInteres() const override {
        double interes = saldo * (tasaBase / 100.0);
        for (const auto& r : reglasActivas) {
            interes += r.aplicar(saldo, moneda);
        }
        return interes;
    }

    std::string getTipoNombre() const override {
        switch (subtipo) {
            case TipoProducto::AHORRO: return "Cuenta Ahorro";
            case TipoProducto::JOVEN: return "Cuenta Joven";
            case TipoProducto::PLUSS: return "Cuenta Pluss";
            case TipoProducto::EMPRESARIAL: return "Cuenta Empresarial";
            default: return "Cuenta General";
        }
    }
};

// Tarjeta de Crédito (Compras 15% a 25%, adelanto/liquidación de deuda)
class TarjetaCredito : public ProductoFinanciero {
private:
    double lineaCredito;
public:
    TarjetaCredito(std::string num, Moneda mon, double linea, double tasaCompra, bool mancomunada = false)
        : ProductoFinanciero(std::move(num), mon, 0.0, tasaCompra, mancomunada), lineaCredito(linea) {
        if (tasaCompra < 15.0 || tasaCompra > 25.0) {
            throw std::invalid_argument("Tasa de compras con TC debe estar entre 15% y 25%");
        }
    }

    void registrarCompra(double monto, const std::string& comercio, const std::string& fecha) {
        if (saldo + monto > lineaCredito) {
            throw std::runtime_error("Línea de crédito excedida.");
        }
        double interesGenerado = monto * (tasaBase / 100.0);
        saldo += (monto + interesGenerado);
        historial.push_back({fecha, "Compra en " + comercio, monto, interesGenerado});
    }

    // Adelantar o liquidar deuda total/parcial
    void pagarDeuda(double monto, const std::string& fecha) {
        if (monto >= saldo) {
            double pagado = saldo;
            saldo = 0.0;
            historial.push_back({fecha, "LIQUIDACIÓN TOTAL DE DEUDA TC", -pagado, 0.0});
        } else {
            saldo -= monto;
            historial.push_back({fecha, "Adelanto/Pago parcial deuda TC", -monto, 0.0});
        }
    }

    double calcularProyeccionInteres() const override {
        return saldo * (tasaBase / 100.0);
    }

    std::string getTipoNombre() const override { return "Tarjeta de Crédito"; }
};

// Préstamos Personales (8.55% a 11.25% sujeto a calificación)
class PrestamoPersonal : public ProductoFinanciero {
private:
    int scoreCrediticio;
public:
    PrestamoPersonal(std::string num, Moneda mon, double montoPrestado, int score, double tasaCalificada)
        : ProductoFinanciero(std::move(num), mon, montoPrestado, tasaCalificada, false), scoreCrediticio(score) {
        if (tasaCalificada < 8.55 || tasaCalificada > 11.25) {
            throw std::invalid_argument("Tasa de préstamo personal fuera de rango (8.55% - 11.25%)");
        }
        historial.push_back({"2026-01-10", "Desembolso Préstamo (Score: " + std::to_string(score) + ")", montoPrestado, 0.0});
    }

    // Liquidar o adelantar cuotas reduciendo capital
    void adelantarOLiquidar(double monto, const std::string& fecha) {
        if (monto >= saldo) {
            historial.push_back({fecha, "LIQUIDACIÓN TOTAL DE PRÉSTAMO", -saldo, 0.0});
            saldo = 0.0;
        } else {
            saldo -= monto;
            historial.push_back({fecha, "Adelanto a capital de préstamo", -monto, 0.0});
        }
    }

    double calcularProyeccionInteres() const override {
        return saldo * (tasaBase / 100.0);
    }

    std::string getTipoNombre() const override { return "Préstamo Personal"; }
};

// -------------------------------------------------------------------------
// 4. CLASE CLIENTE (Soporta múltiples productos y cuentas mancomunadas)
// -------------------------------------------------------------------------
class Cliente {
private:
    std::string id;
    std::string nombre;
    // shared_ptr permite que dos clientes compartan la misma cuenta mancomunada
    std::vector<std::shared_ptr<ProductoFinanciero>> productos;

public:
    Cliente(std::string idCliente, std::string nom) : id(std::move(idCliente)), nombre(std::move(nom)) {}

    void vincularProducto(const std::shared_ptr<ProductoFinanciero>& prod) {
        productos.push_back(prod);
    }

    // Estado de cuenta GLOBAL o filtrado por TIPO
    void solicitarEstadoCuenta(const std::string& fechaCorte, const std::string& filtroTipo = "GLOBAL") const {
        std::cout << "\n====================================================================\n";
        std::cout << "ESTADO DE CUENTA (" << filtroTipo << ") - Cliente: " << nombre << " [" << id << "]\n";
        std::cout << "Fecha de corte: " << fechaCorte << "\n";
        std::cout << "====================================================================\n";

        for (const auto& p : productos) {
            if (filtroTipo == "GLOBAL" || p->getTipoNombre() == filtroTipo) {
                p->imprimirEstadoCuenta(fechaCorte);
            }
        }
    }

    std::string getNombre() const { return nombre; }
    const std::vector<std::shared_ptr<ProductoFinanciero>>& getProductos() const { return productos; }
};

// -------------------------------------------------------------------------
// 5. TEMPLATE GENÉRICO PARA CONSULTAS Y OPERACIONES EN LA ENTIDAD
// -------------------------------------------------------------------------
template <typename T>
class RepositorioEntidad {
private:
    std::vector<std::shared_ptr<T>> elementos;
public:
    void registrar(const std::shared_ptr<T>& item) {
        elementos.push_back(item);
    }

    // Búsqueda genérica utilizando predicados Lambda
    std::vector<std::shared_ptr<T>> filtrar(std::function<bool(const std::shared_ptr<T>&)> criterio) const {
        std::vector<std::shared_ptr<T>> resultado;
        std::copy_if(elementos.begin(), elementos.end(), std::back_inserter(resultado), criterio);
        return resultado;
    }
};

// -------------------------------------------------------------------------
// 6. FUNCIÓN PRINCIPAL (Demostración integral de todos los requerimientos)
// -------------------------------------------------------------------------
int main() {
    try {
        RepositorioEntidad<Cliente> bancoClientes;

        auto cliente1 = std::make_shared<Cliente>("DNI-45891230", "Valeria Mendoza");
        auto cliente2 = std::make_shared<Cliente>("DNI-71239845", "Carlos Mendoza"); // Cotitular

        bancoClientes.registrar(cliente1);
        bancoClientes.registrar(cliente2);

        // 1. Creación de diversos productos con tasas dentro de rangos exigidos
        auto ctaAhorro = std::make_shared<CuentaBancaria>("191-001", TipoProducto::AHORRO, Moneda::PEN, 5000.0, 1.20);
        auto ctaPlussMancomunada = std::make_shared<CuentaBancaria>("191-002", TipoProducto::PLUSS, Moneda::USD, 12000.0, 2.50, true);
        auto ctaEmpresarial = std::make_shared<CuentaBancaria>("191-003", TipoProducto::EMPRESARIAL, Moneda::PEN, 35000.0, 3.10);
        auto tcOro = std::make_shared<TarjetaCredito>("4557-8899", Moneda::PEN, 15000.0, 18.50);
        auto prestamo = std::make_shared<PrestamoPersonal>("PR-9001", Moneda::PEN, 10000.0, 780, 9.40);

        // 2. Vinculación múltiple y cuenta mancomunada compartida entre cliente1 y cliente2
        cliente1->vincularProducto(ctaAhorro);
        cliente1->vincularProducto(ctaPlussMancomunada);
        cliente1->vincularProducto(tcOro);
        cliente1->vincularProducto(prestamo);

        cliente2->vincularProducto(ctaPlussMancomunada); // Misma instancia compartida
        cliente2->vincularProducto(ctaEmpresarial);

        // 3. Extensibilidad: Nueva opción de mercado con consentimiento del cliente (Lambda)
        ReglaFinanciera bonoFidelidadPluss{
            "Bono Mercado 2026 (+0.75% si saldo > $10,000)",
            0.75,
            [](double saldo, Moneda m) -> double {
                return (m == Moneda::USD && saldo > 10000.0) ? saldo * 0.0075 : 0.0;
            }
        };
        ctaPlussMancomunada->suscribirReglaMercado(bonoFidelidadPluss, true);

        // 4. Operaciones: Sobrecarga de operador (+=), Transferencias nacionales, Compras y Liquidaciones
        *ctaAhorro += 1500.0; // Uso de operador sobrecargado +=
        ctaAhorro->transferirNacional(*ctaEmpresarial, 1000.0, 1.5, "2026-06-15"); // 1.5% PEN

        tcOro->registrarCompra(2000.0, "TechStore Arequipa", "2026-05-10");
        tcOro->pagarDeuda(1000.0, "2026-06-01"); // Adelanto de deuda TC

        prestamo->adelantarOLiquidar(10000.0, "2026-08-20"); // Liquidación total de préstamo

        // 5. Consultas: Estado de cuenta GLOBAL con corte a una fecha y filtrado por TIPO
        cliente1->solicitarEstadoCuenta("2026-07-01", "GLOBAL");
        cliente1->solicitarEstadoCuenta("2026-11-24", "Tarjeta de Crédito");

        // 6. Consulta mediante Template + Expresión Lambda (Buscar clientes con cuentas mancomunadas)
        auto conMancomunadas = bancoClientes.filtrar([](const std::shared_ptr<Cliente>& c) {
            for (const auto& p : c->getProductos()) {
                if (p->getTipoNombre() == "Cuenta Pluss") return true;
            }
            return false;
        });

        std::cout << "\nClientes titulares de Cuenta Pluss (Mancomunada): " << conMancomunadas.size() << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error operativo: " << ex.what() << "\n";
    }

    return 0;
}
