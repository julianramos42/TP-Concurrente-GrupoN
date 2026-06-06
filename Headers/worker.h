#ifndef WORKER_H_INCLUDED
#define WORKER_H_INCLUDED

#include <mutex>

extern int tareas_finalizadas;
extern std::mutex mtx_contador;

void ejecutar_worker(int total_tareas_esperadas);

#endif
