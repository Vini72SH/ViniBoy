#include "../include/emu.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../include/cart.h"
#include "../include/cpu.h"

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

void delay(uint32_t ms) { return SDL_Delay(ms); }

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

    SDL_Init(SDL_INIT_VIDEO);
    DEBUG_PRINT("SDL INIT\n");
    TTF_Init();
    DEBUG_PRINT("TTF INIT\n");

    cpu_init();

    emu_ctx.running = true;
    emu_ctx.paused = false;
    emu_ctx.ticks = 0;

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

    SDL_Quit();
    DEBUG_PRINT("SDL QUIT\n");
    TTF_Quit();
    DEBUG_PRINT("TTF INIT\n");

    cart_close();

    return 0;
}

void emu_cycles(int cpu_cycles) { emu_ctx.ticks += cpu_cycles; }
