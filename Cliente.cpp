#include "Socket.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib> 
#include <string>
#include <sstream>

using namespace std;


bool ejecutarTarea(string tipo, float carga) {
   
    int tasaFallo = 0;
    if (tipo == "t1") tasaFallo = 5;
    else if (tipo == "t2") tasaFallo = 7;
    else if (tipo == "t3") tasaFallo = 10;

    int r = rand() % 100;
    bool exito = (r >= tasaFallo); 

    int ms = (int)(carga * 10); 
    this_thread::sleep_for(chrono::milliseconds(ms));

    return exito;
}

void controllerTask(int id, string ip_tareas, int port_tareas, string ip_matriz, int port_matriz) {
    srand(time(NULL) + id);    
    Socket sockTareas(ip_tareas, port_tareas);
    int fdTareas = sockTareas.Connect();
    while (fdTareas == -1) {
        this_thread::sleep_for(chrono::seconds(1));
        fdTareas = sockTareas.Connect();
    }

    Socket sockMatriz(ip_matriz, port_matriz);
    int fdMatriz = sockMatriz.Connect();
    while (fdMatriz == -1) {
        this_thread::sleep_for(chrono::seconds(1));
        fdMatriz = sockMatriz.Connect();
    }

    const int BUFF_SIZE = 100;
    string buffer;
    bool final = false;

    while (!final) {
        // Limpiamos el buffer antes de recibir para evitar basura
        buffer = "";

        sockTareas.Send(fdTareas, "GET_TASK");
        int n = sockTareas.Receive(fdTareas, buffer, BUFF_SIZE);
        
        if (n <= 0) break;

        if (buffer == "TF") {
            sockMatriz.Send(fdMatriz, "TF");
            sockMatriz.Receive(fdMatriz, buffer, BUFF_SIZE); 
            sockTareas.Send(fdTareas, "END");
            final = true;
        } else {
            // Parseo del mensaje recibido
            stringstream ss(buffer);
            string tipo;
            float carga;
            getline(ss, tipo, ',');
            ss >> carga;

            // Ejecución de la tarea
            bool exito = ejecutarTarea(tipo, carga);

            // Envío de resultados al Servidor Matriz
            string estado = exito ? "OK" : "FAIL";
            string mensajeResultado = tipo + "," + estado + "," + to_string(carga);
            sockMatriz.Send(fdMatriz, mensajeResultado);
        }
    }
    sockTareas.Close();
    sockMatriz.Close();
}
int main(int argc, char* argv[]) {
    string ipT = "localhost"; int portT = 3000;
    string ipM = "localhost"; int portM = 3001;

    if (argc == 5) {
        ipT = argv[1]; portT = atoi(argv[2]);
        ipM = argv[3]; portM = atoi(argv[4]);
    }

    const int N = 10; 
    thread controladores[N];
    
    cout << "Iniciando " << N << " controladores..." << endl;

    for (int i = 0; i < N; ++i) {
        controladores[i] = thread(controllerTask, i, ipT, portT, ipM, portM);
    }

    for (int i = 0; i < N; ++i) {
        if (controladores[i].joinable()) {
            controladores[i].join();
        }
    }
    cout << "Todos los clientes han terminado." << endl;
    return 0;
}