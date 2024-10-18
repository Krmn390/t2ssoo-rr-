#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../osrms_File/Osrms_File.h"
#include "./osrms_API.h"

// Variable global para el archivo de memoria
FILE *memoria_file = NULL;

void os_mount(char *memory_path) {
    memoria_file = fopen(memory_path, "r+b"); // Abrir en modo lectura/escritura binario
    if (!memoria_file) {
        perror("Error al abrir el archivo de memoria");
        exit(EXIT_FAILURE);
    }
}

void os_ls_processes() {
    fseek(memoria_file, 0, SEEK_SET); // La Tabla de PCBs comienza en el offset 0
    for (int i = 0; i < 32; i++) {
        Entrada_PCB pcb;
        fread(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
        if (pcb.estado == 0x01) {
            printf("ID: %d, Nombre: %s\n", pcb.id_proceso, pcb.nombre_proceso);
        }
    }
}

int os_exists(int process_id, char *file_name) {
    fseek(memoria_file, 0, SEEK_SET);
    Entrada_PCB pcb;
    for (int i = 0; i < 32; i++) {
        fread(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
        if (pcb.id_proceso == process_id) {
            Entrada_Archivo tabla_archivos[5];
            memcpy(tabla_archivos, pcb.tabla_archivos, 115);
            for (int j = 0; j < 5; j++) {
                if (tabla_archivos[j].validez == 0x01 && strcmp(tabla_archivos[j].nombre_archivo, file_name) == 0) {
                    return 1; // El archivo existe
                }
            }
            return 0; // El archivo no existe
        }
    }
    return 0; // Proceso no existe
}

void os_ls_files(int process_id) {
    fseek(memoria_file, 0, SEEK_SET);
    Entrada_PCB pcb;
    for (int i = 0; i < 32; i++) {
        fread(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
        if (pcb.id_proceso == process_id) {
            Entrada_Archivo tabla_archivos[5];
            memcpy(tabla_archivos, pcb.tabla_archivos, 115);
            printf("Archivos del proceso %d:\n", process_id);
            for (int j = 0; j < 5; j++) {
                if (tabla_archivos[j].validez == 0x01) {
                    printf("Archivo: %s, Tamaño: %u Bytes\n",
                           tabla_archivos[j].nombre_archivo,
                           tabla_archivos[j].tamano_archivo);
                }
            }
            return;
        }
    }
    printf("Proceso %d no encontrado\n", process_id);
}

void os_frame_bitmap() {
    fseek(memoria_file, 8192 + 128 + 131072, SEEK_SET); // Offset del Frame Bitmap
    Frame_Bitmap bitmap;
    fread(&bitmap, sizeof(Frame_Bitmap), 1, memoria_file);

    int libres = 0, ocupados = 0;
    for (int i = 0; i < 65536; i++) {
        if (bitmap.bits[i / 8] & (1 << (i % 8))) {
            ocupados++;
        } else {
            libres++;
        }
    }
    printf("Frames ocupados: %d, Frames libres: %d\n", ocupados, libres);
}

void os_tp_bitmap() {
    fseek(memoria_file, 8192, SEEK_SET); // Offset del Bitmap de Tablas de Páginas
    Bitmap_Tablas_Paginas bitmap;
    fread(&bitmap, sizeof(Bitmap_Tablas_Paginas), 1, memoria_file);

    int ocupadas = 0, libres = 0;
    for (int i = 0; i < 128; i++) {
        if (bitmap.bits[i / 8] & (1 << (i % 8))) {
            ocupadas++;
        } else {
            libres++;
        }
    }
    printf("Tablas de páginas ocupadas: %d, Tablas de páginas libres: %d\n", ocupadas, libres);
}

void os_start_process(int process_id, char *process_name) {
    fseek(memoria_file, 0, SEEK_SET);
    Entrada_PCB pcb;
    for (int i = 0; i < 32; i++) {
        fread(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
        if (pcb.estado == 0x00) { // Entrada libre
            pcb.estado = 0x01;
            pcb.id_proceso = process_id;
            strncpy(pcb.nombre_proceso, process_name, 10); // Máximo 10 caracteres
            memset(pcb.tabla_archivos, 0, 115); // Inicializar tabla de archivos
            memset(pcb.tabla_paginas_1, 0, 128); // Inicializar tabla de páginas
            fseek(memoria_file, i * sizeof(Entrada_PCB), SEEK_SET);
            fwrite(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
            printf("Proceso %s iniciado con ID %d\n", process_name, process_id);
            return;
        }
    }
    printf("No hay espacio para iniciar el proceso %s\n", process_name);
}

void os_finish_process(int process_id) {
    fseek(memoria_file, 0, SEEK_SET);
    Entrada_PCB pcb;
    for (int i = 0; i < 32; i++) {
        long position = ftell(memoria_file);
        fread(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
        if (pcb.id_proceso == process_id) {
            pcb.estado = 0x00;
            fseek(memoria_file, position, SEEK_SET);
            fwrite(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
            printf("Proceso %d terminado\n", process_id);
            return;
        }
    }
    printf("Proceso %d no encontrado\n", process_id);
}

osrmsFile* os_open(int process_id, char* file_name, char mode) {
    if (mode != 'r' && mode != 'w') {
        return NULL;
    }
    fseek(memoria_file, 0, SEEK_SET);
    Entrada_PCB pcb;
    for (int i = 0; i < 32; i++) {
        long position = ftell(memoria_file);
        fread(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
        if (pcb.id_proceso == process_id) {
            Entrada_Archivo tabla_archivos[5];
            memcpy(tabla_archivos, pcb.tabla_archivos, 115);
            for (int j = 0; j < 5; j++) {
                if (tabla_archivos[j].validez == 0x01 && strcmp(tabla_archivos[j].nombre_archivo, file_name) == 0) {
                    if (mode == 'r') {
                        osrmsFile* file = malloc(sizeof(osrmsFile));
                        file->process_id = process_id;
                        strcpy(file->file_name, file_name);
                        file->mode = mode;
                        file->size = tabla_archivos[j].tamano_archivo;
                        file->virtual_address = tabla_archivos[j].dir_virtual;
                        return file;
                    } else {
                        return NULL;
                    }
                }
            }
            if (mode == 'w') {
                for (int j = 0; j < 5; j++) {
                    if (tabla_archivos[j].validez == 0x00) {
                        tabla_archivos[j].validez = 0x01;
                        strncpy(tabla_archivos[j].nombre_archivo, file_name, 14);
                        tabla_archivos[j].tamano_archivo = 0;
                        tabla_archivos[j].dir_virtual = 0;
                        memcpy(pcb.tabla_archivos, tabla_archivos, 115);
                        fseek(memoria_file, position, SEEK_SET);
                        fwrite(&pcb, sizeof(Entrada_PCB), 1, memoria_file);
                        osrmsFile* file = malloc(sizeof(osrmsFile));
                        file->process_id = process_id;
                        strcpy(file->file_name, file_name);
                        file->mode = mode;
                        file->size = 0;
                        file->virtual_address = 0;
                        return file;
                    }
                }
                printf("No hay espacio en la Tabla de Archivos para el proceso %d\n", process_id);
                return NULL;
            }
        }
    }
    return NULL;
}

int os_write_file(osrmsFile* file_desc, char* src) {
    if (file_desc->mode != 'w') {
        printf("El archivo no está abierto en modo escritura.\n");
        return -1;
    }
    FILE *src_file = fopen(src, "rb");
    if (!src_file) {
        perror("Error al abrir el archivo fuente");
        return -1;
    }
    fseek(src_file, 0, SEEK_END);
    uint32_t size = ftell(src_file);
    fseek(src_file, 0, SEEK_SET);
    if (size > 67108864) {
        printf("El archivo excede el tamaño máximo permitido (64 MB).\n");
        fclose(src_file);
        return -1;
    }
    fclose(src_file);
    return size;
}
