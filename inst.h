#include <stdlib.h>

class Inst {

  void readVal();

  public:
    enum Type { ADC, AND, ASL, BIT, BPL, BMI, BVC, BVS, BCC, BCS, BNE, BEQ, BRK, CMP, CPX, CPY, DEC, EOR, CLC, SEC, CLI, SEI, CLV, CLD, SED, INC, JMP, JSR, LDA, LDX, LDY, LSR, NOP, ORA, TAX, TXA, DEX, INX, TAY, TYA, DEY, INY, ROL, ROR, RTI, RTS, SBC, STA, TXS, TSX, PHA, PLA, PHP, PLP, STX, STY};
    enum Admode {ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, ACCUMULATOR, IMMEDIATE, INDIRECT, INDIRECT_X, INDEXED_ABSOLUTE, INDIRECT_Y, ZERO_PAGE_X, ZERO_PAGE_Y, NONE, RELATIVE, ZERO_PAGE};
    void execute();

    Inst (Type, Admode, int pos, unsigned char * buffer);

    Type type;
    Admode admode;
    int pos;

    unsigned char * buffer;

    uint16_t adr;
    int8_t val;
};

Inst parseInstruction(int pos, unsigned char * buffer);