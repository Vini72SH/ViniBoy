#include "../include/interrupts.h"

#include "../include/stack.h"

void int_handler(cpu_context *ctx, uint16_t addr) {
    stack_push16(ctx->regs.pc);
    ctx->regs.pc = addr;
}

bool int_check(cpu_context *ctx, uint16_t addr, interrupt_type type) {
    if (ctx->int_flags & type && ctx->inter_reg & type) {
        int_handler(ctx, addr);
        ctx->int_flags &= ~INT_VBLANK;
        ctx->halted = false;
        ctx->int_master_enabled = false;

        return true;
    }

    return false;
}

void cpu_handle_interrupts(cpu_context *ctx) {
    if (int_check(ctx, 0x40, INT_VBLANK)) {
    } else if (int_check(ctx, 0x48, INT_LCD_STAT)) {
    } else if (int_check(ctx, 0x50, INT_TIMER)) {
    } else if (int_check(ctx, 0x58, INT_SERIAL)) {
    } else if (int_check(ctx, 0x60, INT_JOYPAD)) {
    }
}