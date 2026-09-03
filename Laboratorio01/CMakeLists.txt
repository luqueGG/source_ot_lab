cmake_minimum_required(VERSION 3.20)

project(Laboratorio01 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    src/ejercicio1.cpp
    src/ejercicio2.cpp
    src/ejercicio3.cpp
    src/ejercicio4.cpp
    src/ejercicio5.cpp
    main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
