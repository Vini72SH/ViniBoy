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

    [0x00] = {IN_NOP, AM_IMP},

    [0x05] = {IN_DEC, AM_R, RT_B},

    [0x0E] = {IN_LD, AM_R_D8, RT_C},

    [0xAF] = {IN_XOR, AM_R, RT_A},

    [0xC3] = {IN_JP, AM_D16},

    [0xF3] = {IN_DI}

};

char *instr_name(in_type type) { return instruction_name[type]; }

instruction *instruction_by_opcode(uint8_t opcode) {
    if (instructions[opcode].type == IN_NONE) {
        return NULL;
    }

    return &instructions[opcode];
}
