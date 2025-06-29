#include "../include/emu.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "../include/cart.h"
#include "../include/cpu.h"

static emu_context ctx;

/*
 * Emulator Components
 * | CPU |
 * | Cart |
 * | Address Bus |
 * | PPU |
 * | Timer
 */