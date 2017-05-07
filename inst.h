#include <stdlib.h>
#include "memory.h"

#ifndef _INST_H_
#define _INST_H_

class Inst {
  public:
    const char * TypeNames[56] = {"ADC", "AND", "ASL", "BIT", "BPL", "BMI", "BVC", "BVS", "BCC", "BCS", "BNE", "BEQ", "BRK", "CMP", "CPX", "CPY", "DEC", "EOR", "CLC", "SEC", "CLI", "SEI", "CLV", "CLD", "SED", "INC", "JMP", "JSR", "LDA", "LDX", "LDY", "LSR", "NOP", "ORA", "TAX", "TXA", "DEX", "INX", "TAY", "TYA", "DEY", "INY", "ROL", "ROR", "RTI", "RTS", "SBC", "STA", "TXS", "TSX", "PHA", "PLA", "PHP", "PLP", "STX", "STY"};
    enum Type { ADC, AND, ASL, BIT, BPL, BMI, BVC, BVS, BCC, BCS, BNE, BEQ, BRK, CMP, CPX, CPY, DEC, EOR, CLC, SEC, CLI, SEI, CLV, CLD, SED, INC, JMP, JSR, LDA, LDX, LDY, LSR, NOP, ORA, TAX, TXA, DEX, INX, TAY, TYA, DEY, INY, ROL, ROR, RTI, RTS, SBC, STA, TXS, TSX, PHA, PLA, PHP, PLP, STX, STY};
    enum Admode {ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, ACCUMULATOR, IMMEDIATE, INDIRECT, INDIRECT_X, INDEXED_ABSOLUTE, INDIRECT_Y, ZERO_PAGE_X, ZERO_PAGE_Y, NONE, RELATIVE, ZERO_PAGE};

    Inst (Type, Admode);

    Type type;
    Admode admode;
    u_int16_t adr;
    u_int8_t val;
};

Inst parseInstruction(int pos, Memory & memory);
void loadVal(Inst & inst, Memory & memory, u_int8_t X, u_int8_t Y, u_int8_t A, int pos);


#endif