#ifndef DMA_H
#define DMA_H

#include "common.h"

typedef struct {
    bool active;
    uint8_t byte;
    uint8_t value;
    uint8_t start_delay;
} dma_context;

void dma_start(uint8_t start);

void dma_tick();

bool dma_transferring();

#endif  // DMA_H