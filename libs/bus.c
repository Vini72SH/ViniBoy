#include "../include/bus.h"

#include "../include/cart.h"
#include "../include/cpu.h"
#include "../include/ram.h"

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
    } else if (addr < 0xA000) {
        // Char / Map Data
        // TODO
        NO_IMPL
    } else if (addr < 0xC000) {
        // Cartridge Ram
        return cart_read(addr);
    } else if (addr < 0xE000) {
        // WRAM (Working Ram)
        return wram_read(addr);
    } else if (addr < 0xFE00) {
        // Reserved Echo Ram
        return 0;
    } else if (addr < 0xFEA0) {
        // OAM
        // TODO
        // NO_IMPL
        return 0;
    } else if (addr < 0xFF00) {
        // Reserved Unusable
        return 0;
    } else if (addr < 0xFF80) {
        // IO Registers
        // TODO
        // NO_IMPL
        return 0;
    } else if (addr == 0xFFFF) {
        return cpu_get_inter_reg();
    }

    return hram_read(addr);
}

void bus_write(uint16_t addr, uint8_t value) {
    if (addr < 0x8000) {
        // ROM Data
        cart_write(addr, value);
        return;
    } else if (addr < 0xA000) {
        // Char / Map Data
        // TODO
        // NO_IMPL
    } else if (addr < 0xC000) {
        // EXT-RAM
        cart_write(addr, value);
    } else if (addr < 0xE000) {
        // WRAM
        wram_write(addr, value);
    } else if (addr < 0xFE00) {
        // Reserved Echo Ram
        // TODO
        // NO_IMPL
    } else if (addr < 0xFEA0) {
        // OAM
        // Todo
        // NO_IMPL
    } else if (addr < 0xFF00) {
        // Reserved Unusable
        // TODO
        // NO_IMPL
    } else if (addr < 0xFF80) {
        // I0 Registers
        // TODO
        // NO_IMPL
    } else if (addr == 0xFFFF) {
        cpu_set_inter_reg(value);
    } else {
        hram_write(addr, value);
    }
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