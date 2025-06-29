#ifndef EMU_H
#define EMU_H

#include "common.h"

typedef struct {
    int paused;
    int running;
    long int ticks;
} emu_context;

emu_context *emu_get_context();

int emu_run(int argc, char **argv);

#endif  // EMU_H