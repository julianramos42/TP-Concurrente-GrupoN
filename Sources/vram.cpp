#include "vram.h"
#include "semaforo.h"
#include <mutex>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

Semaforo slots_disponibles; // CAPACIDAD TOTAL (5)

// MTX PARA EL CONTROL DE FLUJO Y ACCESOS PEDIDOS EN EL ENUNCIADO
mutex mtx_asignacion; // 450MS
mutex mtx_liberacion; // 250MS

// BUFFER2 (POOL VRAM) SE USA UN ARREGLO PARA MEJORAR LA EFICIENCIA Y QUE LOS 5 ESPACIOS TRABAJEN INDEPENDIENTEMENTE
mutex mtx_slots;
bool slots_vram[5];

void init_vram() { // INICIA EL SEMAFORO CON 5 ESPACIOS Y LOS 5 SLOTS DEL BUFFER EN FALSE
    init(slots_disponibles, 5);
    for(int i = 0; i < 5; i++) {
        slots_vram[i] = false;
    }
}

int ingresar_a_vram(Job& job) {
    wait(slots_disponibles); // ESPERA UN SLOT DISPONIBLE SINO SE BLOQUEA

    // SE BUSCA EN QUE SLOT ASIGNARLO
    mtx_slots.lock();
    int mi_slot = -1;
    int i = 0;

    // SI ES MENOR QUE 5 O SI NO HAY LUGAR
    while (i < 5 && mi_slot == -1) {
        if (!slots_vram[i]) { // SI ES FALSE (ESTA VACIO)
            slots_vram[i] = true;
            mi_slot = i;
        }
        i++;
    }
    mtx_slots.unlock();

    // ASIGNACION
    mtx_asignacion.lock();

    // ACA IRIA EL LOG DE ASIGNADO_VRAM

    sleep_for(milliseconds(450)); // SE GENERA LA ESPERA (ENUNCIADO)
    mtx_asignacion.unlock();

    return mi_slot;
}

void procesar_y_liberar_vram(Job& job, int slot_asignado) {
    // TIEMPO DE CARGA DE ASSETS (600MS)
    sleep_for(milliseconds(600));

    // LIBERACION
    mtx_liberacion.lock();

    // ACA IRIA EL LOG DE FINALIZADO

    sleep_for(milliseconds(250)); // SE GENERA LA ESPERA (ENUNCIADO)
    mtx_liberacion.unlock();

    // SE LIBERA EL SLOT (VUELVE A FALSE)
    mtx_slots.lock();
    slots_vram[slot_asignado] = false;
    mtx_slots.unlock();

    // LIBERA EL PERMISO Y DESPIERTA 1 HILO
    signal(slots_disponibles);
}