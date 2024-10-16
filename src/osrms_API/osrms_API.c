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
// Función para montar la memoria
void  os_mount (char * memory_path) {
    // Abre el archivo en modo binario
    FILE *file = fopen(memory_path, "rb");
    if (!file) {
        perror("Error al abrir el archivo");
        return false;
    }

    // Leer la Tabla de PCBs
    fread(&memoria.tabla_pcbs, sizeof(Tabla_PCBs), 1, file);

    // Leer el Bitmap de Tablas de Páginas
    fread(&memoria.bitmap_tablas_paginas, sizeof(Bit_map), 1, file);

    // Leer el Espacio de Tablas de Segundo Orden
    fread(&memoria.espacio_tablas_2do_orden, sizeof(Espacio_Tablas_2do_Orden), 1, file);

    // Leer el Frame Bitmap
    fread(&memoria.frame_bitmap, sizeof(Frame_Bitmap), 1, file);

    // Leer los Frames (216 frames de 32 KB)
    for (int i = 0; i < 65536; i++) {
        fread(&memoria.frames[i], sizeof(Frames), 1, file);
    }

    // Cierra el archivo
    fclose(file);
}

