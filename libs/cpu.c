#include "../include/cpu.h"

#include "../include/bus.h"
#include "../include/emu.h"
#include "../include/interrupts.h"
#include "../include/stack.h"
#include "../include/timer.h"

cpu_context cpu_ctx = {0};

uint8_t cpu_get_inter_reg() { return cpu_ctx.inter_reg; }

cpu_registers *cpu_get_regs() { return &cpu_ctx.regs; }

void cpu_set_inter_reg(uint8_t value) { cpu_ctx.inter_reg = value; }

uint8_t cpu_get_int_flags() { return cpu_ctx.int_flags; }

void cpu_set_int_flags(uint8_t flags) { cpu_ctx.int_flags = flags; }

/*
 * Reverse n
 */
uint16_t reverse(uint16_t n) { return ((n & 0xFF00) >> 8) | ((n & 0xFF << 8)); }

/*
 * Set the flags in register f
 */
void cpu_set_flags(cpu_context *ctx, bool z, bool n, bool h, bool c) {
    CPU_SET_FLAG_Z(z);
    CPU_SET_FLAG_N(n);
    CPU_SET_FLAG_H(h);
    CPU_SET_FLAG_C(c);
}

reg_type rt_lookup[] = {
    RT_B, RT_C, RT_D, RT_E, RT_H, RT_L, RT_HL, RT_A,
};

reg_type decode_reg(uint8_t reg) {
    if (reg > 0b111) {
        return RT_NONE;
    }

    return rt_lookup[reg];
}

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
 * Sets the value in register
 */
void cpu_set_reg(reg_type rt, uint16_t value) {
    switch (rt) {
        case RT_NONE:
            break;
        case RT_A:
            cpu_ctx.regs.a = value & 0xFF;
            break;
        case RT_F:
            cpu_ctx.regs.f = value & 0xFF;
            break;
        case RT_B:
            cpu_ctx.regs.b = value & 0xFF;
            break;
        case RT_C:
            cpu_ctx.regs.c = value & 0xFF;
            break;
        case RT_D:
            cpu_ctx.regs.d = value & 0xFF;
            break;
        case RT_E:
            cpu_ctx.regs.e = value & 0xFF;
            break;
        case RT_H:
            cpu_ctx.regs.h = value & 0xFF;
            break;
        case RT_L:
            cpu_ctx.regs.l = value & 0xFF;
            break;
        case RT_AF:
            *((uint16_t *)&cpu_ctx.regs.a) = reverse(value);
            break;
        case RT_BC:
            *((uint16_t *)&cpu_ctx.regs.b) = reverse(value);
            break;
        case RT_DE:
            *((uint16_t *)&cpu_ctx.regs.d) = reverse(value);
            break;
        case RT_HL:
            *((uint16_t *)&cpu_ctx.regs.h) = reverse(value);
            break;
        case RT_SP:
            cpu_ctx.regs.sp = value & 0xFF;
            break;
        case RT_PC:
            cpu_ctx.regs.pc = value & 0xFF;
            break;
    }
}

uint8_t cpu_read_reg8(reg_type rt) {
    switch (rt) {
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
        case RT_HL: {
            return bus_read(cpu_read_reg(RT_HL));
        }
        default:
            DEBUG_PRINT("**ERR INVALID REG8: %d\n", rt);
            NO_IMPL
    }
}

