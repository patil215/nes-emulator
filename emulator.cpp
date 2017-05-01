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
    enum Type { ADC, AND, ASL, BIT, BPL, BMI, BVC, BVS, BCC, BCS, BNE, BEQ, BRK, CMP, CPX, CPY, DEC, EOR, CLC, SEC, CLI, SEI, CLV, CLD, SED, INC, JMP, JSR, LDA, LDX, LDY, LSR, NOP, ORA, TAX, TXA, DEX, INX, TAY, TYA, DEY, INY};
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
  memory[SP] = value;
  SP--;
}

void Inst::execute() {
  switch (type) {
    case ADC:
    {
      // TODO: this logic might not be right
      int16_t notoverflow = ((int16_t) A) + ((int16_t) val) + flags[0];
      u_int16_t notcarry = ((u_int16_t) A) + ((u_int16_t) val) + flags[0];

      A = A + val + flags[0];
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
      // TODO this one is messed up because it directly shifts memory
      //setCFlag(A & 0x80);
      //A = A << 1;
      incrementPc();
      break;
    }
    case BIT:
    {
      // Probably bug
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
      // TODO
      incrementPc();
      break;
    }
    case EOR:
    {
      // TODO
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
      // TODO
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
      push(pc + 2);
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
        // TODO
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
      // TODO this might need a special case
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

