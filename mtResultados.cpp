#include "mtResultados.hpp"

mtResultados::mtResultados() {}
mtResultados::~mtResultados() {}

int mtResultados::indicePorTipo(string tipo) {
    if (tipo == "t1") return 0;
    if (tipo == "t2") return 1;
    if (tipo == "t3") return 2;
    return -1;
}

void mtResultados::anotarResultado(string tipoTarea, bool exito, float carga) {
    lock_guard<mutex> lock(mtx);
    int idx = indicePorTipo(tipoTarea);
    if (idx != -1) {
        stats[idx].numEjecutadas++;
        if (exito) {
            stats[idx].numExito++;
            stats[idx].tiempoTotalExito += carga;
        }
    }
}

// Este método encapsula la barrera. Todos los hilos entran aquí.
void mtResultados::esperarFinalizacion(int totalClientes) {
    unique_lock<mutex> lock(mtx);
    clientesFinalizados++;

    if (clientesFinalizados == totalClientes) {
        // El último hilo imprime los resultados y despierta a los demás
        imprimirResultados();
        cv_fin.notify_all();
    } else {
        // Los demás esperan
        cv_fin.wait(lock, [this, totalClientes] { 
            return clientesFinalizados == totalClientes; 
        });
    }
}

void mtResultados::imprimirResultados() {
    // Nota: El mutex ya está bloqueado por quien llama a esta función
    cout << "Resultados de las tareas:" << endl;
    string tipos[3] = {"t1", "t2", "t3"};
    for (int i = 0; i < 3; ++i) {
        cout << "Tipo de tarea " << tipos[i] << ":" << endl;
        cout << "  Número de tareas ejecutadas: " << stats[i].numEjecutadas << endl;
        cout << "  Número de tareas con éxito: " << stats[i].numExito << endl;
        if (stats[i].numExito > 0) {
            cout << "  Tiempo medio de ejecución: " 
                 << stats[i].tiempoTotalExito / stats[i].numExito 
                 << " segundos" << endl;
        } else {
             cout << "  Tiempo medio de ejecución: 0 segundos" << endl;
        }
    }
}