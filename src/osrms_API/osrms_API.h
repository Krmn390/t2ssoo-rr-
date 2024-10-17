#pragma once
#include "../osrms_File/Osrms_File.h"

typedef struct {
    int process_id;          // ID del proceso que posee el archivo
    char file_name[14];      // Nombre del archivo (máximo 14 caracteres)
    char mode;               // Modo de apertura ('r' o 'w')
    uint32_t size;           // Tamaño del archivo
    uint32_t virtual_address; // Dirección virtual del archivo
} osrmsFile;

// Funciones de la API
void os_mount(char *memory_path);
void os_ls_processes();
int os_exists(int process_id, char *file_name);
void os_ls_files(int process_id);
void os_frame_bitmap();
void os_tp_bitmap();
void os_start_process(int process_id, char *process_name);
void os_finish_process(int process_id);
osrmsFile* os_open(int process_id, char* file_name, char mode);
int os_write_file(osrmsFile* file_desc, char* src);
