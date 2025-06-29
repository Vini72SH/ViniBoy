#ifndef CART_H
#define CART_H

#include "common.h"

typedef struct {
    char title[16];
    uint8_t entry[4];
    uint8_t nintendo_logo[48];
    uint8_t sgb_flag;
    uint8_t cartridge_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t dest_code;
    uint8_t old_lic_code;
    uint8_t version;
    uint8_t checksum;
    uint16_t new_lic_code;
    uint16_t global_checksum;
} rom_header;

typedef struct {
    char filename[1024];
    uint8_t *rom_data;
    uint32_t rom_size;
    rom_header header;
} cart_context;

int cart_load(const char *filename);

#endif  // CART_H