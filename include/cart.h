#ifndef CART_H
#define CART_H

#include "common.h"

typedef struct {
    uint8_t entry[4];
    uint8_t nintendo_logo[48];

    char title[16];
    uint16_t new_lic_code;
    uint8_t sgb_flag;
    uint8_t cartridge_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t dest_code;
    uint8_t lic_code;
    uint8_t version;
    uint8_t checksum;
    uint16_t global_checksum;
} rom_header;

typedef struct {
    char filename[1024];
    uint32_t rom_size;
    uint8_t *rom_data;
    rom_header *header;
} cart_context;

bool cart_load(const char *cart);
bool cart_close();

#endif  // CART_H