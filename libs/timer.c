#include "../include/timer.h"

#include "../include/interrupts.h"

timer_context timer_ctx = {0};

timer_context* timer_get_context() { return &timer_ctx; }

void timer_init() { timer_ctx.div = 0xAC00; }

void timer_tick() {
    bool timer_update = false;
    uint16_t prev_div = timer_ctx.div;

    timer_ctx.div++;

    switch (timer_ctx.tac & (0b11)) {
        case 0b00:
            timer_update =
                (prev_div & (1 << 9)) && (!(timer_ctx.div & (1 << 9)));
            break;
        case 0b01:
            timer_update =
                (prev_div & (1 << 3)) && (!(timer_ctx.div & (1 << 3)));
            break;
        case 0b10:
            timer_update =
                (prev_div & (1 << 5)) && (!(timer_ctx.div & (1 << 5)));
            break;
        case 0b11:
            timer_update =
                (prev_div & (1 << 7)) && (!(timer_ctx.div & (1 << 7)));
            break;
    }

    if (timer_update && timer_ctx.tac & (1 << 2)) {
        timer_ctx.tima++;

        if (timer_ctx.tima == 0xFF) {
            cpu_request_interrupt(INT_TIMER);
        }
    }
}

uint8_t timer_read(uint16_t addr) {
    switch (addr) {
        case 0xFF04:
            // DIV
            return timer_ctx.div >> 8;
        case 0xFF05:
            // TIMA
            return timer_ctx.tima;
        case 0xFF06:
            // TMA
            return timer_ctx.tma;
        case 0xFF07:
            // TAC
            return timer_ctx.tac;
    }

    return 0;
}

void timer_write(uint16_t addr, uint8_t value) {
    switch (addr) {
        case 0xFF04:
            // DIV
            timer_ctx.div = 0;
            break;
        case 0xFF05:
            // TIMA
            timer_ctx.tima = value;
            break;
        case 0xFF06:
            // TMA
            timer_ctx.tma = value;
            break;
        case 0xFF07:
            // TAC
            timer_ctx.tac = value;
            break;
    }
}