#include "../include/emu.h"

#include <pthread.h>
#include <unistd.h>

#include "../include/cart.h"
#include "../include/cpu.h"
#include "../include/ui.h"

/*
 * Emulator Components
 * | CPU |
 * | Cart |
 * | Address Bus |
 * | PPU |
 * | Timer |
 */

static emu_context emu_ctx;

emu_context *emu_get_context() { return &emu_ctx; }

void *cpu_run(void *p) {
    cpu_init();

    // Dijkstra probably hates me
    while (emu_ctx.running) {
        if (emu_ctx.paused) {
            delay(10);
            continue;
        }

        if (!(cpu_step())) {
            DEBUG_PRINT("CPU Stopped\n");
            break;
        }

        emu_ctx.ticks++;
    }

    return 0;
}

int emu_run(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./viniboy <rom_file>\n");
        return -1;
    }

    if (!(cart_load(argv[1]))) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    emu_ctx.paused = false;
    emu_ctx.running = true;
    emu_ctx.ticks = 0;
    emu_ctx.stop = false;

    ui_init();

    pthread_t t1;
    if (pthread_create(&t1, NULL, cpu_run, NULL)) {
        DEBUG_PRINT("FAILED TO START MAIN CPU THREAD\n");
    }

    while (!(emu_ctx.stop)) {
        usleep(1000);
        ui_handle_events();
    }

    ui_exit();

    cart_close();

    return 0;
}

void emu_cycles(int cpu_cycles) { emu_ctx.ticks += cpu_cycles; }
