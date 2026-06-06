#ifndef MESSAGEQUEUE_H_INCLUDED
#define MESSAGEQUEUE_H_INCLUDED

#include <queue>
#include <mutex>
#include "job.h"


extern std::vector<Job> messageQueue;
extern std::mutex mtxQueue;

void init_queue();

void enqueueJob(Job job);

Job dequeueJob();

int buscarPrioridad(Priority prioridad);

Job liberarJob(int posicion);


#endif // MESSAGEQUEUE_H_INCLUDED
