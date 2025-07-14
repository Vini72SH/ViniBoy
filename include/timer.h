#ifndef TIMER_H
#define TIMER_H

#include "common.h"

typedef struct {
    uint16_t div;
    uint8_t tima;
    uint8_t tma;
    uint8_t tac;
} timer_context;

timer_context *timer_get_context();

void timer_init();

void timer_tick();

uint8_t timer_read(uint16_t addr);

void timer_write(uint16_t addr, uint8_t value);

#endif  // TIMER_H