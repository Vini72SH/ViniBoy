#include "../include/stack.h"

#include "../include/bus.h"
#include "../include/cpu.h"

void stack_push(uint8_t data) {
    cpu_registers* regs;

    regs = cpu_get_regs();
    regs->sp--;

    bus_write(regs->sp, data);
}

void stack_push16(uint16_t data) {
    stack_push((data >> 8) & 0xFF);
    stack_push((data & 0xFF));
}

uint8_t stack_pop() {
    cpu_registers* regs;

    regs = cpu_get_regs();
    return bus_read(regs->sp++);
}

uint16_t stack_pop16() {
    uint16_t hi, lo;

    lo = stack_pop();
    hi = stack_pop();

    return lo | (hi << 8);
}