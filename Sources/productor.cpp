#include <iostream>
#include <mutex>
#include <cstdlib>
#include "job.h"
#include "productor.h"
#include "queue.h"

using namespace std;
mutex mtx_id;
//mutex mtx_consola;

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
        j.state = CREATED;
        agregarJob(j);

 //       mtx_consola.lock();
        cout << "Job creado: " << j.id << "Prioridad: " << j.prioridad << endl;
 //       mtx_consola.unlock();
    }
}


























