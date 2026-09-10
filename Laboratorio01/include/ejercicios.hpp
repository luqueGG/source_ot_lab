#pragma once


/*
* EJERCICIO 1
* -------------
Crear un programa en C++, utilizar CIN / COUT para el input y output por consola (terminal),
lo siguiente: 
- El programa debe empezar solicitando digitar N (N es un numero entero positivo)
- Luego el programa solicita que el usuario digite una letra
- Si es “g”, el programa debe autogenerar números enteros aleatorios entre 1 y N (revisar SRAND). 
- Si es “i”, el programa permite ingresar manualmente N números enteros. 
- Luego, el programa debe ordenar de mayor a menor (utilizar un algoritmo de ordenamiento de su preferencia). 
*/
void ejercicio1();


/*
 * EJERCICIO 2
 * -------------
 * Hacer un programa C++ y usar el manipulador “setw” para listar una estadística relacionado a 
 * deportes (usted debe proponer). La información en la consola debe visualizarse en orden. 
 * Luego reutilizar el código anterior, y reemplazar “setw” por otro manipulador que investigaste en 
 * la hora de laboratorio. ¿Cuál te ha servido mejor? 
 * */
void ejercicio2();


/*
* EJERCICIO 5
* --------------
Implementa una cita medica usando una clase (class) en vez de estructura.
- Se define la clase CitaMedica con los atributos fecha, hora, tipoAtencion y
  numeroAsegurado, todos privados (encapsulados).
- El constructor recibe esos 4 datos y los asigna directamente al crear el objeto,
  asi no puede existir una cita sin datos cargados.
- El metodo mostrar() imprime en consola los datos de la cita ya guardados.
- El metodo esUrgencia() compara el tipoAtencion contra "Emergencia" y devuelve
  true o false segun corresponda.
- En main, se piden los 4 datos por teclado (cin y getline para el tipo de atencion,
  que puede tener espacios), se crea el objeto CitaMedica pasandole esos datos al
  constructor, se llama a mostrar() para ver la cita completa, y finalmente se usa
  esUrgencia() para avisar si es una urgencia o una cita normal.
*/
void ejercicio5();

