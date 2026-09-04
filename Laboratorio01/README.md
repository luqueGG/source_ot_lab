# Indicaciones
Compilar desde Laboratorio01/

## Configuracion
Aqui construye los binarios necesarios para el proyecto
Solo se hace una vez
```
cmake -B build -G Ninja
```
Ejecutado esto, entonces podemos desarrollar, editar archivos
y/o compilar el laboratorio.
Para ello el comando es el siguiente
```
cmake --build build
```
El ejecutable se encontrara en build/Laboratorio01.exe

## Desarrollo
- Si se edita el contenido de cualquier archivo (excepto CMakeLists.txt)
los cambios se reflejaran cuando ejecutemos 
```
cmake --build build
```
- Si se cambia el nombre de algun archivo, se agrega mas archivos o se
modifica el CMakeLists.txt el proceso es
```
rmdir build/
cmake -B build -G Ninja
cmake --build build
```