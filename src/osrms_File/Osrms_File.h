#pragma once
#include <stdint.h>

// Estructuras de PCB y archivos
typedef struct {
    uint8_t estado;
    uint8_t id_proceso;
    char nombre_proceso[11];
    uint8_t tabla_archivos[115];
    uint8_t tabla_paginas_1[128];
} Entrada_PCB;

typedef struct {
    uint8_t validez;
    char nombre_archivo[14];
    uint32_t tamano_archivo;
    uint32_t dir_virtual;
} Entrada_Archivo;

// Función para obtener la dirección física
uint32_t get_physical_address();
