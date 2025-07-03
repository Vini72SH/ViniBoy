#ifndef RAM_H
#define RAM_H

#include "common.h"

typedef struct {
    uint8_t wram[0x2000];  // 8KB WRAM
    uint8_t hram[0x80];    // 127 bytes HRAM
} ram_context;

uint8_t wram_read(uint16_t addr);
void wram_write(uint16_t addr, uint8_t value);

uint8_t hram_read(uint16_t addr);
void hram_write(uint16_t addr, uint8_t value);

#endif  // RAM_H