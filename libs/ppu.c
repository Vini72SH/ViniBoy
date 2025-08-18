#include "../include/ppu.h"

static ppu_context ppu_ctx;

void ppu_init();
void ppu_tick();

void ppu_oam_write(uint16_t address, uint8_t value) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)ppu_ctx.oam_ram;
    p[address] = value;
}

uint8_t ppu_oam_read(uint16_t address) {
    if (address >= 0xFE00) {
        address -= 0xFE00;
    }

    uint8_t *p = (uint8_t *)ppu_ctx.oam_ram;
    return p[address];
}

void ppu_vram_write(uint16_t address, uint8_t value) {
    address -= 0x8000;

    ppu_ctx.vram[address] = value;
}
uint8_t ppu_vram_read(uint16_t address) {
    address -= 0x8000;

    return ppu_ctx.vram[address];
}
