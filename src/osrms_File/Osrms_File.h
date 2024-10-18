#pragma once
#include <stdint.h>

// Definición de la estructura PCB
typedef struct {
    uint8_t estado;
    uint8_t id_proceso;
    char nombre_proceso[11];
    uint8_t tabla_archivos[115];
    uint8_t tabla_paginas_1[128];
} Entrada_PCB;

// Definición de la estructura de archivos
typedef struct {
    uint8_t validez;
    char nombre_archivo[14];
    uint32_t tamano_archivo;
    uint32_t dir_virtual;
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

// Función para obtener la dirección física
uint32_t get_physical_address();
