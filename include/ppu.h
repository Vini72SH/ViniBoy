#ifndef PPU_H
#define PPU_H

#include "common.h"

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t tile;

    unsigned f_cgb_pn : 3;
    unsigned f_cgb_vram_bank : 1;
    unsigned f_pn : 1;
    unsigned f_x_flip : 1;
    unsigned f_y_flip : 1;
    unsigned f_bgp : 1;

} oam_entry;

// Priority: 0 = No, 1 = BG and Window color indices 1–3 are drawn over this OBJ
// Y flip: 0 = Normal, 1 = Entire OBJ is vertically mirrored
// X flip: 0 = Normal, 1 = Entire OBJ is horizontally mirrored
// DMG palette [Non CGB Mode only]: 0 = OBP0, 1 = OBP1
// Bank [CGB Mode Only]: 0 = Fetch tile from VRAM bank 0, 1 = Fetch tile from
// VRAM bank 1
// CGB palette [CGB Mode Only]: Which of OBP0–7 to use

typedef struct {
    oam_entry oam_ram[40];
    uint8_t vram[0x2000];
} ppu_context;

void ppu_init();
void ppu_tick();

void ppu_oam_write(uint16_t address, uint8_t value);
uint8_t ppu_oam_read(uint16_t address);

void ppu_vram_write(uint16_t address, uint8_t value);
uint8_t ppu_vram_read(uint16_t address);

#endif  // PPU_H