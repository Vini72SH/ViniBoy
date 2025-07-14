#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "cpu.h"

typedef enum {
    INT_VBLANK = 0x01,
    INT_LCD_STAT = 0x02,
    INT_TIMER = 0x04,
    INT_SERIAL = 0x08,
    INT_JOYPAD = 0x10
} interrupt_type;

void cpu_request_interrupt(interrupt_type type);

void cpu_handle_interrupts(cpu_context* ctx);

#endif  // INTERRUPTS_H