#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> 
#include "osrms_API.h"



typedef struct tabla_pcbs{
    Entrada entradas[32];
} Tabla_PCBs;


typedef struct entrada{
    bool estado;
    char entrada_id[1];
    char nombre_proc[11];
    char tabla_archs[115];
    char tabla_pags[118];
} Entrada;

typedef struct tabla_archs{
    char entrada_archs[5];
    bool validez;
    char nombre_arch[14];
    uint32_t tamano;
    char virtual_dir[4]
} Tabla_Arch;

typedef struct tabla_pag_1_ord{
    int16_t entrada_pag_prim_ord[64];
} Tabla_Pag_Prim_Ord;

typedef struct espacio_tablas_2do_orden{
    unsigned tamano; //128000 bytes = 128KB
    char tabla_pagina_2do_orden; // 128 bytes
} Espacio_Tablas_2do_Orden;


typedef struct bit_map_tabla{
    bool bitmap[128] ; // libre false, ocupada true
} Bit_map;

typedef struct tabla_pags_2do_orden{   // Relativas a los últimos 2Gb de memoria 
    int dir;  // La dirección absoluta es 8 * 2^10 + 128 + 128 * 2^10 + 8 * 2^10 + dir 
    uint16_t entradas_2do_orden[64];
} Tabla_Pags_2do_Orden;