void cpu_set_reg8(reg_type rt, uint8_t value) {
    switch (rt) {
        case RT_A:
            cpu_ctx.regs.a = value & 0xFF;
            break;
        case RT_F:
            cpu_ctx.regs.f = value & 0xFF;
            break;
        case RT_B:
            cpu_ctx.regs.b = value & 0xFF;
            break;
        case RT_C:
            cpu_ctx.regs.c = value & 0xFF;
            break;
        case RT_D:
            cpu_ctx.regs.d = value & 0xFF;
            break;
        case RT_E:
            cpu_ctx.regs.e = value & 0xFF;
            break;
        case RT_H:
            cpu_ctx.regs.h = value & 0xFF;
            break;
        case RT_L:
            cpu_ctx.regs.l = value & 0xFF;
            break;
        case RT_HL:
            bus_write(cpu_read_reg(RT_HL), value);
            break;
        default:
            DEBUG_PRINT("**ERR INVALID REG8: %d\n", rt);
            NO_IMPL
    }
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

void goto_addr(cpu_context *ctx, uint16_t addr, bool pushpc) {
    if (check_cond(ctx)) {
        if (pushpc) {
            emu_cycles(2);
            stack_push16(ctx->regs.pc);
        }
    }

    ctx->regs.pc = addr;
    emu_cycles(1);
}

void proc_none(cpu_context *ctx) { exit(EXIT_FAILURE); }

void proc_nop(cpu_context *ctx) {}

void proc_ld(cpu_context *ctx) {
    if (cpu_ctx.dest_is_mem) {
        if (cpu_ctx.cur_inst->reg_2 >= RT_AF) {
            // Data has more than 8 bits
            emu_cycles(1);
            bus_write16(ctx->mem_dest, ctx->fetched_data);
        } else {
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }

        return;
    }

    if (cpu_ctx.cur_inst->mode == AM_HL_SPR) {
        // Special Case
        bool hflag = (cpu_read_reg(cpu_ctx.cur_inst->reg_2) & 0xF) +
                         (cpu_ctx.fetched_data & 0xF) >=
                     0x10;
        bool cflag = (cpu_read_reg(cpu_ctx.cur_inst->reg_2) & 0xFF) +
                         (cpu_ctx.fetched_data & 0xFF) >=
                     0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_2) +
                                              (char)ctx->fetched_data);

        return;
    }

    // Only writes the data in register
    cpu_set_reg(cpu_ctx.cur_inst->reg_1, cpu_ctx.fetched_data);
}

