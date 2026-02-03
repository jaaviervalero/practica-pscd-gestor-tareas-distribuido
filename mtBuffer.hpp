#ifndef MT_BUFFER_HPP
#define MT_BUFFER_HPP

#include <mutex>
#include <condition_variable>
#include <string>
#include "librerias/MultiBuffer/MultiBuffer.hpp" // Necesario para que reconozca MultiBuffer

using namespace std;

// Definimos la constante aquí para que esté disponible
const int N_CONTROLLERS = 10;

// Definimos la estructura 'tarea' FUERA de la clase para que sea pública y global.
// Esto soluciona los problemas de visibilidad en otros archivos.
struct tarea {
    string tipoTarea;
    float cargaDeTrabajo;
};

class mtBuffer {
    public:
        mtBuffer(); 
        ~mtBuffer();
        
        // Pasamos 'MultiBuffer' por referencia (&) para poder modificarlo.
        // Si lo pasas por valor, estarías modificando una copia local.
        void ponerTarea(MultiBuffer<tarea, N_CONTROLLERS> &mBT); 
        void ponerFinTareas(MultiBuffer<tarea, N_CONTROLLERS> &mBT); 
        
        // El id es int para coincidir con la implementación
        tarea obtenerTarea(MultiBuffer<tarea, N_CONTROLLERS> &mBT, int id); 

    private:
        mutex mtx; // Nombre estándar para el mutex
        condition_variable cv_huecos_libres;
        condition_variable cv_items_disponibles[N_CONTROLLERS];
};

#endif