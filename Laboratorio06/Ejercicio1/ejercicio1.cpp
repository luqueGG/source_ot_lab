#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>

// Estructura del Nodo para la lista enlazada simple
struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

// Clase ThreadSafeList: Lista enlazada protegida por mutex
class ThreadSafeList {
private:
    Node* head;
    mutable std::mutex mtx; // Mutex para controlar el acceso concurrente

public:
    ThreadSafeList() : head(nullptr) {}

    // Destructor para liberar memoria
    ~ThreadSafeList() {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Inserción de un elemento (Sección Crítica)
    void insert(int val) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        std::cout << "[Thread Insertar] Insertando: " << val << std::endl;
    }

    // Eliminación de un elemento (Sección Crítica)
    bool remove(int val) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->data == val) {
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                std::cout << "[Thread Eliminar] eliminando: " << val << std::endl;
                return true;
            }
            prev = current;
            current = current->next;
        }
        std::cout << "[Thread Eliminar] No se eliminó " << val << std::endl;
        return false;
    }

    // Búsqueda de un elemento (Sección Crítica)
    bool search(int val) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        while (current != nullptr) {
            if (current->data == val) {
                std::cout << "[Thread Buscar] Buscado: " << val << std::endl;
                return true;
            }
            current = current->next;
        }
        std::cout << "[Thread Buscar] No encontrado: " << val << std::endl;
        return false;
    }

    // Modificación de un elemento (Sección Crítica)
    bool modify(int target, int added_value) {
        std::lock_guard<std::mutex> lock(mtx);
        Node* current = head;
        while (current != nullptr) {
            if (current->data == target) {
                int new_val = target + added_value;
                current->data = new_val;
                std::cout << "[Thread Modificar] Modificando " << target << " a " << new_val << std::endl;
                return true;
            }
            current = current->next;
        }
        std::cout << "[Thread Modificar] No se encontró " << target << std::endl;
        return false;
    }
};

int main() {
    ThreadSafeList lista;

    // Inicializamos la lista con algunos valores base para facilitar pruebas iniciales
    for (int i = 1; i <= 5; ++i) {
        lista.insert(i * 10);
    }

    std::cout << "\n--- INICIO DE PROCESOS CONCURRENTES ---\n\n";

    // Generador de números aleatorios independiente por hilo o compartido de forma segura
    auto random_generator = []() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 50);
        return dis(gen);
    };

    // Thread 1: Se encarga de eliminar valores (10 operaciones)
    std::thread t1([&lista]() {
        for (int i = 0; i < 10; ++i) {
            int val = (rand() % 50) + 1;
            lista.remove(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    });

    // Thread 2: Se encarga de insertar elementos (10 operaciones)
    std::thread t2([&lista]() {
        for (int i = 0; i < 10; ++i) {
            int val = (rand() % 50) + 1;
            lista.insert(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    });

    // Thread 3: Se encarga de consultar valores (10 operaciones)
    std::thread t3([&lista]() {
        for (int i = 0; i < 10; ++i) {
            int val = (rand() % 50) + 1;
            lista.search(val);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    });

    // Thread 4: Se encarga de modificar valores sumándoles una cantidad (10 operaciones)
    std::thread t4([&lista]() {
        for (int i = 0; i < 10; ++i) {
            int val = (rand() % 50) + 1;
            int incremento = 5;
            lista.modify(val, incremento);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    });

    // Esperar a que terminen los 4 threads (join)
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "\n--- FIN DE LA EJECUCIÓN CONCURRENTE ---\n";
    return 0;
}
