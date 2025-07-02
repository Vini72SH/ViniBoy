#include "../include/bus.h"

#include "../include/cart.h"

/*
 * 0x0000 - 0x3FFF : ROM Bank 0
 * 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
 * 0x8000 - 0x97FF : CHR RAM
 * 0x9800 - 0x9BFF : BG Map 1
 * 0x9C00 - 0x9FFF : BG Map 2
 * 0xA000 - 0xBFFF : Cartridge RAM
 * 0xC000 - 0xCFFF : RAM Bank 0
 * 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
 * 0xE000 - 0xFDFF : Reserved - Echo RAM
 * 0xFE00 - 0xFE9F : Object Attribute Memory
 * 0xFEA0 - 0xFEFF : Reserved - Unusable
 * 0xFF00 - 0xFF7F : I/O Registers
 * 0xFF80 - 0xFFFE : Zero Page
 */

uint8_t bus_read(uint16_t addr) {
    if (addr < 0x8000) {
        // ROM Data
        return cart_read(addr);
    }
    // NO_IMPL
    return 0;
}

void bus_write(uint16_t addr, uint8_t value) {
    if (addr < 0x8000) {
        // ROM Data
        cart_write(addr, value);
        return;
    }
    // NO_IMPL
}

uint16_t bus_read16(uint16_t addr) {
    uint16_t hi, lo;

    lo = bus_read(addr);
    hi = bus_read(addr + 1);

    return lo | (hi << 8);
}

void bus_write16(uint16_t addr, uint16_t value) {
    bus_write(addr + 1, (value >> 8) & 0xFF);
    bus_write(addr, value & 0xFF);
}