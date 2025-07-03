#include "../include/ram.h"

ram_context ram_ctx;

/*
 * Work and High RAM Reads and Writes
 */

uint8_t wram_read(uint16_t addr) {
    addr -= 0xC000;

    if (addr >= 0x2000) {
        DEBUG_PRINT("INVALID (READ) WRAM ADDR %08X\n", addr + 0xC000);
        return 0;
    }

    return ram_ctx.wram[addr];
}

void wram_write(uint16_t addr, uint8_t value) {
    addr -= 0xC000;

    if (addr >= 0x2000) {
        DEBUG_PRINT("INVALID (WRITE) WRAM ADDR %08X\n", addr + 0xC000);
        return;
    }

    ram_ctx.wram[addr] = value;
}

uint8_t hram_read(uint16_t addr) {
    addr -= 0xFF80;

    if (addr >= 0x80) {
        DEBUG_PRINT("INVALID (READ) HRAM ADDR %08X\n", addr + 0xFF80);
        return 0;
    }

    return ram_ctx.hram[addr];
}

void hram_write(uint16_t addr, uint8_t value) {
    addr -= 0xFF80;

    if (addr >= 0x80) {
        DEBUG_PRINT("INVALID (WRITE) HRAM ADDR %08X\n", addr + 0xFF80);
        return;
    }

    ram_ctx.hram[addr] = value;
}