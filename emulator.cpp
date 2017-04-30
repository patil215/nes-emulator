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
int8_t memory[0x800];


using namespace std;


class Inst {

  void readVal();
  void incrementPc();

  public:
    enum Type { ADD, BRK, JSR, LDX, LDY, TAX, TAY, NOP};
    enum Admode {ABSOLUTE, ABSOLUTE_X, ABSOLUTE_Y, ACCUMULATOR, IMMEDIATE, INDIRECT, INDIRECT_INDEXED, INDEXED_ABSOLUTE, INDEXED_INDIRECT, ZERO_PAGE_X, ZERO_PAGE_Y, NONE, RELATIVE, ZERO_PAGE};
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

void setSFlag(int8_t result) {
  if(result < 0) {
    flags[7] = 1;
  } else {
    flags[7] = 0;
  }
}

void setZFlag(int8_t result) {
  if(result == 0) {
    flags[1] = 1;
  } else {
    flags[1] = 0;
  }
}

void Inst::incrementPc() {
  switch (admode) {
    case ZERO_PAGE:
      pc += 2;
      break;
    case NONE:
      pc += 1;
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
    case JSR:
      push(pc + 2);
      pc = adr;
      break;
    case LDX:
      X = val;
      setSFlag(X);
      setZFlag(X);
      incrementPc();
      break;
    case LDY:
      Y = val;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc();
      break;
    case NOP:
      incrementPc();
      break;
    case TAX:
      X = A;
      setSFlag(X);
      setZFlag(X);
      incrementPc();
      break;
    case TAY:
      Y = A;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc();
      break;
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
    case INDEXED_INDIRECT:
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
    case INDIRECT_INDEXED:
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
      // TODO this needs a special case
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
    case 0x20:
      return Inst(Inst::JSR, Inst::ABSOLUTE, pos, buffer);
    case 0xA6:
      // TODO: does this load the value or memory[value]?
      return Inst(Inst::LDX, Inst::ZERO_PAGE, pos, buffer);
    case 0xA4:
      return Inst(Inst::LDY, Inst::ZERO_PAGE, pos, buffer);
    case 0xAA:
      return Inst(Inst::TAX, Inst::NONE, pos, buffer);
    case 0xA8:
      return Inst(Inst::TAY, Inst::NONE, pos, buffer);
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

