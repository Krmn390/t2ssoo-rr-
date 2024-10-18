#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "Osrms_File.h"

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
