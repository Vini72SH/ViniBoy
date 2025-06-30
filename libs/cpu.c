#include "../include/cpu.h"

#include "../include/bus.h"
#include "../include/emu.h"

cpu_context cpu_ctx = {0};

/*
 * Set the flags in register f
 */
void cpu_set_flags(cpu_context *ctx, bool z, bool n, bool h, bool c) {
    CPU_SET_FLAG_Z(z);
    CPU_SET_FLAG_N(n);
    CPU_SET_FLAG_H(h);
    CPU_SET_FLAG_C(c);
}

/*
 * Checks the condition of the jump
 */
bool check_cond(cpu_context *ctx) {
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_CARRY;

    switch (ctx->cur_inst->cond) {
        case CT_NONE:
            return true;
        case CT_Z:
            return z;
        case CT_NZ:
            return !(z);
        case CT_C:
            return c;
        case CT_NC:
            return !(c);
    }

    return false;
}

void proc_none(cpu_context *ctx) { exit(EXIT_FAILURE); }

void proc_nop(cpu_context *ctx) {}

void proc_ld(cpu_context *ctx) {
    // TODO
}

void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= (ctx->fetched_data & 0xFF);
    cpu_set_flags(ctx, ctx->regs.a, 0, 0, 0);
}

void proc_jp(cpu_context *ctx) {
    if (check_cond(&cpu_ctx)) {
        cpu_ctx.regs.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}

void proc_di(cpu_context *ctx) { ctx->int_master_enabled = false; }

/*
 * Function Pointer Map
 * It contains the reference to the Software functions that will emulate the
 * Assembly instructions
 */
static IN_PROC processors[] = {
    [IN_NONE] = proc_none, [IN_NOP] = proc_nop, [IN_LD] = proc_ld,
    [IN_INC] = NULL,       [IN_DEC] = NULL,     [IN_RLCA] = NULL,
    [IN_ADD] = NULL,       [IN_RRCA] = NULL,    [IN_STOP] = NULL,
    [IN_RLA] = NULL,       [IN_JR] = NULL,      [IN_RRA] = NULL,
    [IN_DAA] = NULL,       [IN_CPL] = NULL,     [IN_SCF] = NULL,
    [IN_CCF] = NULL,       [IN_HALT] = NULL,    [IN_ADC] = NULL,
    [IN_SUB] = NULL,       [IN_SBC] = NULL,     [IN_AND] = NULL,
    [IN_XOR] = proc_xor,   [IN_OR] = NULL,      [IN_CP] = NULL,
    [IN_POP] = NULL,       [IN_JP] = proc_jp,   [IN_PUSH] = NULL,
    [IN_RET] = NULL,       [IN_CB] = NULL,      [IN_CALL] = NULL,
    [IN_RETI] = NULL,      [IN_LDH] = NULL,     [IN_JPHL] = NULL,
    [IN_DI] = proc_di,     [IN_EI] = NULL,      [IN_RST] = NULL,
    [IN_ERR] = NULL,       [IN_RLC] = NULL,     [IN_RRC] = NULL,
    [IN_RL] = NULL,        [IN_RR] = NULL,      [IN_SLA] = NULL,
    [IN_SRA] = NULL,       [IN_SWAP] = NULL,    [IN_SRL] = NULL,
    [IN_BIT] = NULL,       [IN_RES] = NULL,     [IN_SET] = NULL};

/*
 * Returns the pointer to the function corresponding to the type of the
 * instruction
 */
IN_PROC inst_get_processor(in_type type) { return processors[type]; }

/*
 * Start Address 0x100
 */
void cpu_init() {
    cpu_ctx.cur_opcode = 0;
    cpu_ctx.fetched_data = 0;
    cpu_ctx.mem_dest = 0;
    cpu_ctx.halted = false;
    cpu_ctx.stepping = true;
    cpu_ctx.dest_is_mem = false;
    cpu_ctx.regs.pc = 0x100;
};

/*
 * Reverse n
 */
uint16_t reverse(uint16_t n) { return ((n & 0xFF00) >> 8) | ((n & 0xFF << 8)); }

/*
 * Returns the value of the register
 */
uint16_t cpu_read_reg(reg_type rt) {
    switch (rt) {
        case RT_NONE:
            return 0;
        case RT_A:
            return cpu_ctx.regs.a;
        case RT_F:
            return cpu_ctx.regs.f;
        case RT_B:
            return cpu_ctx.regs.b;
        case RT_C:
            return cpu_ctx.regs.c;
        case RT_D:
            return cpu_ctx.regs.d;
        case RT_E:
            return cpu_ctx.regs.e;
        case RT_H:
            return cpu_ctx.regs.h;
        case RT_L:
            return cpu_ctx.regs.l;
        case RT_AF:
            return reverse(*((uint16_t *)&cpu_ctx.regs.a));
        case RT_BC:
            return reverse(*((uint16_t *)&cpu_ctx.regs.b));
        case RT_DE:
            return reverse(*((uint16_t *)&cpu_ctx.regs.d));
        case RT_HL:
            return reverse(*((uint16_t *)&cpu_ctx.regs.h));
        case RT_SP:
            return cpu_ctx.regs.sp;
        case RT_PC:
            return cpu_ctx.regs.pc;
    }
    return 0;
}

/*
 * Instruction Fetch
 */
void fetch_instruction() {
    cpu_ctx.cur_opcode = bus_read(cpu_ctx.regs.pc++);
    cpu_ctx.cur_inst = instruction_by_opcode(cpu_ctx.cur_opcode);
}

void fetch_data() {
    if (cpu_ctx.cur_inst == NULL) return;

    cpu_ctx.mem_dest = 0;
    cpu_ctx.dest_is_mem = false;

    switch (cpu_ctx.cur_inst->mode) {
        case AM_IMP:
            return;

        case AM_R:
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            return;

        case AM_R_D8:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            return;

        case AM_D16: {
            uint16_t lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);

            uint16_t hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);

            cpu_ctx.fetched_data = lo | (hi << 8);

            cpu_ctx.regs.pc += 2;

            return;
        }

        default:
            DEBUG_PRINT("Unknown Addressing Mode: %d\n",
                        cpu_ctx.cur_inst->mode);
            exit(EXIT_FAILURE);
    }
}

static void execute() {
    IN_PROC proc = inst_get_processor(cpu_ctx.cur_inst->type);

    if (!(proc)) {
        NO_IMPL
    }

    proc(&cpu_ctx);
}

bool cpu_step() {
    if (!(cpu_ctx.halted)) {
        uint16_t pc = cpu_ctx.regs.pc;
        fetch_instruction();
        fetch_data();

        if (cpu_ctx.cur_inst == NULL) {
            DEBUG_PRINT("[PC: %04X] Unknown Instruction:        (%02X)\n", pc,
                        cpu_ctx.cur_opcode);
            return false;
        }

        DEBUG_PRINT("[PC: %04X] Executing Instruction: %4s (%02X)\n", pc,
                    instr_name(cpu_ctx.cur_inst->type), cpu_ctx.cur_opcode);

        execute();
    }

    return true;
}