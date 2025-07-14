#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Debugging macro
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf("[DEBUG]: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

/* Get bit n of a*/
#define BIT(a, n) ((a & (a << n)) ? 1 : 0)

/* Set bit n of a to on or off */
#define BIT_SET(a, n, on)       \
    do {                        \
        if (on)                 \
            (a) |= (1 << (n));  \
        else                    \
            (a) &= ~(1 << (n)); \
    } while (0)

/* Tells if a is between b and c */
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#define NO_IMPL                                   \
    {                                             \
        fprintf(stderr, "NOT YET IMPLEMENTED\n"); \
        exit(EXIT_FAILURE);                       \
    }

void delay(uint32_t ms);

#endif  // COMMON_H