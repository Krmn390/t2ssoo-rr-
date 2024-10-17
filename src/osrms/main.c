#include "../osrms_API/osrms_API.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
    // Montar la memoria
    os_mount((char *)argv[1]);

    // Probar las funciones
    os_ls_processes();
    os_start_process(1, "ProcesoA");
    os_ls_processes();
    os_ls_files(1);
    os_frame_bitmap();
    os_tp_bitmap();
    os_finish_process(1);
    os_ls_processes();

    return 0;
}
