#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "instructions.h"

/*
 * CPU registers and flags
 * a: Accumulator
 * f: Flags register (bit 7: Z, bit 6: N, bit 5: H, bit 4: C)
 * b, c, d, e, h, l: General-purpose registers
 * pc: Program Counter
 * sp: Stack Pointer
 */
typedef struct {
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint16_t pc;
    uint16_t sp;
} cpu_registers;

/*
 * CPU context
 * This structure holds the current state of the CPU, including * * registers,
 * fetched data, memory destination, current opcode, and flags for halted and
 * stepping states.
 */
typedef struct {
    uint8_t cur_opcode;
    uint16_t fetched_data;
    uint16_t mem_dest;
    cpu_registers regs;
    instruction* cur_inst;

    bool halted;
    bool stepping;
    bool dest_is_mem;
} cpu_context;

void cpu_init();

bool cpu_step();

#endif  // CPU_H