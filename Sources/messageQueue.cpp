#include <iostream>
#include <vector>
#include <mutex>
#include <ctime>
#include "messageQueue.h"
#include "job.h"
#include "logger.h"
#include "semaforo.h"

std::vector<Job> messageQueue;
std::mutex mtxQueue;

Semaforo items_cola;
int MAX_PREMIUMS_SEGUIDOS = 8;
int PREMIUMS_LIBERADOS = 0;

void init_queue() {
    init(items_cola, 0);
}

void enqueueJob(Job job) {
    job.enqueueTime = time(0);

    mtxQueue.lock();
    messageQueue.push_back(job);

    if (job.id != -1) {
        loggear_evento(job, "EN_COLA");
    }
    mtxQueue.unlock();

    signal(items_cola);
}

Job dequeueJob() {
    wait(items_cola);

    mtxQueue.lock();
    Job j;

    if (messageQueue.front().id == -1) {
        j = messageQueue.front();
        messageQueue.erase(messageQueue.begin());
        mtxQueue.unlock();
        return j;
    }

    time_t ahora = time(0);
    int pos_free = buscarPrioridad(FREE);

    if (pos_free != -1 && (ahora - messageQueue[pos_free].enqueueTime) >= 5) {
        j = liberarJob(pos_free);
        loggear_evento(j, "DESENCOLANDO");
        mtxQueue.unlock();
        return j;
    }

    if (PREMIUMS_LIBERADOS < MAX_PREMIUMS_SEGUIDOS) {
        int pos_premium = buscarPrioridad(PREMIUM);
        if (pos_premium != -1) {
            j = liberarJob(pos_premium);
            loggear_evento(j, "DESENCOLANDO");
            PREMIUMS_LIBERADOS++;
        } else {
            j = liberarJob(buscarPrioridad(FREE));
            loggear_evento(j, "DESENCOLANDO");
            PREMIUMS_LIBERADOS = 0;
        }
    } else {
        j = liberarJob(buscarPrioridad(FREE));
        loggear_evento(j, "DESENCOLANDO");
        PREMIUMS_LIBERADOS = 0;
    }

    mtxQueue.unlock();
    return j;
}

Job liberarJob(int posicion) {
    Job jobLiberar;
    if(posicion != -1) {
        jobLiberar = messageQueue[posicion];
        messageQueue.erase(messageQueue.begin() + posicion);
    } else {
        jobLiberar = messageQueue.front();
        messageQueue.erase(messageQueue.begin());
    }
    return jobLiberar;
}

int buscarPrioridad(Priority prioridad) {
    for(int i = 0; i < messageQueue.size(); i++) {
        if(messageQueue[i].priority == prioridad) {
            return i;
        }
    }
    return -1;
}
