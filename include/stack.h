#ifndef STACK_H
#define STACK_H

#include "common.h"

void stack_push(uint8_t data);
void stack_push16(uint16_t data);

uint8_t stack_pop();
uint16_t stack_pop16();

#endif  // STACK_H