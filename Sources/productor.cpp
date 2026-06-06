#include <iostream>
#include <mutex>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include "job.h"
#include "productor.h"
#include "messageQueue.h"
#include "logger.h"

using namespace std;
mutex mtx_id;
mutex mtx_consola;

int ultimoId = 1;

void productor(){
    for(int i=0; i<10; i++){
        Job j;
        mtx_id.lock();
        j.id = ultimoId;
        ultimoId++;
        mtx_id.unlock();

        if(rand() % 2 == 0){
            j.priority = FREE;
        }else{
            j.priority = PREMIUM;
        }
        loggear_evento(j, "CREADO");
        this_thread::sleep_for(chrono::milliseconds(100));
        enqueueJob(j);

        mtx_consola.lock();
        cout << "Job creado: " << j.id << " - Prioridad: " << j.priority << endl;
        mtx_consola.unlock();
    }
}
