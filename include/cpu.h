#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "instructions.h"

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_SUBTRACT BIT(ctx->regs.f, 6)
#define CPU_FLAG_HALF_CARRY BIT(ctx->regs.f, 5)
#define CPU_FLAG_CARRY BIT(ctx->regs.f, 4)

#define CPU_SET_FLAG_Z(on) BIT_SET(ctx->regs.f, 7, on)
#define CPU_SET_FLAG_N(on) BIT_SET(ctx->regs.f, 6, on)
#define CPU_SET_FLAG_H(on) BIT_SET(ctx->regs.f, 5, on)
#define CPU_SET_FLAG_C(on) BIT_SET(ctx->regs.f, 4, on)

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
    uint8_t inter_reg;     /* Interrupt Enable register */
    uint8_t cur_opcode;    /* The current opcode */
    uint16_t fetched_data; /* Data fetched from memory */
    uint16_t mem_dest;     /* Memory destination for the current operation */
    cpu_registers regs;    /* CPU registers */
    instruction* cur_inst; /* Current instruction being executed */

    bool halted;   /* Indicates if the CPU is halted */
    bool stepping; /* Indicates if the CPU is stepping through instructions */
    bool dest_is_mem; /* Indicates if the memory destination is a mem address */
    bool int_master_enabled; /* Indicates if the interrupt master is enabled */
} cpu_context;

typedef void (*IN_PROC)(cpu_context*);

uint8_t cpu_get_inter_reg();
cpu_registers* cpu_get_regs();

void cpu_set_inter_reg(uint8_t value);

void cpu_set_flags(cpu_context* ctx, bool z, bool n, bool h, bool c);

void cpu_init();

bool cpu_step();

IN_PROC inst_get_processor(in_type type);

#endif  // CPU_H