#include "../include/io.h"

#include "../include/cpu.h"
#include "../include/timer.h"

char serial_data[2];

uint8_t io_read(uint16_t addr) {
    if (addr == 0xFF01) {
        return serial_data[0];
    }

    if (addr == 0xFF02) {
        return serial_data[1];
    }

    if (BETWEEN(addr, 0xFF04, 0xFF07)) {
        return timer_read(addr);
    }

    if (addr == 0xFF0F) {
        return cpu_get_int_flags();
    }

    return 0;
}

void io_write(uint16_t addr, uint8_t value) {
    if (addr == 0xFF01) {
        serial_data[0] = value;
        return;
    }

    if (addr == 0xFF02) {
        serial_data[1] = value;
        return;
    }

    if (BETWEEN(addr, 0xFF04, 0xFF07)) {
        timer_write(addr, value);
        return;
    }

    if (addr == 0xFF0F) {
        cpu_set_int_flags(value);
        return;
    }
}