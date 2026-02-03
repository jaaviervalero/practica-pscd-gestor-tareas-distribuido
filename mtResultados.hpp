#ifndef MT_RESULTADOS_HPP
#define MT_RESULTADOS_HPP

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>

using namespace std;

class mtResultados {
    struct Estadisticas {
        unsigned int numEjecutadas = 0;
        unsigned int numExito = 0;
        float tiempoTotalExito = 0.0;
    };

    public:
        mtResultados(); 
        ~mtResultados(); 

        void anotarResultado(string tipoTarea, bool exito, float carga);
        
        // Nuevo método para gestionar la sincronización final
        void esperarFinalizacion(int totalClientes);

    private:
        Estadisticas stats[3]; 
        mutex mtx;
        condition_variable cv_fin; // Para esperar a que todos acaben
        int clientesFinalizados = 0; // Contador interno
        
        int indicePorTipo(string tipo);
        void imprimirResultados(); // Ahora es privado, se llama al acabar
};

#endif