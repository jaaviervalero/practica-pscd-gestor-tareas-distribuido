#include "librerias/Socket/Socket.hpp"
#include "mtResultados.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <cstdlib> 
#include <sstream> // Para stringstream

using namespace std;

const int N_CONTROLLERS = 10;

void servCliente(int client_fd, mtResultados &mtR) {
    Socket chan(0);
    const int BUFF_SIZE = 100;
    string buffer;
    bool fin = false;

    while (!fin) {
        int rcv = chan.Receive(client_fd, buffer, BUFF_SIZE);
        if (rcv <= 0) break;

        if (buffer == "TF") {
            // El monitor se encarga de esperar a que todos lleguen aquí
            mtR.esperarFinalizacion(N_CONTROLLERS);
            
            chan.Send(client_fd, "END");
            fin = true;
        } else {
            stringstream ss(buffer);
            string tipo, estadoStr;
            float carga;
            getline(ss, tipo, ',');
            getline(ss, estadoStr, ',');
            ss >> carga;

            bool exito = (estadoStr == "OK");
            mtR.anotarResultado(tipo, exito, carga);
        }
    }
}

int main(int argc, char* argv[]) {
    int server_port = 3001; 
    if (argc == 2) server_port = atoi(argv[1]);

    Socket chan(server_port);
    if (chan.Bind() == -1 || chan.Listen() == -1) {
        cerr << "Error inicio servidor matriz" << endl;
        return 1;
    }

    mtResultados mtR; // El monitor gestiona datos y sincronización
    thread threads_clientes[N_CONTROLLERS];

    cout << "Servidor de Matriz escuchando en puerto " << server_port << "..." << endl;

    for (int i = 0; i < N_CONTROLLERS; i++) {
        int fd = chan.Accept();
        if (fd != -1) {
            threads_clientes[i] = thread(servCliente, fd, ref(mtR));
            cout << "Controlador " << i << " conectado." << endl;
        }
    }

    for (int i = 0; i < N_CONTROLLERS; i++) {
        threads_clientes[i].join();
    }

    chan.Close();
    return 0;
}