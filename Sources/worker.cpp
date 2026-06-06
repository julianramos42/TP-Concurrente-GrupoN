#include "worker.h"
#include "vram.h"
#include "job.h"
#include "messageQueue.h"
#include <mutex>
#include <iostream>

using namespace std;

int tareas_finalizadas = 0;
mutex mtx_contador;
extern mutex mtx_consola;

void ejecutar_worker(int total_tareas_esperadas) {
    bool trabajando = true;

    while (trabajando) {

        mtx_contador.lock();
        if (tareas_finalizadas >= total_tareas_esperadas) {
            trabajando = false;
        }
        mtx_contador.unlock();

        if (trabajando) {
            Job job = dequeueJob();

            if (job.id != -1) {
                int slot = ingresar_a_vram(job);
                procesar_y_liberar_vram(job, slot);

                mtx_contador.lock();
                tareas_finalizadas++;
                int loc_fin = tareas_finalizadas;
                mtx_contador.unlock();

                mtx_consola.lock();
                cout << "-> Worker finalizo Job ID: " << job.id << " (Total: " << loc_fin << ")" << endl;
                mtx_consola.unlock();
            }
        }
    }
}
