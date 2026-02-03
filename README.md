# Sistema Distribuido de Gestión de Tareas (PSCD) 🚀

Proyecto académico para la asignatura de **Programación de Sistemas Concurrentes y Distribuidos**. Implementa una arquitectura Cliente-Servidor en C++ para la distribución y procesamiento concurrente de tareas.

## 📋 Descripción del Sistema
El sistema simula un entorno de procesamiento distribuido compuesto por tres entidades principales:
1.  **Servidor de Tareas:** Almacena una lista de trabajos y los distribuye bajo demanda a los clientes conectados. Gestiona la concurrencia mediante monitores (`mtBuffer`).
2.  **Clientes (Controladores):** Solicitan tareas, simulan su ejecución (con retardo y probabilidad de fallo) y reportan el resultado.
3.  **Servidor Matriz/Resultados:** Recolecta los estados finales ("OK" o "FAIL") de cada tarea procesada.

## 🛠️ Tecnologías Clave
* **Lenguaje:** C++11 (STL).
* **Comunicación:** Sockets TCP/IP (API de Berkeley).
* **Concurrencia:** `std::thread` y `pthread`.
* **Sincronización:** Monitores y Semáforos para garantizar *Thread Safety*.

## ⚙️ Compilación
El proyecto cuenta con un `Makefile` automatizado.

1.  **Compilar todo el proyecto:**
    ```bash
    make
    ```
    Esto generará los ejecutables: `ServidorTareas`, `ServidorMatriz` y `Cliente`.

2.  **Limpiar binarios:**
    ```bash
    make clean
    ```

## 🚀 Instrucciones de Ejecución
Para desplegar el sistema correctamente, es necesario abrir **3 terminales** distintas. Se recomienda seguir este orden estricto para asegurar que los puertos estén abiertos cuando los clientes intenten conectarse.

### 1. Iniciar Servidor de Resultados (Matriz)
Este servidor recolecta los estados finales de las tareas. Según la configuración por defecto del cliente, se espera que escuche en el puerto **3001**.

```bash
# Sintaxis: ./ServidorMatriz <puerto>
./ServidorMatriz 3001
```

### 2. Iniciar Servidor de Tareas
Este servidor distribuye el trabajo. Por defecto en el código (`ServidorTareas.cpp`), si no se especifica puerto, escucha en el **3000**.

```bash
# Sintaxis: ./ServidorTareas <puerto>
./ServidorTareas 3000
```

### 3. Iniciar Clientes (Controladores)
El ejecutable del cliente lanza múltiples hilos (10 por defecto) que actúan como controladores concurrentes.

**Opción A: Ejecución Local (Por defecto)**
Si no se pasan argumentos, el cliente intenta conectarse a `localhost:3000` (Tareas) y `localhost:3001` (Matriz).
```bash
./Cliente
```

**Opción B: Ejecución Distribuida (Argumentos explícitos)**
Si quieres cambiar IPs o puertos (según la lógica en `Cliente.cpp`), debes proporcionar los 4 argumentos:
```bash
# Sintaxis: ./Cliente <IP_Tareas> <Puerto_Tareas> <IP_Matriz> <Puerto_Matriz>
./Cliente 127.0.0.1 3000 127.0.0.1 3001
```

## 📂 Detalles de Implementación

### Tipos de Tareas y Simulación de Fallos
El sistema simula diferentes cargas de trabajo y fiabilidad según el tipo de tarea solicitado (definido en `Cliente.cpp`):

| Tipo Tarea | Tasa de Fallo | Simulación |
| :--- | :--- | :--- |
| **t1** | 5% | Baja probabilidad de error. |
| **t2** | 7% | Fiabilidad media. |
| **t3** | 10% | Alta probabilidad de error. |

*La carga de trabajo (duración simulada) se recibe dinámicamente desde el servidor.*

### Protocolo de Comunicación
El intercambio de mensajes sigue un formato de texto simple sobre TCP:
1.  **Solicitud:** Cliente envía `GET_TASK`.
2.  **Respuesta:** Servidor responde `tipo,carga` (ej: `t1,0.5`) o `TF` (Task Finished) si no quedan tareas.
3.  **Reporte:** Cliente envía el resultado al Servidor Matriz en formato `tipo,estado,carga` (ej: `t1,OK,0.5`).

## 📁 Estructura del Proyecto

* **`src/`**:
    * `ServidorTareas.cpp`: Lógica del servidor maestro y gestión de la cola.
    * `Cliente.cpp`: Implementación de los hilos controladores.
    * `mtBuffer.cpp` / `mtResultados.cpp`: Monitores para sincronización *thread-safe*.
* **`librerias/`**:
    * `Socket/`: Clase `Socket` que encapsula la API de Berkeley (bind, listen, connect).
    * `MultiBuffer/`: Estructura de datos para el manejo concurrente de pedidos.
* **`tareas.txt`**: Archivo de entrada que define la carga de trabajo inicial.

---
*Autor: Javier Valero - Grado en Ingeniería Informática*
