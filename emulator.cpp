#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "nes.h"

// ROM associated variables
//unsigned char * prg;
//unsigned char * chr;
unsigned char * buffer;


// Hardware associated variables
// TODO: Might have to initialize variables
int pc;
int8_t X;
int8_t Y;
int8_t A;
int SP = 0x1FF;
bool flags[8];
// TODO load program into memory, account for PRG-ROM, SRAM, Expansion ROM, I/O Registers
int8_t memory[0xF00800];


using namespace std;


class Inst {

  void readVal();
  void incrementPc();

  public:
    enum Type { ADC, AND, ASL, BIT, BPL, BMI, BVC, BVS, BCC, BCS, BNE, BEQ, BRK, CMP, CPX, CPY, DEC, EOR, CLC, SEC, CLI, SEI, CLV, CLD, SED, INC, JMP, JSR, LDA, LDX, LDY, LSR, NOP, ORA, TAX, TXA, DEX, INX, TAY, TYA, DEY, INY, ROL, ROR, RTI, RTS, SBC, STA, TXS, TSX, PHA, PLA, PHP, PLP, STX, STY};
    enum Admode {ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, ACCUMULATOR, IMMEDIATE, INDIRECT, INDIRECT_X, INDEXED_ABSOLUTE, INDIRECT_Y, ZERO_PAGE_X, ZERO_PAGE_Y, NONE, RELATIVE, ZERO_PAGE};
    void execute();

    Inst (Type, Admode, int pos, unsigned char * buffer);

    Type type;
    Admode admode;
    int pos;

    uint16_t adr;
    int8_t val;
};


Inst::Inst(Type type_t, Admode admode_t, int pos_t, unsigned char * buffer_t) {
  type = type_t;
  admode = admode_t;
  pos = pos_t;
  buffer = buffer_t;
  readVal();
}

bool getCFlag() {
  return flags[0];
}

bool getZFlag() {
  return flags[1];
}

bool getIFlag() {
  return flags[2];
}

bool getDFlag() {
  return flags[3];
}

bool getBFlag() {
  return flags[4];
}

bool getVFlag() {
  return flags[6];
}

bool getSFlag() {
  return flags[7];
}

void setIFlag(bool result) {
  flags[2] = result;
}

void setDFlag(bool result) {
  flags[3] = result;
}

void setBFlag(bool result) {
  flags[4] = result;
}

void setCFlag(bool result) {
  flags[0] = result;
}

void setVFlag(bool result) {
  flags[6] = result;
}

void setSFlag(int8_t result) {
  if(result < 0) {
    flags[7] = true;
  } else {
    flags[7] = false;
  }
}

void setZFlag(int8_t result) {
  if(result == 0) {
    flags[1] = true;
  } else {
    flags[1] = false;
  }
}


    enum Admode {ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, ACCUMULATOR, IMMEDIATE, INDIRECT, INDIRECT_X, INDEXED_ABSOLUTE, INDIRECT_Y, ZERO_PAGE_X, ZERO_PAGE_Y, NONE, RELATIVE, ZERO_PAGE};
void Inst::incrementPc() {
  switch (admode) {
    case ABSOLUTE:
      pc += 3;
      break;
    case ABSOLUTE_X:
      pc += 3;
      break;
    case ABSOLUTE_Y:
      pc += 3;
      break;
    case ACCUMULATOR:
      pc += 1;
      break;
    case IMMEDIATE:
      pc += 2;
      break;
    case INDIRECT:
      pc += 3;
      break;
    case INDIRECT_X:
      pc += 2;
      break;
    case INDEXED_ABSOLUTE:
      pc += 3;
      break;
    case INDIRECT_Y:
      pc += 2;
      break;
    case ZERO_PAGE_X:
      pc += 2;
      break;
    case ZERO_PAGE_Y:
      pc += 2;
      break;
    case NONE:
      pc += 1;
      break;
    case RELATIVE:
      break;
      pc += 2;
    case ZERO_PAGE:
      pc += 2;
      break;
    default:
      cout << "GOING TO DEFAULT - THIS PROBABLY ISN'T GOOD" << "\n";
      break;
  }
}

void push(int8_t value) {
  SP--;
  memory[SP] = value;
}

int8_t pop() {
  int8_t value = memory[SP];
  SP++;
  return value;
}

