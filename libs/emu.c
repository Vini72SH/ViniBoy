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

static emu_context ctx;

emu_context *emu_get_context() { return &ctx; }

void delay(uint32_t ms) { SDL_Delay(ms); }

int emu_run(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./viniboy <rom_file>\n");
        return -1;
    }

    if (!(cart_load(argv[1]))) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    DEBUG_PRINT("Cart Loaded..\n");

    SDL_Init(SDL_INIT_VIDEO);
    DEBUG_PRINT("SDL INIT\n");
    TTF_Init();
    DEBUG_PRINT("TTF INIT\n");

    cpu_init();

    ctx.running = true;
    ctx.paused = false;
    ctx.ticks = 0;

    // Dijkstra probably hates me
    while (ctx.running) {
        if (ctx.paused) {
            delay(10);
            continue;
        }

        if (!(cpu_step())) {
            printf("CPU Stopped\n");
            return -3;
        }

        ctx.ticks++;
    }

    return 0;
}