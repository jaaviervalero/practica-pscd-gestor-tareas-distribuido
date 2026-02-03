#include "librerias/Socket/Socket.hpp"
#include <iostream>
#include <thread>
#include <string>
#include "mtBuffer.hpp"
#include "librerias/MultiBuffer/MultiBuffer.hpp"

using namespace std;

void masterTask(MultiBuffer<tarea, N_CONTROLLERS> &mBT, mtBuffer &mtB) {
    mtB.ponerTarea(mBT);      
    mtB.ponerFinTareas(mBT);  
}

void servCliente(int client_fd, MultiBuffer<tarea, N_CONTROLLERS> &mBT, mtBuffer &mtB, int id) {
    Socket chan(0); 
    const int BUFF_SIZE = 100;
    string buffer;
    bool fin = false;

    while (!fin) {
        int rcv_bytes = chan.Receive(client_fd, buffer, BUFF_SIZE);
        if (rcv_bytes <= 0) break; 

        if (buffer == "GET_TASK") {
            tarea t = mtB.obtenerTarea(mBT, id);
            
            string response;
            if (t.tipoTarea == "FIN") {
                response = "TF";
            } else {
                response = t.tipoTarea + "," + to_string(t.cargaDeTrabajo);
            }

            chan.Send(client_fd, response);

            if (response == "TF") {
                chan.Receive(client_fd, buffer, BUFF_SIZE);
                if (buffer == "END") {
                    fin = true; 
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int server_port = 3000; 
    if (argc == 2) server_port = atoi(argv[1]);

    Socket chan(server_port);
    if (chan.Bind() == -1 || chan.Listen() == -1) {
        cerr << "Error en inicio servidor tareas" << endl;
        return 1;
    }

    MultiBuffer<tarea, N_CONTROLLERS> mBT;
    mtBuffer mtB;
    
    // Array estático de threads
    thread threads_clientes[N_CONTROLLERS];

    cout << "Servidor de Tareas escuchando en puerto " << server_port << "..." << endl;

    for (int i = 0; i < N_CONTROLLERS; i++) {
        int client_fd = chan.Accept();
        if (client_fd != -1) {
            threads_clientes[i] = thread(servCliente, client_fd, ref(mBT), ref(mtB), i);
            cout << "Controlador " << i << " conectado." << endl;
        }
    }

    thread master(masterTask, ref(mBT), ref(mtB));

    master.join();
    for (int i = 0; i < N_CONTROLLERS; i++) {
        threads_clientes[i].join();
    }

    chan.Close();
    cout << "Servidor de Tareas finalizado." << endl;
    return 0;
}