void pushFlags() {
  u_int8_t f = 0;
  f += (getSFlag() ? 1 : 0);
  f = f << 1;
  f += (getVFlag() ? 1 : 0);
  f = f << 1;
  f = f << 1;
  f += (getBFlag() ? 1 : 0);
  f = f << 1;
  f += (getDFlag() ? 1 : 0);
  f = f << 1;
  f += (getIFlag() ? 1 : 0);
  f = f << 1;
  f += (getZFlag() ? 1 : 0);
  f = f << 1;
  f += (getCFlag() ? 1 : 0);
}

void popFlags() {
  u_int8_t f = pop();
  if ((f & 0x80) > 0) {
    setSFlag(-1);
  } else {
    setSFlag(1);
  }
  setVFlag((f & 0x40) > 0);
  setBFlag((f & 0x10) > 0);
  setDFlag((f & 0x8) > 0);
  setIFlag((f & 0x4) > 0);
  if ((f & 0x2) > 0) {
    setZFlag(0);
  } else {
    setZFlag(1);
  }
  setCFlag((f & 0x1) > 0);
}

void Inst::execute() {
  // TODO memory mirroring / duplication
  switch (type) {
    case ADC:
    {
      u_int8_t carry = getCFlag() ? 1 : 0;
      // TODO: this logic might not be right
      int16_t notoverflow = ((int16_t) A) + ((int16_t) val) + carry;
      // TODO: this logic definitely not right
      u_int16_t notcarry = ((u_int16_t) A) + ((u_int16_t) val) + carry;

      A = A + val + carry;
      setSFlag(A);
      setVFlag(notoverflow != A);
      setZFlag(A);
      setCFlag(notcarry != ((u_int8_t) A));
      incrementPc();
      break;
    }
    case AND:
    {
      A = A & val;
      setZFlag(A);
      setSFlag(A);
      incrementPc();
      break;
    }
    case ASL:
    {
      
      setCFlag(val & 0x80);
      // TODO is this actually an arithmetic shift
      val = val << 1;
      if (admode == ACCUMULATOR) {
        A = val;
        setSFlag(A);
        setZFlag(A);
      } else {
        memory[adr] = val;
        setSFlag(memory[adr]);
        setZFlag(memory[adr]);
      }
      incrementPc();
      break;
    }
    case BIT:
    {
      // TODO Probably bug
      flags[7] = (0x80 & val) > 0;
      flags[6] = (0x40 & val) > 0;
      setZFlag(val & A);
      incrementPc();
      break;
    }
    case BPL:
    {
      if (!getSFlag()) {
        pc += val;
      } 
      incrementPc();
      break;
    }
    case BMI:
    {
      if(getSFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BVC:
    {
      if(!getVFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BVS:
    {
      if(getVFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BCC:
    {
      if(!getCFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BCS:
    {
      if(getCFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BNE:
    {
      if(!getZFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BEQ:
    {
      if(getZFlag()) {
        pc += val;
      }
      incrementPc();
      break;
    }
    case BRK:
    {
      setBFlag(true);
      incrementPc();
      break;
    }
    case CMP:
    {
      setCFlag(A >= val);
      setZFlag(A - val);
      setSFlag(A - val);
      incrementPc();
      break;
    }
    case CPX:
    {
      setCFlag(X >= val);
      setZFlag(X - val);
      setSFlag(X - val);
      incrementPc();
      break;
    }
    case CPY:
    {
      setCFlag(Y >= val);
      setZFlag(Y - val);
      setSFlag(Y - val);
      incrementPc();
      break;
    }
    case DEC:
    {
      memory[adr]--;
      setSFlag(memory[adr]);
      setZFlag(memory[adr]);
      incrementPc();
      break;
    }
    case EOR:
    {
      A = A ^ val;
      setSFlag(A);
      setZFlag(A);
      incrementPc();
      break;
    }
    case CLC:
    {
      setCFlag(false);
      incrementPc();
      break;
    }
    case SEC:
    {
      setCFlag(true);
      incrementPc();
      break;
    }
    case CLI:
    {
      setIFlag(false);
      incrementPc();
      break;
    }
    case SEI:
    {
      setIFlag(true);
      incrementPc();
      break;
    }
    case CLV:
    {
      setVFlag(false);
      incrementPc();
      break;
    }
    case CLD:
    {
      setDFlag(false);
      incrementPc();
      break;
    }
    case SED:
    {
      setDFlag(true);
      incrementPc();
      break;
    }
    case INC:
    {
      memory[adr]++;
      setSFlag(memory[adr]);
      setZFlag(memory[adr]);
      incrementPc();
      break;
    }
    case JMP:
    {
      pc = adr;
      break;
    }
    case JSR:
    {
      push((u_int8_t) (pc + 2));
      push((u_int8_t) ((pc + 2) >> 8));
      pc = adr;
      break;
    }
    case LDA:
    {
      A = val;
      setSFlag(A);
      setZFlag(A);
      incrementPc();
      break;
    }
    case LDX:
      {
      X = val;
      setSFlag(X);
      setZFlag(X);
      incrementPc();
      break;
      }
    case LDY:
      {
      Y = val;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc();
      break;
      }
    case LSR:
      {
        setCFlag((0x1 & val) > 0);
        // TODO is this actually a logical shift
        val = val >> 1;
        if (admode == ACCUMULATOR) {
          A = val;
          setSFlag(A);
          setZFlag(A);
        } else {
          memory[adr] = val;
          setSFlag(memory[adr]);
          setZFlag(memory[adr]);
        }
        incrementPc();
        break;
      }
    case NOP:
      {
      incrementPc();
      break;
      }
    case ORA:
      {
        A = A | val;
        setSFlag(A);
        setZFlag(A);
        incrementPc();
        break;
      }
    case TAX:
      {
      X = A;
      setSFlag(X);
      setZFlag(X);
      incrementPc();
      break;
      }
    case TXA:
      {
        A = X;
        setSFlag(A);
        setZFlag(A);
        incrementPc();
        break;
      }
    case DEX:
      {
        X--;
        setSFlag(X);
        setZFlag(X);
        incrementPc();
        break;
      }
    case INX:
      {
        X++;
        setSFlag(X);
        setZFlag(X);
        incrementPc();
        break;
      }
    case TAY:
      {
      Y = A;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc();
      break;
      }
    case TYA:
      {
        A = Y;
        setSFlag(A);
        setZFlag(A);
        incrementPc();
        break;
      }
    case DEY:
      {
        Y--;
        setSFlag(Y);
        setZFlag(Y);
        incrementPc();
        break;
      }
    case INY:
      {
        Y++;
        setSFlag(Y);
        setZFlag(Y);
        incrementPc();
        break;
      }
    case ROL:
      {
        u_int8_t carryBit = getCFlag() ? 1 : 0;
        setCFlag((val & 0x80) > 0);
        val = val << 1;
        val += carryBit;
        if (admode == ACCUMULATOR) {
          A = val;
          setSFlag(A);
          setZFlag(A);
        } else {
          memory[adr] = val;
          setSFlag(memory[adr]);
          setZFlag(memory[adr]);
        }
        incrementPc();
        break;
      }
    case ROR:
      {
        u_int8_t carryBit = getCFlag() ? 0x80 : 0;
        setCFlag((val & 0x1) > 0);
        val = val >> 1;
        val += (carryBit << 7);
        if (admode == ACCUMULATOR) {
          A = val;
          setSFlag(A);
          setZFlag(A);
        } else {
          memory[adr] = val;
          setSFlag(memory[adr]);
          setZFlag(memory[adr]);
        }
        incrementPc();
        break;
      }
    case RTI:
      {
        popFlags();
        // TODO this might be only 1 byte
        u_int16_t newAdr = pop();
        newAdr = newAdr << 8;
        newAdr += pop();
        pc = newAdr;
        break;
      }
    case RTS:
      {
        u_int16_t newAdr = pop();
        newAdr = newAdr << 8;
        newAdr += pop();
        pc = newAdr + 1;
        break;
      }
    case SBC:
      {
        u_int8_t borrow = getCFlag() ? 1 : 0;
        A = A - val - borrow;

        // TODO: this logic might not be right
        int16_t notoverflow = ((int16_t) A) - ((int16_t) val) - borrow;
        // TODO: this logic definitely not right
        u_int16_t notcarry = ((u_int16_t) A) - ((u_int16_t) val) - borrow;

        setZFlag(A);
        setSFlag(A);
        setVFlag(notoverflow != A);
        setCFlag(notcarry != (u_int8_t) A);
        incrementPc();
        break;
      }

    case STA:
      {
        memory[adr] = A;
        incrementPc();
        break;
      }

    case TXS:
      {
        SP = X;
        incrementPc();
        break;
      }

    case TSX:
      {
        X = SP;
        incrementPc();
        break;
      }

    case PHA:
      {
        push(A);
        incrementPc();
        break;
      }

    case PLA:
      {
        A = pop();
        incrementPc();
        break;
      }

    case PHP:
      {
        pushFlags();
        incrementPc();
        break;
      }

    case PLP:
      {
        popFlags();
        incrementPc();
        break;
      }

    case STX:
      {
        memory[adr] = X;
        incrementPc();
        break;
      }

    case STY:
      {
        memory[adr] = Y;
        incrementPc();
        break;
      }


    default:
      cout << "Invalid command\n";
      break;
  }
}


void Inst::readVal() {
  switch (admode) {
    case ABSOLUTE:
      adr = buffer[pos + 2];
      adr = adr << 8;
      adr += buffer[pos + 1];
      val = memory[adr];
      break;
    case ACCUMULATOR:
      val = A;
      break;
    case IMMEDIATE:
      val = buffer[pos + 1];
      break;
    case ABSOLUTE_X:
      adr = buffer[pos + 2];
      adr = adr << 8;
      adr += buffer[pos + 1];
      adr += X;
      val = memory[adr];
      break;
    case ABSOLUTE_Y:
      adr = buffer[pos + 2];
      adr = adr << 8;
      adr += buffer[pos + 1];
      adr += Y;
      val = memory[adr];
      break;
    case ZERO_PAGE_X:
      adr = ((int8_t) buffer[pos + 1]) + X;
      val = memory[adr];
      break;
    case ZERO_PAGE_Y:
      adr = ((int8_t) buffer[pos + 1]) + Y;
      val = memory[adr];
      break;
    case INDIRECT_X:
      {
        int8_t op = buffer[pos + 1];
        op += X;
        adr = memory[op + 1];
        adr = adr << 8;
        adr += memory[op];
        val = memory[adr];
        break;
      }
    case INDIRECT:
      {
        u_int16_t address = buffer[pos + 2];
        address = address << 8;
        address += buffer[pos + 1];
        adr = memory[address + 1];
        adr = adr << 8;
        adr += memory[address];
        val = memory[adr];
        break;
      }
    case INDIRECT_Y:
      {
        int8_t op = buffer[pos + 1];
        adr = memory[op + 1];
        adr = adr << 8;
        adr += memory[op];
        adr += Y;
        val = memory[adr];
        break;
      }
    case RELATIVE:
      val = buffer[pos + 1];
      break;
    case ZERO_PAGE:
      adr = buffer[pos + 1];
      val = memory[adr];
      break;
    default:
      break;
  }
}

Inst parseInstruction(int pos) {
  unsigned char code = buffer[pos];
  switch (code) {

    case 0x69:
      return Inst(Inst::ADC, Inst::IMMEDIATE, pos, buffer);
    case 0x65:
      return Inst(Inst::ADC, Inst::ZERO_PAGE, pos, buffer);
    case 0x75:
      return Inst(Inst::ADC, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x6D:
      return Inst(Inst::ADC, Inst::ABSOLUTE, pos, buffer);
    case 0x7D:
      return Inst(Inst::ADC, Inst::ABSOLUTE_X, pos, buffer);
    case 0x79:
      return Inst(Inst::ADC, Inst::ABSOLUTE_Y, pos, buffer);
    case 0x61:
      return Inst(Inst::ADC, Inst::INDIRECT_X, pos, buffer);
    case 0x71:
      return Inst(Inst::ADC, Inst::INDIRECT_Y, pos, buffer);

    case 0x29:
      return Inst(Inst::AND, Inst::IMMEDIATE, pos, buffer);
    case 0x25:
      return Inst(Inst::AND, Inst::ZERO_PAGE, pos, buffer);
    case 0x35:
      return Inst(Inst::AND, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x2D:
      return Inst(Inst::AND, Inst::ABSOLUTE, pos, buffer);
    case 0x3D:
      return Inst(Inst::AND, Inst::ABSOLUTE_X, pos, buffer);
    case 0x39:
      return Inst(Inst::AND, Inst::ABSOLUTE_Y, pos, buffer);
    case 0x21:
      return Inst(Inst::AND, Inst::INDIRECT_X, pos, buffer);
    case 0x31:
      return Inst(Inst::AND, Inst::INDIRECT_Y, pos, buffer);

    case 0x0A:
      return Inst(Inst::ASL, Inst::ACCUMULATOR, pos, buffer);
    case 0x06:
      return Inst(Inst::ASL, Inst::ZERO_PAGE, pos, buffer);
    case 0x16:
      return Inst(Inst::ASL, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x0E:
      return Inst(Inst::ASL, Inst::ABSOLUTE, pos, buffer);
    case 0x1E:
      return Inst(Inst::ASL, Inst::ABSOLUTE_X, pos, buffer);

    case 0x24:
      return Inst(Inst::BIT, Inst::ZERO_PAGE, pos, buffer);
    case 0x2C:
      return Inst(Inst::BIT, Inst::ABSOLUTE, pos, buffer);

    case 0x10:
      return Inst(Inst::BPL, Inst::NONE, pos, buffer);
    case 0x30:
      return Inst(Inst::BMI, Inst::NONE, pos, buffer);
    case 0x50:
      return Inst(Inst::BVC, Inst::NONE, pos, buffer);
    case 0x70:
      return Inst(Inst::BVS, Inst::NONE, pos, buffer);
    case 0x90:
      return Inst(Inst::BCC, Inst::NONE, pos, buffer);
    case 0xB0:
      return Inst(Inst::BCS, Inst::NONE, pos, buffer);
    case 0xD0:
      return Inst(Inst::BNE, Inst::NONE, pos, buffer);
    case 0xF0:
      return Inst(Inst::BEQ, Inst::NONE, pos, buffer);

    case 0x00:
      return Inst(Inst::BRK, Inst::NONE, pos, buffer);

    case 0xC9:
      return Inst(Inst::CMP, Inst::IMMEDIATE, pos, buffer);
    case 0xC5:
      return Inst(Inst::CMP, Inst::ZERO_PAGE, pos, buffer);
    case 0xD5:
      return Inst(Inst::CMP, Inst::ZERO_PAGE_X, pos, buffer);
    case 0xCD:
      return Inst(Inst::CMP, Inst::ABSOLUTE, pos, buffer);
    case 0xDD:
      return Inst(Inst::CMP, Inst::ABSOLUTE_X, pos, buffer);
    case 0xD9:
      return Inst(Inst::CMP, Inst::ABSOLUTE_Y, pos, buffer);
    case 0xC1:
      return Inst(Inst::CMP, Inst::INDIRECT_X, pos, buffer);
    case 0xD1:
      return Inst(Inst::CMP, Inst::INDIRECT_Y, pos, buffer);

    case 0xE0:
      return Inst(Inst::CPX, Inst::IMMEDIATE, pos, buffer);
    case 0xE4:
      return Inst(Inst::CPX, Inst::ZERO_PAGE, pos, buffer);
    case 0xEC:
      return Inst(Inst::CPX, Inst::ABSOLUTE, pos, buffer);

    case 0xC0:
      return Inst(Inst::CPY, Inst::IMMEDIATE, pos, buffer);
    case 0xC4:
      return Inst(Inst::CPY, Inst::ZERO_PAGE, pos, buffer);
    case 0xCC:
      return Inst(Inst::CPY, Inst::ABSOLUTE, pos, buffer);


    case 0xC6:
      return Inst(Inst::DEC, Inst::ZERO_PAGE, pos, buffer);
    case 0xD6:
      return Inst(Inst::DEC, Inst::ZERO_PAGE_X, pos, buffer);
    case 0xCE:
      return Inst(Inst::DEC, Inst::ABSOLUTE, pos, buffer);
    case 0xDE:
      return Inst(Inst::DEC, Inst::ABSOLUTE_X, pos, buffer);

    case 0x49:
      return Inst(Inst::EOR, Inst::IMMEDIATE, pos, buffer);
    case 0x45:
      return Inst(Inst::EOR, Inst::ZERO_PAGE, pos, buffer);
    case 0x55:
      return Inst(Inst::EOR, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x4D:
      return Inst(Inst::EOR, Inst::ABSOLUTE, pos, buffer);
    case 0x5D:
      return Inst(Inst::EOR, Inst::ABSOLUTE_X, pos, buffer);
    case 0x59:
      return Inst(Inst::EOR, Inst::ABSOLUTE_Y, pos, buffer);
    case 0x41:
      return Inst(Inst::EOR, Inst::INDIRECT_X, pos, buffer);
    case 0x51:
      return Inst(Inst::EOR, Inst::INDIRECT_Y, pos, buffer);

    case 0x18:
      return Inst(Inst::CLC, Inst::NONE, pos, buffer);
    case 0x38:
      return Inst(Inst::SEC, Inst::NONE, pos, buffer);
    case 0x58:
      return Inst(Inst::CLI, Inst::NONE, pos, buffer);
    case 0x78:
      return Inst(Inst::SEI, Inst::NONE, pos, buffer);
    case 0xB8:
      return Inst(Inst::CLV, Inst::NONE, pos, buffer);
    case 0xD8:
      return Inst(Inst::CLD, Inst::NONE, pos, buffer);
    case 0xF8:
      return Inst(Inst::SED, Inst::NONE, pos, buffer);


    case 0xE6:
      return Inst(Inst::INC, Inst::ZERO_PAGE, pos, buffer);
    case 0xF6:
      return Inst(Inst::INC, Inst::ZERO_PAGE_X, pos, buffer);
    case 0xEE:
      return Inst(Inst::INC, Inst::ABSOLUTE, pos, buffer);
    case 0xFE:
      return Inst(Inst::INC, Inst::ABSOLUTE_X, pos, buffer);

    case 0x4C:
      return Inst(Inst::JMP, Inst::ABSOLUTE, pos, buffer);
    case 0x6C:
      return Inst(Inst::JMP, Inst::INDIRECT, pos, buffer);

    case 0x20:
      return Inst(Inst::JSR, Inst::ABSOLUTE, pos, buffer);

    case 0xA9:
      return Inst(Inst::LDA, Inst::IMMEDIATE, pos, buffer);
    case 0xA5:
      return Inst(Inst::LDA, Inst::ZERO_PAGE, pos, buffer);
    case 0xB5:
      return Inst(Inst::LDA, Inst::ZERO_PAGE_X, pos, buffer);
    case 0xAD:
      return Inst(Inst::LDA, Inst::ABSOLUTE, pos, buffer);
    case 0xBD:
      return Inst(Inst::LDA, Inst::ABSOLUTE_X, pos, buffer);
    case 0xB9:
      return Inst(Inst::LDA, Inst::ABSOLUTE_Y, pos, buffer);
    case 0xA1:
      return Inst(Inst::LDA, Inst::INDIRECT_X, pos, buffer);
    case 0xB1:
      return Inst(Inst::LDA, Inst::INDIRECT_Y, pos, buffer);

    case 0xA2:
      // TODO: does this load the value or memory[value]?
      return Inst(Inst::LDX, Inst::IMMEDIATE, pos, buffer);
    case 0xA6:
      return Inst(Inst::LDX, Inst::ZERO_PAGE, pos, buffer);
    case 0xB6:
      return Inst(Inst::LDX, Inst::ZERO_PAGE_Y, pos, buffer);
    case 0xAE:
      return Inst(Inst::LDX, Inst::ABSOLUTE, pos, buffer);
    case 0xBE:
      return Inst(Inst::LDX, Inst::ABSOLUTE_Y, pos, buffer);

    case 0xA0:
      return Inst(Inst::LDY, Inst::IMMEDIATE, pos, buffer);
    case 0xA4:
      return Inst(Inst::LDY, Inst::ZERO_PAGE, pos, buffer);
    case 0xB4:
      return Inst(Inst::LDY, Inst::ZERO_PAGE_X, pos, buffer);
    case 0xAC:
      return Inst(Inst::LDY, Inst::ABSOLUTE, pos, buffer);
    case 0xBC:
      return Inst(Inst::LDY, Inst::ABSOLUTE_X, pos, buffer);

    case 0x4A:
      return Inst(Inst::LSR, Inst::ACCUMULATOR, pos, buffer);
    case 0x46:
      return Inst(Inst::LSR, Inst::ZERO_PAGE, pos, buffer);
    case 0x56:
      return Inst(Inst::LSR, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x4E:
      return Inst(Inst::LSR, Inst::ABSOLUTE, pos, buffer);
    case 0x5E:
      return Inst(Inst::LSR, Inst::ABSOLUTE_X, pos, buffer);

    case 0xEA:
      return Inst(Inst::NOP, Inst::NONE, pos, buffer);

    case 0x09:
      return Inst(Inst::ORA, Inst::IMMEDIATE, pos, buffer);
    case 0x05:
      return Inst(Inst::ORA, Inst::ZERO_PAGE, pos, buffer);
    case 0x15:
      return Inst(Inst::ORA, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x0D:
      return Inst(Inst::ORA, Inst::ABSOLUTE, pos, buffer);
    case 0x1D:
      return Inst(Inst::ORA, Inst::ABSOLUTE_X, pos, buffer);
    case 0x19:
      return Inst(Inst::ORA, Inst::ABSOLUTE_Y, pos, buffer);
    case 0x01:
      return Inst(Inst::ORA, Inst::INDIRECT_X, pos, buffer);
    case 0x11:
      return Inst(Inst::ORA, Inst::INDIRECT_Y, pos, buffer);

    case 0xAA:
      return Inst(Inst::TAX, Inst::NONE, pos, buffer);
    case 0x8A:
      return Inst(Inst::TXA, Inst::NONE, pos, buffer);
    case 0xCA:
      return Inst(Inst::DEX, Inst::NONE, pos, buffer);
    case 0xE8:
      return Inst(Inst::INX, Inst::NONE, pos, buffer);
    case 0xA8:
      return Inst(Inst::TAY, Inst::NONE, pos, buffer);
    case 0x98:
      return Inst(Inst::TYA, Inst::NONE, pos, buffer);
    case 0x88:
      return Inst(Inst::DEY, Inst::NONE, pos, buffer);
    case 0xC8:
      return Inst(Inst::INY, Inst::NONE, pos, buffer);

    case 0x2A:
      return Inst(Inst::ROL, Inst::ACCUMULATOR, pos, buffer);
    case 0x26:
      return Inst(Inst::ROL, Inst::ZERO_PAGE, pos, buffer);
    case 0x36:
      return Inst(Inst::ROL, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x2E:
      return Inst(Inst::ROL, Inst::ABSOLUTE, pos, buffer);
    case 0x3E:
      return Inst(Inst::ROL, Inst::ABSOLUTE_X, pos, buffer);

    case 0x6A:
      return Inst(Inst::ROR, Inst::ACCUMULATOR, pos, buffer);
    case 0x66:
      return Inst(Inst::ROR, Inst::ZERO_PAGE, pos, buffer);
    case 0x76:
      return Inst(Inst::ROR, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x6E:
      return Inst(Inst::ROR, Inst::ABSOLUTE, pos, buffer);
    case 0x7E:
      return Inst(Inst::ROR, Inst::ABSOLUTE_X, pos, buffer);

    case 0x40:
      return Inst(Inst::RTI, Inst::NONE, pos, buffer);

    case 0x60:
      return Inst(Inst::RTS, Inst::NONE, pos, buffer);
     
    case 0xE9:
      return Inst(Inst::SBC, Inst::IMMEDIATE, pos, buffer);
    case 0xE5:
      return Inst(Inst::SBC, Inst::ZERO_PAGE, pos, buffer);
    case 0xF5:
      return Inst(Inst::SBC, Inst::ZERO_PAGE_X, pos, buffer);
    case 0xED:
      return Inst(Inst::SBC, Inst::ABSOLUTE, pos, buffer);
    case 0xFD:
      return Inst(Inst::SBC, Inst::ABSOLUTE_X, pos, buffer);
    case 0xF9:
      return Inst(Inst::SBC, Inst::ABSOLUTE_Y, pos, buffer);
    case 0xE1:
      return Inst(Inst::SBC, Inst::INDIRECT_X, pos, buffer);
    case 0xF1:
      return Inst(Inst::SBC, Inst::INDIRECT_Y, pos, buffer);

    case 0x85:
      return Inst(Inst::STA, Inst::ZERO_PAGE, pos, buffer);
    case 0x95:
      return Inst(Inst::STA, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x8D:
      return Inst(Inst::STA, Inst::ABSOLUTE, pos, buffer);
    case 0x9D:
      return Inst(Inst::STA, Inst::ABSOLUTE_X, pos, buffer);
    case 0x99:
      return Inst(Inst::STA, Inst::ABSOLUTE_Y, pos, buffer);
    case 0x81:
      return Inst(Inst::STA, Inst::INDIRECT_X, pos, buffer);
    case 0x91:
      return Inst(Inst::STA, Inst::INDIRECT_Y, pos, buffer);

    case 0x9A:
      return Inst(Inst::TXS, Inst::NONE, pos, buffer);
    case 0xBA:
      return Inst(Inst::TSX, Inst::NONE, pos, buffer);
    case 0x48:
      return Inst(Inst::PHA, Inst::NONE, pos, buffer);
    case 0x68:
      return Inst(Inst::PLA, Inst::NONE, pos, buffer);
    case 0x08:
      return Inst(Inst::PHP, Inst::NONE, pos, buffer);
    case 0x28:
      return Inst(Inst::PLP, Inst::NONE, pos, buffer);

    case 0x86:
      return Inst(Inst::STX, Inst::ZERO_PAGE, pos, buffer);
    case 0x96:
      return Inst(Inst::STX, Inst::ZERO_PAGE_Y, pos, buffer);
    case 0x8E:
      return Inst(Inst::STX, Inst::ABSOLUTE, pos, buffer);

    case 0x84:
      return Inst(Inst::STX, Inst::ZERO_PAGE, pos, buffer);
    case 0x94:
      return Inst(Inst::STX, Inst::ZERO_PAGE_X, pos, buffer);
    case 0x8C:
      return Inst(Inst::STX, Inst::ABSOLUTE, pos, buffer);

    default:
      return Inst(Inst::NOP, Inst::NONE, pos, buffer);
  }
}


void emulate() {
  int numToRun = 8;
  for(int i = 0; i < numToRun; i++) {
    Inst inst = parseInstruction(pc);

    if (inst.type == Inst::JSR && inst.admode == Inst::ABSOLUTE) {
      cout << "doing absolute jump to address " << inst.adr <<  "\n";
    }

    if (inst.type == Inst::LDY) {
      cout << "loading into register y " << (int) inst.val << " with address " << inst.adr << "\n";
    }
    
    if (inst.type == Inst::LDX) {
      cout << "loading into register x " << (int) inst.val << " with address " << inst.adr << "\n";
    }

    if (inst.type == Inst::TAX) {
      cout << "loading into X from accumulator (value " << (int) A << ")\n";
    }

    if (inst.type == Inst::TAY) {
      cout << "loading into Y from accumulator (value " << (int) A << ")\n";
    }

    if (inst.type == Inst::NOP) {
      cout << "no op " << "\n";
    }

    inst.execute();

  }
}

int main(int argc, char * argv[]) {
  // Open file
  char * filename = argv[1];
  FILE * file = fopen(filename, "rb");

  // Read file
  long lSize;
  fseek (file, 0, SEEK_END);
  lSize = ftell (file);
  rewind (file);
  buffer = (unsigned char * ) malloc(sizeof(unsigned char) * lSize);
  fread(buffer, 1, lSize, file);

  Nes_Hdr * header = (Nes_Hdr *) (buffer);

  // Convert sizes to # bytes
  int Prg_Size = header->Prg_Size * 16384;
  int Chr_Size = header->Chr_Size * 8192;


  /*// TODO: Account for trainer
  prg = (unsigned char *) malloc(sizeof(unsigned char) * Prg_Size);
  chr = (unsigned char *) malloc(sizeof(unsigned char) * Chr_Size);

  // Read into PRG and CHR rom
  for (int i = 0; i < Prg_Size; i++) {
    prg[i] = buffer[i + Nes_HeaderSize];
  }
  for (int i = 0; i < Chr_Size; i++) {
    chr[i] = buffer[i + Nes_HeaderSize + Prg_Size];
  }*/

  pc = 16;
  emulate();
}

