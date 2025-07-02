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
    [0x20] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_NZ, 0},
    [0x21] = {IN_LD, AM_R_D16, RT_HL, RT_NONE, CT_NONE, 0},
    [0x22] = {IN_LD, AM_HLI_R, RT_HL, RT_A, CT_NONE, 0},
    [0x23] = {IN_INC, AM_R, RT_HL, RT_NONE, CT_NONE, 0},
    [0x24] = {IN_INC, AM_R, RT_H, RT_NONE, CT_NONE, 0},
    [0x25] = {IN_DEC, AM_R, RT_H, RT_NONE, CT_NONE, 0},
    [0x26] = {IN_LD, AM_R_D8, RT_H, RT_NONE, CT_NONE, 0},
    [0x27] = {IN_DAA, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x28] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_Z, 0},
    [0x29] = {IN_ADD, AM_R_R, RT_HL, RT_HL, CT_NONE, 0},
    [0x2A] = {IN_LD, AM_R_HLI, RT_A, RT_HL, CT_NONE, 0},
    [0x2B] = {IN_DEC, AM_R, RT_HL, RT_NONE, CT_NONE, 0},
    [0x2C] = {IN_INC, AM_R, RT_L, RT_NONE, CT_NONE, 0},
    [0x2D] = {IN_DEC, AM_R, RT_L, RT_NONE, CT_NONE, 0},
    [0x2E] = {IN_LD, AM_R_D8, RT_L, RT_NONE, CT_NONE, 0},
    [0x2F] = {IN_CPL, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},

    // 0x30 - 0x3F
    [0x30] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_NC, 0},
    [0x31] = {IN_LD, AM_R_D16, RT_SP, RT_NONE, CT_NONE, 0},
    [0x32] = {IN_LD, AM_HLD_R, RT_HL, RT_A, CT_NONE, 0},
    [0x33] = {IN_INC, AM_R, RT_SP, RT_NONE, CT_NONE, 0},
    [0x34] = {IN_INC, AM_MR, RT_HL, RT_NONE, CT_NONE, 0},
    [0x35] = {IN_DEC, AM_MR, RT_HL, RT_NONE, CT_NONE, 0},
    [0x36] = {IN_LD, AM_MR_D8, RT_HL, RT_NONE, CT_NONE, 0},
    [0x37] = {IN_SCF, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x38] = {IN_JR, AM_D8, RT_NONE, RT_NONE, CT_C, 0},
    [0x39] = {IN_ADD, AM_R_R, RT_HL, RT_SP, CT_NONE, 0},
    [0x3A] = {IN_LD, AM_R_HLD, RT_A, RT_HL, CT_NONE, 0},
    [0x3B] = {IN_DEC, AM_R, RT_SP, RT_NONE, CT_NONE, 0},
    [0x3C] = {IN_INC, AM_R, RT_A, RT_NONE, CT_NONE, 0},
    [0x3D] = {IN_DEC, AM_R, RT_A, RT_NONE, CT_NONE, 0},
    [0x3E] = {IN_LD, AM_R_D8, RT_A, RT_NONE, CT_NONE, 0},
    [0x3F] = {IN_CCF, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},

    // 0x40 - 0x4F
    [0x40] = {IN_LD, AM_R_R, RT_B, RT_B, CT_NONE, 0},
    [0x41] = {IN_LD, AM_R_R, RT_B, RT_C, CT_NONE, 0},
    [0x42] = {IN_LD, AM_R_R, RT_B, RT_D, CT_NONE, 0},
    [0x43] = {IN_LD, AM_R_R, RT_B, RT_E, CT_NONE, 0},
    [0x44] = {IN_LD, AM_R_R, RT_B, RT_H, CT_NONE, 0},
    [0x45] = {IN_LD, AM_R_R, RT_B, RT_L, CT_NONE, 0},
    [0x46] = {IN_LD, AM_R_MR, RT_B, RT_HL, CT_NONE, 0},
    [0x47] = {IN_LD, AM_R_R, RT_B, RT_A, CT_NONE, 0},
    [0x48] = {IN_LD, AM_R_R, RT_C, RT_B, CT_NONE, 0},
    [0x49] = {IN_LD, AM_R_R, RT_C, RT_C, CT_NONE, 0},
    [0x4A] = {IN_LD, AM_R_R, RT_C, RT_D, CT_NONE, 0},
    [0x4B] = {IN_LD, AM_R_R, RT_C, RT_E, CT_NONE, 0},
    [0x4C] = {IN_LD, AM_R_R, RT_C, RT_H, CT_NONE, 0},
    [0x4D] = {IN_LD, AM_R_R, RT_C, RT_L, CT_NONE, 0},
    [0x4E] = {IN_LD, AM_R_MR, RT_C, RT_HL, CT_NONE, 0},
    [0x4F] = {IN_LD, AM_R_R, RT_C, RT_A, CT_NONE, 0},

    // 0x50 - 0x5F
    [0x50] = {IN_LD, AM_R_R, RT_D, RT_B, CT_NONE, 0},
    [0x51] = {IN_LD, AM_R_R, RT_D, RT_C, CT_NONE, 0},
    [0x52] = {IN_LD, AM_R_R, RT_D, RT_D, CT_NONE, 0},
    [0x53] = {IN_LD, AM_R_R, RT_D, RT_E, CT_NONE, 0},
    [0x54] = {IN_LD, AM_R_R, RT_D, RT_H, CT_NONE, 0},
    [0x55] = {IN_LD, AM_R_R, RT_D, RT_L, CT_NONE, 0},
    [0x56] = {IN_LD, AM_R_MR, RT_D, RT_HL, CT_NONE, 0},
    [0x57] = {IN_LD, AM_R_R, RT_D, RT_A, CT_NONE, 0},
    [0x58] = {IN_LD, AM_R_R, RT_E, RT_B, CT_NONE, 0},
    [0x59] = {IN_LD, AM_R_R, RT_E, RT_C, CT_NONE, 0},
    [0x5A] = {IN_LD, AM_R_R, RT_E, RT_D, CT_NONE, 0},
    [0x5B] = {IN_LD, AM_R_R, RT_E, RT_E, CT_NONE, 0},
    [0x5C] = {IN_LD, AM_R_R, RT_E, RT_H, CT_NONE, 0},
    [0x5D] = {IN_LD, AM_R_R, RT_E, RT_L, CT_NONE, 0},
    [0x5E] = {IN_LD, AM_R_MR, RT_E, RT_HL, CT_NONE, 0},
    [0x5F] = {IN_LD, AM_R_R, RT_E, RT_A, CT_NONE, 0},

    // 0x60 - 0x6F
    [0x60] = {IN_LD, AM_R_R, RT_H, RT_B, CT_NONE, 0},
    [0x61] = {IN_LD, AM_R_R, RT_H, RT_C, CT_NONE, 0},
    [0x62] = {IN_LD, AM_R_R, RT_H, RT_D, CT_NONE, 0},
    [0x63] = {IN_LD, AM_R_R, RT_H, RT_E, CT_NONE, 0},
    [0x64] = {IN_LD, AM_R_R, RT_H, RT_H, CT_NONE, 0},
    [0x65] = {IN_LD, AM_R_R, RT_H, RT_L, CT_NONE, 0},
    [0x66] = {IN_LD, AM_R_MR, RT_H, RT_HL, CT_NONE, 0},
    [0x67] = {IN_LD, AM_R_R, RT_H, RT_A, CT_NONE, 0},
    [0x68] = {IN_LD, AM_R_R, RT_L, RT_B, CT_NONE, 0},
    [0x69] = {IN_LD, AM_R_R, RT_L, RT_C, CT_NONE, 0},
    [0x6A] = {IN_LD, AM_R_R, RT_L, RT_D, CT_NONE, 0},
    [0x6B] = {IN_LD, AM_R_R, RT_L, RT_E, CT_NONE, 0},
    [0x6C] = {IN_LD, AM_R_R, RT_L, RT_H, CT_NONE, 0},
    [0x6D] = {IN_LD, AM_R_R, RT_L, RT_L, CT_NONE, 0},
    [0x6E] = {IN_LD, AM_R_MR, RT_L, RT_HL, CT_NONE, 0},
    [0x6F] = {IN_LD, AM_R_R, RT_L, RT_A, CT_NONE, 0},

    // 0x70 - 0x7F
    [0x70] = {IN_LD, AM_MR_R, RT_HL, RT_B, CT_NONE, 0},
    [0x71] = {IN_LD, AM_MR_R, RT_HL, RT_C, CT_NONE, 0},
    [0x72] = {IN_LD, AM_MR_R, RT_HL, RT_D, CT_NONE, 0},
    [0x73] = {IN_LD, AM_MR_R, RT_HL, RT_E, CT_NONE, 0},
    [0x74] = {IN_LD, AM_MR_R, RT_HL, RT_H, CT_NONE, 0},
    [0x75] = {IN_LD, AM_MR_R, RT_HL, RT_L, CT_NONE, 0},
    [0x76] = {IN_HALT, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0x77] = {IN_LD, AM_MR_R, RT_HL, RT_A, CT_NONE, 0},
    [0x78] = {IN_LD, AM_R_R, RT_A, RT_B, CT_NONE, 0},
    [0x79] = {IN_LD, AM_R_R, RT_A, RT_C, CT_NONE, 0},
    [0x7A] = {IN_LD, AM_R_R, RT_A, RT_D, CT_NONE, 0},
    [0x7B] = {IN_LD, AM_R_R, RT_A, RT_E, CT_NONE, 0},
    [0x7C] = {IN_LD, AM_R_R, RT_A, RT_H, CT_NONE, 0},
    [0x7D] = {IN_LD, AM_R_R, RT_A, RT_L, CT_NONE, 0},
    [0x7E] = {IN_LD, AM_R_MR, RT_A, RT_HL, CT_NONE, 0},
    [0x7F] = {IN_LD, AM_R_R, RT_A, RT_A, CT_NONE, 0},

    // 0xA0 - 0xAF
    [0xAF] = {IN_XOR, AM_R, RT_A, RT_NONE, CT_NONE, 0},

    // 0xC0 - 0xCF
    [0xC3] = {IN_JP, AM_D16, RT_NONE, RT_NONE, CT_NONE, 0},

    // 0xE0 - 0xEF
    [0xE0] = {IN_LDH, AM_A8_R, RT_NONE, RT_A, CT_NONE, 0},
    [0xE2] = {IN_LD, AM_MR_R, RT_C, RT_A, CT_NONE, 0},
    [0xEA] = {IN_LD, AM_A16_R, RT_NONE, RT_A, CT_NONE, 0},

    // 0xF0 - 0xFF
    [0xF0] = {IN_LDH, AM_R_A8, RT_A, RT_NONE, CT_NONE, 0},
    [0xF2] = {IN_LD, AM_R_MR, RT_A, RT_C, CT_NONE, 0},
    [0xF3] = {IN_DI, AM_IMP, RT_NONE, RT_NONE, CT_NONE, 0},
    [0xFA] = {IN_LD, AM_R_A16, RT_A, RT_NONE, CT_NONE, 0}

};

char *instr_name(in_type type) { return instruction_name[type]; }

instruction *instruction_by_opcode(uint8_t opcode) {
    if (instructions[opcode].type == IN_NONE) {
        return NULL;
    }

    return &instructions[opcode];
}