void proc_inc(cpu_context *ctx) {
    bool z, n, h, c;
    uint16_t data = cpu_read_reg(ctx->cur_inst->reg_1);

    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR) {
        data = bus_read(cpu_read_reg(RT_HL)) + 1;
        data &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), data);
    } else {
        data++;
        cpu_set_reg(ctx->cur_inst->reg_1, data);
    }

    if ((ctx->cur_opcode & 0x03) != 0x03) {
        emu_cycles(1);
        return;
    }

    z = (data == 0);
    n = 0;
    h = (data & 0x0F) == 0;
    c = CPU_FLAG_CARRY;
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_dec(cpu_context *ctx) {
    bool z, n, h, c;
    uint16_t data = cpu_read_reg(ctx->cur_inst->reg_1);

    if ((ctx->cur_inst->reg_1 == RT_HL) && (ctx->cur_inst->mode == AM_MR)) {
        data = bus_read(cpu_read_reg(RT_HL)) - 1;
        bus_write(cpu_read_reg(RT_HL), data);
    } else {
        data--;
        cpu_set_reg(ctx->cur_inst->reg_1, data);
    }

    if ((ctx->cur_opcode & 0x0B) != 0x0B) {
        emu_cycles(1);
        return;
    }

    z = (data == 0);
    n = 1;
    h = (data & 0x0F) == 0x0F;
    c = CPU_FLAG_CARRY;
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_rlca(cpu_context *ctx) {
    uint8_t data = cpu_read_reg8(RT_A);
    bool c = BIT(data, 7);
    data = (data << 1) | c;
    cpu_set_reg(RT_A, data);

    cpu_set_flags(ctx, 0, 0, 0, c);
}

void proc_add(cpu_context *ctx) {
    bool z, n, h, c;
    uint32_t data = cpu_read_reg(ctx->cur_inst->reg_1);

    if (ctx->cur_inst->reg_1 == RT_SP) {
        data += (char)ctx->fetched_data;
    } else {
        data += ctx->fetched_data;
    }

    if (ctx->cur_inst->reg_1 >= RT_AF) {
        emu_cycles(1);
        z = (ctx->cur_inst->reg_1 == RT_SP) ? 0 : CPU_FLAG_Z;
        n = 0;
        h = (ctx->cur_inst->reg_1 == RT_SP) ? ((data & 0xFF) > 0x0F)
                                            : ((data & 0xFFFF) >= 0x1000);
        c = (ctx->cur_inst->reg_1 == RT_SP) ? (data > 0xFF) : ((data > 0xFFFF));
    } else {
        z = (data == 0);
        n = 0;
        h = (data & 0xFF) > 0x0F;
        c = (data > 0xFF);
    }

    cpu_set_reg(ctx->cur_inst->reg_1, data & 0xFFFF);
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_rrca(cpu_context *ctx) {
    uint8_t data = cpu_read_reg8(RT_A);
    uint8_t b = (data & 1);
    data = (data >> 1) | (b << 7);
    cpu_set_reg(RT_A, data);

    cpu_set_flags(ctx, 0, 0, 0, b);
}

void proc_stop(cpu_context *ctx) {
    DEBUG_PRINT("STOPPING\n");
    NO_IMPL
}

void proc_rla(cpu_context *ctx) {
    uint8_t data = cpu_read_reg8(RT_A);
    uint8_t cf = CPU_FLAG_CARRY;
    uint8_t c = (data >> 7) & 1;

    cpu_set_reg(RT_A, (data << 1) | cf);
    cpu_set_flags(ctx, 0, 0, 0, c);
}

void proc_jr(cpu_context *ctx) {
    char relative = (char)(ctx->fetched_data & 0xFF);
    uint16_t addr = ctx->regs.pc + relative;
    goto_addr(ctx, addr, false);
}

void proc_rra(cpu_context *ctx) {
    uint8_t data = cpu_read_reg(RT_A);
    uint8_t c = CPU_FLAG_CARRY;
    uint8_t new_c = data & 1;

    data >>= 1;
    data |= (c << 7);
    cpu_set_flags(ctx, 0, 0, 0, new_c);
}

void proc_daa(cpu_context *ctx) {
    int fc = 0;
    uint8_t reg_a, u = 0;

    reg_a = cpu_read_reg(RT_A);
    if ((CPU_FLAG_HALF_CARRY) || (!CPU_FLAG_SUBTRACT && (reg_a & 0xF) > 9)) {
        u = 6;
    }

    if ((CPU_FLAG_CARRY) || (!CPU_FLAG_SUBTRACT && (reg_a > 0x99))) {
        u |= 0x60;
        fc = 1;
    }

    reg_a += (CPU_FLAG_SUBTRACT) ? -u : u;
    cpu_set_reg(RT_A, reg_a);
    cpu_set_flags(ctx, reg_a == 0, CPU_FLAG_SUBTRACT, 0, fc);
}

void proc_cpl(cpu_context *ctx) {
    bool z, n, h, c;
    uint8_t data = ~(cpu_read_reg(RT_A));

    z = CPU_FLAG_Z;
    n = CPU_FLAG_SUBTRACT;
    h = CPU_FLAG_HALF_CARRY;
    c = CPU_FLAG_CARRY;

    cpu_set_reg(RT_A, data);
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_scf(cpu_context *ctx) {
    bool z, n, h, c;

    z = CPU_FLAG_Z;
    n = 0;
    h = 0;
    c = 1;

    cpu_set_flags(ctx, z, n, h, c);
}

void proc_ccf(cpu_context *ctx) {
    bool z, n, h, c;

    z = CPU_FLAG_Z;
    n = 0;
    h = 0;
    c = CPU_FLAG_CARRY ^ 1;

    cpu_set_flags(ctx, z, n, h, c);
}

void proc_halt(cpu_context *ctx) { ctx->halted = true; }

void proc_adc(cpu_context *ctx) {
    bool z, n, h, c;
    uint16_t data, reg, c_flag;

    data = ctx->fetched_data;
    reg = ctx->cur_inst->reg_1;
    c_flag = CPU_FLAG_CARRY;

    data += reg + c_flag;

    z = (data == 0);
    n = 0;
    h = (data & 0xFF) > 0x0F;
    c = (data > 0xFF);

    cpu_set_reg(ctx->cur_inst->reg_1, data);
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_sub(cpu_context *ctx) {
    bool z, n, h, c;
    uint16_t data, reg;
    reg = cpu_read_reg(ctx->cur_inst->reg_1);
    data = ctx->fetched_data;

    z = (reg - data == 0);
    n = 1;
    h = (((int)(reg & 0xF) - (int)(data & 0xF)) < 0);
    c = (((int)reg - (int)data) < 0);

    data = reg - data;
    cpu_set_reg(ctx->cur_inst->reg_1, data);
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_sbc(cpu_context *ctx) {
    bool z, n, h, c;
    uint16_t data, reg;
    reg = cpu_read_reg(ctx->cur_inst->reg_1);
    data = ctx->fetched_data + CPU_FLAG_CARRY;

    z = (reg - data == 0);
    n = 1;
    h = (((int)(reg & 0xF) - (int)(data & 0xF)) < 0);
    c = (((int)reg - (int)data) < 0);

    data = reg - data;
    cpu_set_reg(ctx->cur_inst->reg_1, data);
    cpu_set_flags(ctx, z, n, h, c);
}

void proc_and(cpu_context *ctx) {
    ctx->regs.a &= ctx->fetched_data;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 1, 0);
}

void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= (ctx->fetched_data & 0xFF);
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

void proc_or(cpu_context *ctx) {
    ctx->regs.a |= (ctx->fetched_data & 0xFF);
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

void proc_cp(cpu_context *ctx) {
    int data = (int)ctx->regs.a - (int)ctx->fetched_data;
    cpu_set_flags(
        ctx, data == 0, 1,
        ((int)ctx->regs.a & 0x0F) - ((int)ctx->fetched_data & 0x0F) < 0,
        data < 0);
}

void proc_pop(cpu_context *ctx) {
    uint16_t lo = stack_pop();
    emu_cycles(1);
    uint16_t hi = stack_pop();
    emu_cycles(1);

    uint16_t data = lo | (hi << 8);

    cpu_set_reg(ctx->cur_inst->reg_1, data);

    if (ctx->cur_inst->reg_1 == RT_AF) {
        cpu_set_reg(ctx->cur_inst->reg_1, data & 0xFFF0);
    }
}

void proc_jp(cpu_context *ctx) { goto_addr(ctx, ctx->fetched_data, false); }

void proc_push(cpu_context *ctx) {
    uint16_t hi, lo;

    hi = (cpu_read_reg(ctx->cur_inst->reg_1) >> 8) & 0xFF;
    emu_cycles(1);
    stack_push(hi);

    lo = cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF;
    emu_cycles(1);
    stack_push(lo);

    emu_cycles(1);
}

void proc_ret(cpu_context *ctx) {
    uint16_t hi, lo, addr;

    if (ctx->cur_inst->cond != CT_NONE) {
        emu_cycles(1);
    }

    if (check_cond(ctx)) {
        lo = stack_pop();
        emu_cycles(1);
        hi = stack_pop();
        emu_cycles(1);

        addr = lo | (hi << 8);
        ctx->regs.pc = addr;
    }
}

void proc_cb(cpu_context *ctx) {
    bool c, setC;
    uint8_t old, result;
    uint8_t op = ctx->fetched_data;
    reg_type reg = decode_reg(op & 0b111);
    uint8_t bit = (op >> 3) & 0b111;
    uint8_t bit_op = (op >> 6) & 0b11;
    uint8_t reg_val = cpu_read_reg(reg);

    emu_cycles(1);

    if (reg == RT_HL) {
        emu_cycles(2);
    }

    c = CPU_FLAG_CARRY;
    switch (bit_op) {
        case 1:
            // BIT
            cpu_set_flags(ctx, !(BIT(reg, bit)), 0, 1, c);
            return;

        case 2:
            // RST
            BIT_SET(reg, bit, 0);
            return;

        case 3:
            // SET
            BIT_SET(reg, bit, 1);
            return;
    }

    switch (bit) {
        case 0:
            // RLC
            setC = false;
            result = (reg_val << 1) & 0xFF;

            if (BIT(reg_val, 7) != 0) {
                result |= 1;
                setC = true;
            }

            cpu_set_reg8(reg, result);
            cpu_set_flags(&cpu_ctx, result == 0, 0, 0, setC);
            return;
        case 1:
            // RRC
            old = reg_val;
            reg_val >>= 1;
            reg_val |= BIT(old, 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(&cpu_ctx, !(reg_val), 0, 0, old & 1);
            return;
        case 2:
            // RL
            old = reg_val;
            reg_val <<= 1;
            reg_val |= c;
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(&cpu_ctx, !(reg_val), 0, 0, !!(old & 0x80));
            return;
        case 3:
            // RR
            old = reg_val;
            reg_val >>= 1;
            reg_val |= (c << 7);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(&cpu_ctx, !(reg_val), 0, 0, old & 1);
            return;
        case 4:
            // SLA
            old = reg_val;
            reg_val <<= 1;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(&cpu_ctx, !(reg_val), 0, 0, !!(old & 0x80));
            return;
        case 5:
            // SRA
            old = (int8_t)reg_val >> 1;
            cpu_set_reg8(reg, old);
            cpu_set_flags(&cpu_ctx, !(old), 0, 0, reg_val & 1);
            return;
        case 6:
            // SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0x0F) << 4);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(&cpu_ctx, reg_val == 0, 0, 0, 0);
            return;
        case 7:
            // SRL
            old = reg_val >> 1;
            cpu_set_reg8(reg, old);
            cpu_set_flags(&cpu_ctx, !(old), 0, 0, reg_val & 1);
            return;
    }

    DEBUG_PRINT("ERROR: INVALID CB: %02X\n", op);
    NO_IMPL
}

void proc_call(cpu_context *ctx) { goto_addr(ctx, ctx->fetched_data, true); }

void proc_reti(cpu_context *ctx) {
    ctx->int_master_enabled = true;
    proc_ret(ctx);
}

void proc_ldh(cpu_context *ctx) {
    if (ctx->cur_inst->reg_1 == RT_A) {
        cpu_set_reg(RT_A, bus_read(0xFF00 | ctx->fetched_data));
    } else {
        bus_write(ctx->mem_dest, ctx->fetched_data);
    }

    emu_cycles(1);
}

void proc_di(cpu_context *ctx) { ctx->int_master_enabled = false; }

void proc_ei(cpu_context *ctx) { ctx->enabling_ime = true; }

void proc_rst(cpu_context *ctx) { goto_addr(ctx, ctx->cur_inst->param, true); }

/*
 * Function Pointer Map
 * It contains the reference to the Software functions that will emulate the
 * Assembly instructions
 */
static IN_PROC processors[] = {
    [IN_NONE] = proc_none, [IN_NOP] = proc_nop,   [IN_LD] = proc_ld,
    [IN_INC] = proc_inc,   [IN_DEC] = proc_dec,   [IN_RLCA] = proc_rlca,
    [IN_ADD] = proc_add,   [IN_RRCA] = proc_rrca, [IN_STOP] = proc_stop,
    [IN_RLA] = proc_rla,   [IN_JR] = proc_jr,     [IN_RRA] = proc_rra,
    [IN_DAA] = proc_daa,   [IN_CPL] = proc_cpl,   [IN_SCF] = proc_scf,
    [IN_CCF] = proc_ccf,   [IN_HALT] = proc_halt, [IN_ADC] = proc_adc,
    [IN_SUB] = proc_sub,   [IN_SBC] = proc_sbc,   [IN_AND] = proc_and,
    [IN_XOR] = proc_xor,   [IN_OR] = proc_or,     [IN_CP] = proc_cp,
    [IN_POP] = proc_pop,   [IN_JP] = proc_jp,     [IN_PUSH] = proc_push,
    [IN_RET] = proc_ret,   [IN_CB] = proc_cb,     [IN_CALL] = proc_call,
    [IN_RETI] = proc_reti, [IN_LDH] = proc_ldh,   [IN_JPHL] = NULL,
    [IN_DI] = proc_di,     [IN_EI] = proc_ei,     [IN_RST] = proc_rst,
    [IN_ERR] = NULL,       [IN_RLC] = NULL,       [IN_RRC] = NULL,
    [IN_RL] = NULL,        [IN_RR] = NULL,        [IN_SLA] = NULL,
    [IN_SRA] = NULL,       [IN_SWAP] = NULL,      [IN_SRL] = NULL,
    [IN_BIT] = NULL,       [IN_RES] = NULL,       [IN_SET] = NULL};

/*
 * Returns the pointer to the function corresponding to the type of the
 * instruction
 */
IN_PROC inst_get_processor(in_type type) { return processors[type]; }

/*
 * Start Address 0x100
 */
void cpu_init() {
    cpu_ctx.regs.pc = 0x100;
    cpu_ctx.regs.sp = 0xDFFE;
    *((short *)&cpu_ctx.regs.a) = 0xB001;
    *((short *)&cpu_ctx.regs.b) = 0x1300;
    *((short *)&cpu_ctx.regs.d) = 0xD800;
    *((short *)&cpu_ctx.regs.h) = 0x4D01;

    cpu_ctx.inter_reg = 0;
    cpu_ctx.int_flags = 0;
    cpu_ctx.int_master_enabled = false;
    cpu_ctx.enabling_ime = false;

    cpu_ctx.cur_opcode = 0;
    cpu_ctx.fetched_data = 0;
    cpu_ctx.mem_dest = 0;
    cpu_ctx.halted = false;
    cpu_ctx.stepping = true;
    cpu_ctx.dest_is_mem = false;

    timer_get_context()->div = 0xABCC;
};

/*
 * Instruction Fetch
 */
void fetch_instruction() {
    // Read the address and get the opcode
    cpu_ctx.cur_opcode = bus_read(cpu_ctx.regs.pc++);
    // Translate the opcode into instruction
    cpu_ctx.cur_inst = instruction_by_opcode(cpu_ctx.cur_opcode);
}

void fetch_data() {
    if (cpu_ctx.cur_inst == NULL) return;

    uint16_t hi, lo, addr;
    cpu_ctx.mem_dest = 0;
    cpu_ctx.dest_is_mem = false;

    switch (cpu_ctx.cur_inst->mode) {
        case AM_IMP:
            // Nothing
            break;

        case AM_R_D16:
        case AM_D16:
            // Read the next 2 bytes
            lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);

            hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);

            cpu_ctx.fetched_data = lo | (hi << 8);

            cpu_ctx.regs.pc += 2;
            break;

        case AM_R_R:
            // Only read the reg2 data
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            break;

        case AM_MR_R:
            // Read the reg2 data and calculates the destination address in
            // memory
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);

            // Special case
            if (cpu_ctx.cur_inst->reg_1 == RT_C) {
                cpu_ctx.mem_dest |= 0xFF00;
            }

            break;

        case AM_R:
            // Only read the reg1 data
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            break;

        case AM_R_D8:
            // Read the next byte
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            break;

        case AM_R_MR:
            // Takes the memory address stored in a register and loads the data
            // from this address
            addr = cpu_read_reg(cpu_ctx.cur_inst->reg_2);

            // Special case
            if (cpu_ctx.cur_inst->reg_1 == RT_C) {
                addr |= 0xFF00;
            }

            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            break;

        case AM_R_HLI:
            // Reads data from the register and increments it
            cpu_ctx.fetched_data =
                bus_read(cpu_read_reg(cpu_ctx.cur_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            break;

        case AM_R_HLD:
            // Reads data from the register and decrements it
            cpu_ctx.fetched_data =
                bus_read(cpu_read_reg(cpu_ctx.cur_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            break;

        case AM_HLI_R:
            // Reads the data from the register, calculates the destination
            // address and increments it
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            break;

        case AM_HLD_R:
            // Reads the data from the register, calculates the destination
            // address and decrements it
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            break;

        case AM_R_A8:
            // Read the next byte
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            break;

        case AM_A8_R:
            // Read the data from register
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = bus_read(cpu_ctx.regs.pc) | 0xFF00;
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            break;

        case AM_HL_SPR:
            // Read the next byte
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            break;

        case AM_A16_R:
        case AM_D16_R:
            // Read the data from register and calculates the destination
            // address
            lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);

            hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);

            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = lo | (hi << 8);

            cpu_ctx.regs.pc += 2;

            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            break;

        case AM_D8:
            // Read the next byte
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            break;

        case AM_MR_D8:
            // Read the next byte and get the destination address from register
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;

            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            break;

        case AM_MR:
            // The destination address and the read address are the same
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            cpu_ctx.fetched_data =
                bus_read(cpu_read_reg(cpu_ctx.cur_inst->reg_1));
            emu_cycles(1);
            break;

        case AM_R_A16:
            // Read the next 2 bytes and get data from 16-bit address
            lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);

            hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);

            addr = lo | (hi << 8);
            cpu_ctx.regs.pc += 2;

            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            break;
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
#ifdef DEBUG
        uint16_t pc = cpu_ctx.regs.pc;
#endif
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
    } else {
        emu_cycles(1);

        if (cpu_ctx.int_flags) {
            cpu_ctx.halted = false;
        }
    }

    if (cpu_ctx.int_master_enabled) {
        cpu_handle_interrupts(&cpu_ctx);
        cpu_ctx.enabling_ime = false;
    }

    if (cpu_ctx.enabling_ime) {
        cpu_ctx.int_master_enabled = true;
    }

    return true;
}

void cpu_request_interrupt(interrupt_type type) { cpu_ctx.int_flags |= type; }