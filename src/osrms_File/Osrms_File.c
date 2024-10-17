#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "Osrms_File.h"

// Estructura de PCB
typedef struct {
    uint8_t estado; // 1 byte
    uint8_t id_proceso; // 1 byte
    char nombre_proceso[11]; // 11 bytes
    uint8_t tabla_archivos[115]; // 115 bytes
    uint8_t tabla_paginas_1[128]; // 128 bytes
} Entrada_PCB;

// Estructura de archivos
typedef struct {
    uint8_t validez; // 1 byte
    char nombre_archivo[14]; // 14 bytes
    uint32_t tamano_archivo; // 4 bytes
    uint32_t dir_virtual; // 4 bytes
} Entrada_Archivo;

// Estructura de la tabla de archivos
typedef struct {
    Entrada_Archivo entradas[5]; // 5 entradas de 23 bytes cada una
} Tabla_Archivos;

// Tabla de páginas de primer orden
typedef struct {
    uint16_t entradas[64]; // 64 entradas de 2 bytes
} Tabla_Paginas_Primer_Orden;

// Estructura para las tablas de segundo orden
typedef struct {
    uint16_t entradas[64]; // 64 entradas de 2 bytes
} Tabla_Paginas_Segundo_Orden;

// Bitmap de tablas de páginas
typedef struct {
    uint8_t bits[128]; // 128 bytes = 1024 bits
} Bitmap_Tablas_Paginas;

// Frame bitmap
typedef struct {
    uint8_t bits[8192]; // 8 KB = 65536 bits
} Frame_Bitmap;

// Frames (datos)
typedef struct {
    uint8_t data[32768]; // 32 KB por frame
} Frame;

// Función para calcular la dirección física a partir de la dirección virtual
uint32_t get_physical_address(Tabla_Archivos *tabla_a, Tabla_Paginas_Primer_Orden *prim_orden, Tabla_Paginas_Segundo_Orden *seg_orden) {
    uint32_t virtual_address = tabla_a->entradas[0].dir_virtual;
    uint32_t vpn = (virtual_address >> 15) & 0xFFF;
    uint32_t offset = virtual_address & 0x7FFF;

    uint32_t entrada_prim_orden = (vpn >> 6) & 0x3F;
    uint16_t sptn = prim_orden->entradas[entrada_prim_orden];

    uint32_t entrada_seg_orden = vpn & 0x3F;
    uint16_t pfn = seg_orden->entradas[entrada_seg_orden];

    return (pfn << 15) | offset;
}
