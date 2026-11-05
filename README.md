# Laboratorios de Tecnología de Objetos — C++

Repositorio académico de ejercicios desarrollados para la asignatura **Tecnología de Objetos**, de la **Universidad Nacional de San Agustín de Arequipa (UNSA)**, durante 2026.

Los laboratorios incluyen fundamentos de C++, programación orientada a objetos, estructuras de datos, plantillas, interfaces gráficas con Qt y programación concurrente.

## Integrantes

- Llavilla Machaca Frayd Yandel
- Luque Guevara Fernando Gerson
- Schreiber Landeo Diego Hans

## Contenido

| Laboratorio | Tema principal | Ejercicios |
| --- | --- | --- |
| [01](Laboratorio01/) | Fundamentos de C++ y clases | Ordenamiento de números, salida tabulada y modelado de citas médicas. |
| [02](Laboratorio02/) | Clases y listas enlazadas | Analizador de expresiones aritméticas y lista doblemente enlazada. |
| [03](Laboratorio03/) | Herencia y árboles | Polimorfismo entre profesiones, herencia múltiple y árbol de expresiones. |
| [04](Laboratorio04/) | Algoritmos y Qt | Conteo de números primos y aplicación gráfica de países. |
| [05](Laboratorio05/) | Plantillas y estructuras genéricas | Lista enlazada genérica, clase plantilla, árbol binario y aplicación Qt de sinónimos. |
| [06](Laboratorio06/) | Concurrencia con hilos | Lista enlazada protegida con `mutex` y cálculo concurrente de distancias. |

## Detalle de los ejercicios

### Laboratorio 01 — Fundamentos y clases

Proyecto organizado con CMake; los ejercicios se llaman desde un programa principal.

- **[Ejercicio 1](Laboratorio01/src/ejercicio1.cpp):** recibe una cantidad de números, los genera aleatoriamente o permite ingresarlos, y los ordena de mayor a menor.
- **[Ejercicio 2](Laboratorio01/src/ejercicio2.cpp):** presenta información tabulada en consola con `std::setw` y alineación de columnas.
- **[Ejercicio 4](Laboratorio01/src/ejercicio4.cpp):** modela una cita médica mediante una estructura con constructor y métodos para mostrar datos, identificar urgencias y modificar información.
- **[Ejercicio 5](Laboratorio01/src/ejercicio5.cpp):** implementa una clase `CitaMedica` con atributos privados, constructor y comprobación del tipo de atención.

Instrucciones específicas: [README del Laboratorio 01](Laboratorio01/README.md).

### Laboratorio 02 — Objetos y listas doblemente enlazadas

- **[Ejercicio 1](Laboratorio02/ejercicio1/main.cpp):** analiza expresiones aritméticas con sumas y restas mediante las clases `Analizador`, `Calculadora` y `Procesador`.
- **[Ejercicio 2](Laboratorio02/ejercicio2/main.cpp):** implementa una lista doblemente enlazada con inserción al final, recorrido en ambos sentidos y liberación de memoria.

### Laboratorio 03 — Herencia, polimorfismo y árboles

- **[Ejercicio 1](Laboratorio03/ejercicio01/main.cpp):** aplica herencia y polimorfismo con `Persona` y diversas profesiones, usando `std::unique_ptr` para administrar los objetos.
- **[Ejercicio 2](Laboratorio03/ejercicio02/Ejercicio2cepas.cpp):** ejemplifica la herencia múltiple con clases que representan cepas y una clase derivada híbrida; se trata de una demostración de programación, no de un modelo epidemiológico validado.
- **[Ejercicio 3](Laboratorio03/ejercicio03/main.cpp):** construye un árbol binario de expresiones y evalúa recursivamente operaciones aritméticas.

### Laboratorio 04 — Algoritmos e interfaces gráficas

- **[Ejercicio 1](Laboratorio04/ejercicio1/main.cpp):** genera 100 enteros aleatorios y cuenta cuántos son números primos menores que 1000; emplea algoritmos de la biblioteca estándar.
- **[Ejercicio 2](Laboratorio04/ejercicio2/main.cpp):** construye una interfaz con **Qt Widgets** que muestra países y actualiza su capital e idioma mediante señales y funciones conectadas.

### Laboratorio 05 — Programación genérica y Qt

- **[Ejercicio 1](Laboratorio05/Ejercicio%201/ListaEnlazada.cpp):** implementa una lista simplemente enlazada genérica con plantillas, inserción, eliminación, búsqueda y consulta del tamaño.
- **[Ejercicio 4](Laboratorio05/Ejercicio%204/clase_plantilla.cpp):** ilustra una clase plantilla parametrizada por tipo y tamaño de un arreglo. **Nota:** el archivo actual contiene marcadores `[cite: 1]` incrustados en el código, por lo que debe corregirse antes de compilar.
- **[Ejercicio 5 — Árbol genérico](Laboratorio05/Ejercicio%205/estructura_arbol_clase.cpp):** implementa un árbol binario de búsqueda con plantillas y recorrido en orden.
- **[Aplicación Qt — Checklist de sinónimos](Laboratorio05/Ejercicio_5/):** aplicación gráfica para administrar pares de sinónimos, con acciones de agregar, editar, quitar y detectar duplicados. Incluye un proyecto CMake independiente.

