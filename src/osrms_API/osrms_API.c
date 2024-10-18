#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../osrms_File/Osrms_File.c"



typedef struct memoria {
    Tabla_PCBs tabla_pcbs;
    Bit_map bitmap_tablas_paginas;
    Espacio_Tablas_2do_Orden espacio_tablas_2do_orden;
    Frame_Bitmap frame_bitmap;
    Frames frames[65536]; // 216 frames, cada uno de 32 KB
} Memoria;


Memoria memoria;

char global_memory_path[256]; // Tamaño máximo de la ruta

// Función para montar la memoria, establece la ruta como global
void os_mount(char *memory_path) {
    // Copiamos la ruta de memory_path a la variable global
    strncpy(global_memory_path, memory_path, sizeof(global_memory_path) - 1);
    
    global_memory_path[sizeof(global_memory_path) - 1] = '\0'; 

    printf("Memoria montada en: %s\n", global_memory_path);
}


void os_ls_process(){
    for (int i= 0; i< )

}