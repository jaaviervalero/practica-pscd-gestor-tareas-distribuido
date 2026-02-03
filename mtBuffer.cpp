#include "mtBuffer.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

mtBuffer::mtBuffer() {
    // Constructor vacío: no necesitamos inicializar nada complejo aquí
}

mtBuffer::~mtBuffer() {
}

void mtBuffer::ponerTarea(MultiBuffer<tarea, N_CONTROLLERS> &mBT) {
    ifstream f("tareas.txt");
    if (!f) {
        cerr << "Error al abrir el fichero de tareas (tareas.txt)." << endl;
        return;
    }
    
    tarea t; // Declaramos la variable 't' antes de usarla en el while
    
    // Leemos el fichero. Asumimos formato: t1,10.5
    // Usamos getline para el string y >> para el float.
    while (getline(f, t.tipoTarea, ',') && f >> t.cargaDeTrabajo) {
        // Ignoramos el salto de línea que queda en el buffer tras leer el float
        f.ignore();    
        
        unique_lock<mutex> lk(mtx); // Bloqueamos el mutex
        while (true) {
            bool puesto = false;
            // Buscamos un hueco en los buffers de los controladores
            for (int i = 0; i < N_CONTROLLERS; i++) {
                if (!mBT.isNew(i)) { // Si el controlador i tiene el buffer libre
                    mBT.put(t, i);
                    puesto = true;
                    cv_items_disponibles[i].notify_one(); // Avisamos al controlador i
                    break;
                }
            }
            
            if (puesto) {
                break; // Tarea puesta, salimos del while(true) para leer la siguiente
            } else {
                // Si no hay hueco en ningún controlador, esperamos
                cv_huecos_libres.wait(lk);
            }
        }
    }
    f.close();
}

void mtBuffer::ponerFinTareas(MultiBuffer<tarea, N_CONTROLLERS> &mBT) {
    // Ponemos una tarea FIN para cada controlador
    for (int i = 0; i < N_CONTROLLERS; ++i) {
        tarea tFin;
        tFin.tipoTarea = "FIN";
        tFin.cargaDeTrabajo = 0.0;
        
        unique_lock<mutex> lk(mtx);
        while (true) {
            if (!mBT.isNew(i)) {
                mBT.put(tFin, i);
                cv_items_disponibles[i].notify_one();
                break; 
            }
            // Si el buffer de este controlador está lleno, esperamos
            cv_huecos_libres.wait(lk);
        }
    }
}

tarea mtBuffer::obtenerTarea(MultiBuffer<tarea, N_CONTROLLERS> &mBT, int id) {
    unique_lock<mutex> lk(mtx);
    
    // Esperamos hasta que haya una tarea nueva para este controlador (id)
    while (!mBT.isNew(id)) {
        cv_items_disponibles[id].wait(lk);
    }
    
    tarea t = mBT.get(id);
    
    // Al sacar la tarea, queda un hueco libre, avisamos al productor (ponerTarea)
    cv_huecos_libres.notify_one();
    
    return t;
}