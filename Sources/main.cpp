#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <chrono>
#include "productor.h"
#include "worker.h"
#include "vram.h"
#include "messageQueue.h"

using namespace std;

int main()
{
    std::ofstream limpiar("sistema.log");
    limpiar.close();
    srand(time(NULL));

    cout << "--- Iniciando Granja de Renderizado ---" << endl;

    init_vram();
    init_queue();
    /*
    Configuración A: 1 Productor y 2 Consumidores (Escenario de baja recepción / alta disponibilidad).
    Configuración B: 3 Productores y 1 Consumidor (Escenario de congestión / cuello de botella).
    Configuración C: 3 Productores y 3 Consumidores (Escenario de alta concurrencia simétrica).
    */
    int num_productores = 3;
    int num_consumidores = 1;
    int total_tareas = num_productores * 10;

    cout << "Escenario configurado:" << endl;
    cout << "- Productores en ejecucion: " << num_productores << endl;
    cout << "- Consumidores (Workers) en ejecucion: " << num_consumidores << endl;
    cout << "- Total de tareas a procesar: " << total_tareas << endl;
    cout << "---------------------------------------" << endl;

    vector<thread> hilos_productores;
    vector<thread> hilos_consumidores;

    for (int i = 0; i < num_productores; i++) {
        hilos_productores.push_back(thread(productor));
    }

    for (int i = 0; i < num_consumidores; i++) {
        hilos_consumidores.push_back(thread(ejecutar_worker, total_tareas));
    }

    for (int i = 0; i < num_productores; i++) {
        hilos_productores[i].join();
    }

    bool terminaron_reales = false;
    while (!terminaron_reales) {
        mtx_contador.lock();
        if (tareas_finalizadas >= total_tareas) {
            terminaron_reales = true;
        }
        mtx_contador.unlock();

        if (!terminaron_reales) {
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    for (int i = 0; i < num_consumidores; i++) {
        Job job_falso;
        job_falso.id = -1;
        job_falso.priority = FREE;
        enqueueJob(job_falso);
    }

    for (int i = 0; i < num_consumidores; i++) {
        hilos_consumidores[i].join();
    }

    cout << "---------------------------------------" << endl;
    cout << "Prueba finalizada con exito." << endl;
    cout << "Contador global de tareas finalizadas: " << tareas_finalizadas << " / " << total_tareas << endl;
    cout << "Revisa el archivo sistema.log para ver el registro completo y ordenado." << endl;

    return 0;
}
