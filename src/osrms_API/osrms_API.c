#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct tabla_pcbs {
    Entrada entradas[32];
} Tabla_PCBs;

typedef struct entrada {
    uint8_t estado; //1 byte 
    uint8_t entrada_id; // 1b yte 
    char nombre_proc[11];
    char tabla_archs[115];
    char tabla_pags[118];
} Entrada;

typedef struct tabla_archs {
    char entrada_archs[5];
    bool validez;
    char nombre_arch[14];
    uint32_t tamano;
    uint32_t virtual_dir; // Usamos uint32_t para manejar 32 bits directamente
} Tabla_Arch;

typedef struct tabla_pag_1_ord {
    int16_t entrada_pag_prim_ord[64];
} Tabla_Pag_Prim_Ord;

typedef struct espacio_tablas_2do_orden {
    unsigned tamano; // 128000 bytes = 128KB
    char tabla_pagina_2do_orden; // 128 bytes
} Espacio_Tablas_2do_Orden;

typedef struct bit_map_tabla {
    uint8_t bitmap[16]; // lo que da 128 bits  (8 * 16 = 128 bits)
} Bit_map;

typedef struct tabla_pags_2do_orden { // Relativas a los últimos 2Gb de memoria
    int dir; // La dirección absoluta es 8 * 2^10 + 128 + 128 * 2^10 + 8 * 2^10 + dir
    uint16_t entradas_2do_orden[64];
} Tabla_Pags_2do_Orden;

typedef struct frame_bitmap {
    bool bit_on_use[65536]; //0 si está libre, 1 si está siendo ocupado
} Frame_Bitmap;     

typedef struct frames{
    unsigned int size; //32 KB , Se encuentran el los últimos 2 GB  y  hay 2^16  franes 
} Frames; 


// Función para extraer la dirección física a partir de la dirección virtual
uint32_t get_physical_address(Tabla_Arch tabla_a, Tabla_Pag_Prim_Ord *prim_orden, Tabla_Pags_2do_Orden *seg_orden) {
    // Dirección virtual de 32 bits almacenada en tabla_a.virtual_dir
    uint32_t virtual_address = tabla_a.virtual_dir;

    // Extraemos las diferentes partes de la dirección virtual
    uint32_t primera_parte = (virtual_address >> 27) & 0x1F;  // 5 bits más significativos (Ej. Primer índice)
    uint32_t vpn = (virtual_address >> 15) & 0xFFF;            // Los siguientes 12 bits (VPN)
    uint32_t offset = virtual_address & 0x7FFF;                // Los últimos 15 bits (offset)

    // Primeros 6 bits del VPN para obtener la entrada en la Tabla de Páginas de Primer Orden
    uint32_t entrada_prim_orden = (vpn >> 6) & 0x3F;
    uint16_t sptn = prim_orden->entrada_pag_prim_ord[entrada_prim_orden];

    // Últimos 6 bits del VPN para la entrada en la Tabla de Páginas de Segundo Orden
    uint32_t entrada_seg_orden = vpn & 0x3F;
    uint16_t pfn = seg_orden->entradas_2do_orden[entrada_seg_orden];

    // La dirección física es el PFN seguido del offset
    uint32_t physical_address = (pfn << 15) | offset;

    // Imprimir las partes y la dirección física
    printf("VPN: %u\n", vpn);
    printf("Offset: %u\n", offset);
    printf("PFN: %u\n", pfn);
    printf("Dirección Física: %u\n", physical_address);

    return physical_address;
}

int main() {
    // Ejemplo de uso
    Tabla_Arch tabla_arch = { .virtual_dir = 0x12345678 };
    Tabla_Pag_Prim_Ord prim_orden = { .entrada_pag_prim_ord = {0} };
    Tabla_Pags_2do_Orden seg_orden = { .entradas_2do_orden = {0} };

    // Supongamos que llenamos las tablas con valores de ejemplo
    prim_orden.entrada_pag_prim_ord[0] = 42; // Ejemplo de Tabla de Primer Orden
    seg_orden.entradas_2do_orden[0] = 84;   // Ejemplo de Tabla de Segundo Orden

    // Obtener la dirección física
    uint32_t direccion_fisica = get_physical_address(tabla_arch, &prim_orden, &seg_orden);

    return 0;
}
