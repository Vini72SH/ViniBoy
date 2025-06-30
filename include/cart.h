#ifndef CART_H
#define CART_H

#include "common.h"

/*
 * Struct to represent the ROM header of a Game
 */
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

/*
 * Struct to represent the cartridge context
 * This includes the filename, ROM size, ROM data, and header information.
 * The ROM data is dynamically allocated based on the size of the ROM.
 */
typedef struct {
    char filename[1024];
    uint32_t rom_size;
    uint8_t *rom_data;
    rom_header *header;
} cart_context;

bool cart_load(const char *cart);

uint8_t cart_read(uint16_t addr);

void cart_write(uint16_t addr, uint8_t value);

bool cart_close();

#endif  // CART_H