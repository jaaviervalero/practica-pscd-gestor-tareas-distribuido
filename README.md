# Sistema Distribuido de Gestión de Tareas (Cliente-Servidor)

Este proyecto implementa una arquitectura **Cliente-Servidor** distribuida en C++ para la gestión concurrente de tareas. El sistema simula un entorno donde múltiples controladores (clientes) solicitan trabajo a un servidor central, procesan la tarea y reportan los resultados a un servidor de métricas/matriz.

## 🚀 Características Principales
* **Arquitectura:** Diseño modular con separación de responsabilidades (Servidor de Tareas, Servidor de Resultados y Clientes).
* **Comunicación:** Uso de **Sockets TCP/IP** robustos (implementados en `librerias/Socket`).
* **Concurrencia:** * Manejo de hilos (`std::thread`) para atender múltiples clientes simultáneamente.
    * Sincronización mediante **Monitores** (`mtBuffer`, `mtResultados`) para garantizar la seguridad en el acceso a datos compartidos (Thread Safety).
* **Simulación de Carga:** Los clientes simulan tiempos de ejecución y tasas de fallo aleatorias según el tipo de tarea.

## 🛠️ Requisitos
* Entorno Linux/Unix.
* Compilador `g++` con soporte para C++11.
* Librería `pthread` (para el manejo de hilos).

## 📦 Compilación
El proyecto incluye un `Makefile` para automatizar la construcción. Simplemente ejecuta:

```bash
make
