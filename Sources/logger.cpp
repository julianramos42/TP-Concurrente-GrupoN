#include "logger.h"
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

mutex mtx_log;

void loggear_evento(Job job, string evento) {
    mtx_log.lock();

    ofstream archivo("sistema.log", ios::app);

    time_t ahora = time(0);
    tm *tiempo = localtime(&ahora);

    string prioridad_str;
    if (job.priority == PREMIUM) {
        prioridad_str = "PREMIUM";
    } else {
        prioridad_str = "FREE";
    }

    if (archivo.is_open()) {
        archivo << "[" << tiempo->tm_hour << ":" << tiempo->tm_min << ":" << tiempo->tm_sec << "] - "
                << "Job ID: " << job.id
                << " - Prioridad: " << prioridad_str
                << " - Evento: " << evento << "\n";

        archivo.close();
    }

    mtx_log.unlock();
}
