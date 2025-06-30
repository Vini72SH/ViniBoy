#include "../include/instructions.h"

char *instruction_name[] = {

    "<NONE>", "NOP", "LD",   "INC",  "DEC", "RLCA", "ADD", "RRCA", "STOP",
    "RLA",    "JR",  "RRA",  "DAA",  "CPL", "SCF",  "CCF", "HALT", "ADC",
    "SUB",    "SBC", "AND",  "XOR",  "OR",  "CP",   "POP", "JP",   "PUSH",
    "RET",    "CB",  "CALL", "RETI", "LDH", "JPHL", "DI",  "EI",   "RST",
    "ERR",    "RLC", "RRC",  "RL",   "RR",  "SLA",  "SRA", "SWAP", "SRL",
    "BIT",    "RES", "SET"

};

instruction instructions[0x100] = {

    // 0x00 - 0x0F
    [0x00] = {IN_NOP, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x01] = {IN_LD, AM_R_D16, RT_BC, RT_NONE, CT_NONE, 0},
    [0x02] = {IN_LD, AM_MR_R, RT_BC, RT_A, CT_NONE, 0},
    [0x03] = {IN_INC, AM_R, RT_BC, RT_NONE, CT_NONE, 0},
    [0x04] = {IN_INC, AM_R, RT_B, RT_NONE, CT_NONE, 0},
    [0x05] = {IN_DEC, AM_R, RT_B, RT_NONE, CT_NONE, 0},
    [0x06] = {IN_LD, AM_R_D8, RT_B, RT_NONE, CT_NONE, 0},
    [0x07] = {IN_RLCA, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x08] = {IN_LD, AM_A16_R, RT_NONE, RT_SP, CT_NONE, 0},
    [0x09] = {IN_ADD, AM_R_R, RT_HL, RT_BC, CT_NONE, 0},
    [0x0A] = {IN_LD, AM_R_MR, RT_A, RT_BC, CT_NONE, 0},
    [0x0B] = {IN_DEC, AM_R, RT_BC, RT_NONE, CT_NONE, 0},
    [0x0C] = {IN_INC, AM_R, RT_C, RT_NONE, CT_NONE, 0},
    [0x0D] = {IN_DEC, AM_R, RT_C, RT_NONE, CT_NONE, 0},
    [0x0E] = {IN_LD, AM_R_D8, RT_C, RT_NONE, CT_NONE, 0},
    [0x0F] = {IN_RRCA, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},

    // 0x10 - 0x1F
    [0x10] = {IN_STOP, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x11] = {IN_LD, AM_R_D16, RT_DE, RT_NONE, CT_NONE, 0},
    [0x12] = {IN_LD, AM_MR_R, RT_DE, RT_A, CT_NONE, 0},
    [0x13] = {IN_INC, AM_R, RT_DE, RT_NONE, CT_NONE, 0},
    [0x14] = {IN_INC, AM_R, RT_D, RT_NONE, CT_NONE, 0},
    [0x15] = {IN_DEC, AM_R, RT_D, RT_NONE, CT_NONE, 0},
    [0x16] = {IN_LD, AM_R_D8, RT_D, RT_NONE, CT_NONE, 0},
    [0x17] = {IN_RLA, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x18] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x19] = {IN_ADD, AM_R_R, RT_HL, RT_DE, CT_NONE, 0},
    [0x1A] = {IN_LD, AM_R_MR, RT_A, RT_DE, CT_NONE, 0},
    [0x1B] = {IN_DEC, AM_R, RT_DE, RT_NONE, CT_NONE, 0},
    [0x1C] = {IN_INC, AM_R, RT_E, RT_NONE, CT_NONE, 0},
    [0x1D] = {IN_DEC, AM_R, RT_E, RT_NONE, CT_NONE, 0},
    [0x1E] = {IN_LD, AM_R_D8, RT_E, RT_NONE, CT_NONE, 0},
    [0x1F] = {IN_RRA, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},

    // 0x20 - 0x2F
    [0xAF] = {IN_XOR, AM_R, RT_A, RT_NONE, CT_NONE, 0},

    [0xC3] = {IN_JP, AM_D16, RT_NONE, RT_NONE, CT_NONE, 0},

    [0xF3] = {IN_DI, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0}

};

char *instr_name(in_type type) { return instruction_name[type]; }

instruction *instruction_by_opcode(uint8_t opcode) {
    if (instructions[opcode].type == IN_NONE) {
        return NULL;
    }

    return &instructions[opcode];
}
