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
    entrada_prim_ord[64]
    
}