### Laboratorio 06 — Programación concurrente con `std::thread`

- **[Ejercicio 1 — Lista enlazada concurrente](Laboratorio06/Ejercicio1/ejercicio1.cpp):** crea cuatro hilos que ejecutan diez operaciones cada uno sobre una lista simplemente enlazada. Las acciones de inserción, eliminación, búsqueda y modificación se sincronizan con `std::mutex` y `std::lock_guard`. El programa usa `join()` antes de finalizar.
- **[Ejercicio 2 — Distancias Manhattan y Euclidiana](Laboratorio06/Ejercicio2/ejericicio2.cpp):** lee pares de puntos desde un [CSV](Laboratorio06/Ejercicio2/distancias_manhattan_euclides.csv), calcula ambas distancias en dos hilos independientes y presenta los cinco primeros resultados. Los hilos comparten datos de entrada de solo lectura y escriben en vectores distintos.

Fórmulas utilizadas en el segundo ejercicio:

- Manhattan: `|x2 - x1| + |y2 - y1|`
- Euclidiana: `sqrt((x2 - x1)^2 + (y2 - y1)^2)`

## Requisitos

- Compilador moderno de **C++** (`g++`, Clang o MSVC), compatible con el estándar utilizado por el ejercicio.
- **CMake 3.20 o superior** y, opcionalmente, **Ninja** para el Laboratorio 01 (configurado con C++23).
- **Qt 5 o Qt 6, módulo Widgets**, para las aplicaciones gráficas de los laboratorios 04 y 05.
- En la aplicación de sinónimos del Laboratorio 05, **CMake 3.16 o superior** y C++17.
- Soporte de hilos en el compilador y entorno de ejecución para el Laboratorio 06.

> No existe una configuración CMake única para todos los directorios del repositorio. Los ejercicios con `main()` independientes se compilan por separado.

## Compilación y ejecución

Clonar el repositorio:

```bash
git clone https://github.com/luqueGG/source_ot_lab.git
cd source_ot_lab
```

### Laboratorio 01 (CMake)

```bash
cmake -S Laboratorio01 -B Laboratorio01/build -G Ninja
cmake --build Laboratorio01/build
```

Ejecutar `Laboratorio01/build/Laboratorio01` (Linux/macOS) o `Laboratorio01/build/Laboratorio01.exe` (Windows).

### Ejercicios de consola independientes

Ejemplo para el Laboratorio 02:

```bash
g++ -std=c++17 Laboratorio02/ejercicio2/main.cpp -o lab02_ej02
./lab02_ej02
```

En Windows, ejecutar el archivo `.exe` generado en lugar de `./lab02_ej02`.

### Laboratorio 06 (Linux, con g++)

Ejercicio 1:

```bash
g++ -std=c++17 -pthread Laboratorio06/Ejercicio1/ejercicio1.cpp -o lab06_ej01
./lab06_ej01
```

Ejercicio 2 (ejecutar desde la carpeta del CSV):

```bash
cd Laboratorio06/Ejercicio2
g++ -std=c++17 -pthread ejericicio2.cpp -o ejercicio2
./ejercicio2
```

**Importante:** el programa abre `distancias_manhattan_euclides.csv` mediante una ruta relativa; por ello, es necesario ejecutarlo con el directorio de trabajo adecuado. El nombre `ejericicio2.cpp` respeta la escritura actual del archivo en el repositorio.

### Aplicación Qt del Laboratorio 05

Tras instalar y configurar Qt Widgets:

```bash
cmake -S Laboratorio05/Ejercicio_5 -B Laboratorio05/Ejercicio_5/build
cmake --build Laboratorio05/Ejercicio_5/build
```

La ejecución requiere disponer de las bibliotecas y complementos de plataforma de Qt correspondientes al sistema.

## Observaciones

- Los ejemplos tienen fines didácticos y no constituyen bibliotecas listas para producción.
- En el ejercicio 1 del Laboratorio 06 se declara un generador con `<random>`, pero las tareas utilizan `rand()`; es recomendable cambiar a generadores independientes por hilo si se amplía el ejercicio.
- La concurrencia no garantiza mayor velocidad en todos los casos: el resultado depende de la carga de trabajo y el costo de coordinación.
- Los proyectos Qt requieren dependencias que no son necesarias para los ejercicios que funcionan únicamente en consola.

## Licencia

Distribuido bajo la [licencia MIT](LICENSE).
