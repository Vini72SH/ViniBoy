#include "../include/cpu.h"

#include "../include/bus.h"
#include "../include/emu.h"

cpu_context cpu_ctx = {0};

/*
 * Start Address 0x100
 */
void cpu_init() { cpu_ctx.regs.pc = 0x100; };

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
            break;
    }
    return 0;
}

/*
 * Instruction Fetch
 */
static void fetch_instruction() {
    cpu_ctx.cur_opcode = bus_read(cpu_ctx.regs.pc++);
    cpu_ctx.cur_inst = instruction_by_opcode(cpu_ctx.cur_opcode);

    if (cpu_ctx.cur_inst == NULL) {
        DEBUG_PRINT("Unknown Instruction: %02X\n", cpu_ctx.cur_opcode);
        exit(EXIT_FAILURE);
    }
}

static void fetch_data() {
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

static void execute() { DEBUG_PRINT("\tNot executing yet...\n"); }

bool cpu_step() {
    if (!(cpu_ctx.halted)) {
        uint16_t pc = cpu_ctx.regs.pc;
        fetch_instruction();
        fetch_data();

        DEBUG_PRINT("Executing Instruction: %02X    PC: %04X\n",
                    cpu_ctx.cur_opcode, pc);

        execute();
    }

    return true;
}