#ifndef EMU_H
#define EMU_H

#include "common.h"

/*
 * Emulator Context
 * This structure holds the state of the emulator, including whether it is
 * paused, running, and the number of ticks that have occurred. It is used to
 * manage the emulator's execution flow and timing.
 */
typedef struct {
    bool paused;
    bool running;
    uint64_t ticks;
} emu_context;

emu_context *emu_get_context();

int emu_run(int argc, char **argv);

void emu_cycles(int cpu_cycles);

#endif  // EMU_H