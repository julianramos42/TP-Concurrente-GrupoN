#ifndef VRAM_H_INCLUDED
#define VRAM_H_INCLUDED

#include "job.h"

void init_vram();
int ingresar_a_vram(Job& job);
void procesar_y_liberar_vram(Job& job, int slot_asignado);

#endif // VRAM_H_INCLUDED
