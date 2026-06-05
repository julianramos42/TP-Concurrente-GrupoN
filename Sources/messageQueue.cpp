#include <iostream>
#include <queue>
#include <string.h>
#include <mutex>
#include <thread>
#include "messageQueue.h"
#include "job.h"

std::vector<Job> messageQueue;
std::mutex mtxQueue;
int MAX_PREMIUMS_SEGUIDOS = 8;
int PREMIUMS_LIBERADOS = 0;



void enqueueJob(Job job)
{
    mtxQueue.lock();

    messageQueue.push_back(job);

    //LOG - ENCOLADO CON EXITO.


    mtxQueue.unlock();

}

Job dequeueJob()
{
    Job j;
    mtxQueue.lock();

    if(messageQueue.empty())
    {
        //LOG - QUEUE VACIA
        mtxQueue.unlock();

        j = createJob("Exception");
        j.id = -1;
        return j;
    }

    //LOG - DESENCOLANDO...

    if(PREMIUMS_LIBERADOS < MAX_PREMIUMS_SEGUIDOS)
    {
        int posicion = buscarPrioridad("PREMIUM");
        j = liberarJob(posicion);

        mtxQueue.unlock();

        if(posicion != -1)
        {
          PREMIUMS_LIBERADOS ++;
        }

    }
    else
    {
        int posicion = buscarPrioridad("FREE");
        j = liberarJob(posicion);

        mtxQueue.unlock();

        PREMIUMS_LIBERADOS = 0;
    }

    return j;
}

Job liberarJob(int posicion)
{
    Job jobLiberar;

    if(posicion != -1)
    {

        jobLiberar = messageQueue[posicion];
        messageQueue.erase(messageQueue.begin()+ posicion);

    }
    else
    {
        jobLiberar = messageQueue.front();
        messageQueue.erase(messageQueue.begin());

    }

    return jobLiberar;

}

int buscarPrioridad(char prioridad[])
{
    for(int i = 0; i < messageQueue.size(); i++)
    {
        if(strcmp(messageQueue[i].priority, prioridad) == 0)
        {

            return i;
        }
    }

    return -1;

}
