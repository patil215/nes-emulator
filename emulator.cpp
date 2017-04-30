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
bool flags[8];


using namespace std;


class Inst {

  void readVal();
  void incrementPc();

  public:
    enum Type { ADD, BRK, JSR, LDX, LDY, NOP};
    enum Admode {ABSOLUTE, NONE, ZERO_PAGE};
    void execute();

    Inst (Type, Admode, int pos, unsigned char * buffer);

    Type type;
    Admode admode;
    int pos;

    uint16_t a;
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

void Inst::execute() {
  switch (type) {
    case JSR:
      pc = a;
      break;
    case LDX:
      X = a;
      setSFlag(X);
      setZFlag(X);
      incrementPc();
    case LDY:
      Y = a;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc();
      break;
    case NOP:
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
      a = buffer[pos + 2];
      a = a << 8;
      a += buffer[pos + 1];
      break;
    case ZERO_PAGE:
      a = buffer[pos + 1];
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
      return Inst(Inst::LDX, Inst::ZERO_PAGE, pos, buffer);
    case 0xA4:
      return Inst(Inst::LDY, Inst::ZERO_PAGE, pos, buffer);
    default:
      return Inst(Inst::NOP, Inst::NONE, pos, buffer);
  }
}


void emulate() {
  int numToRun = 4;
  for(int i = 0; i < numToRun; i++) {
    Inst inst = parseInstruction(pc);

    if (inst.type == Inst::JSR && inst.admode == Inst::ABSOLUTE) {
      cout << "doing absolute jump to address " << inst.a <<  "\n";
    }

    if (inst.type == Inst::LDY) {
      cout << "loading into register y " << inst.a << "\n";
    }
    
    if (inst.type == Inst::LDX) {
      cout << "loading into register x " << inst.a << "\n";
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

