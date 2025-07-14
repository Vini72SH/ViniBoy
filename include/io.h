#ifndef IO_H
#define IO_H

#include "common.h"

uint8_t io_read(uint16_t addr);

void io_write(uint16_t addr, uint8_t value);

#endif  // IO_H