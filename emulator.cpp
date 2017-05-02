#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "nes.h"
#include "flags.h"
#include "inst.h"
#include "memory.h"

// ROM associated variables
unsigned char * prg;
unsigned char * chr;
int Prg_Size;
int Chr_Size;

// Hardware associated variables
// TODO: Might have to initialize variables
int pc;
int8_t X;
int8_t Y;
int8_t A;
Flags flags;
// TODO load program into memory, account for PRG-ROM, SRAM, Expansion ROM, I/O Registers
Memory memory;


using namespace std;

void incrementPc(Inst inst) {
  switch (inst.admode) {
    case Inst::ABSOLUTE:
      pc += 3;
      break;
    case Inst::ABSOLUTE_X:
      pc += 3;
      break;
    case Inst::ABSOLUTE_Y:
      pc += 3;
      break;
    case Inst::ACCUMULATOR:
      pc += 1;
      break;
    case Inst::IMMEDIATE:
      pc += 2;
      break;
    case Inst::INDIRECT:
      pc += 3;
      break;
    case Inst::INDIRECT_X:
      pc += 2;
      break;
    case Inst::INDEXED_ABSOLUTE:
      pc += 3;
      break;
    case Inst::INDIRECT_Y:
      pc += 2;
      break;
    case Inst::ZERO_PAGE_X:
      pc += 2;
      break;
    case Inst::ZERO_PAGE_Y:
      pc += 2;
      break;
    case Inst::NONE:
      pc += 1;
      break;
    case Inst::RELATIVE:
      break;
      pc += 2;
    case Inst::ZERO_PAGE:
      pc += 2;
      break;
    default:
      cout << "GOING TO DEFAULT - THIS PROBABLY ISN'T GOOD" << "\n";
      break;
  }
}


void execute(Inst inst) {
  int8_t val = inst.val;
  u_int16_t adr = inst.adr;
  Inst::Admode admode = inst.admode;
  switch (inst.type) {
    case Inst::ADC:
    {
      u_int8_t carry = flags.getCB();
      // TODO: this logic might not be right
      int16_t notoverflow = ((int16_t) A) + ((int16_t) val) + carry;
      // TODO: this logic definitely not right
      u_int16_t notcarry = ((u_int16_t) A) + ((u_int16_t) val) + carry;

      A = A + val + carry;
      flags.autoSetS(A);
      flags.setV(notoverflow != A);
      flags.autoSetZ(A);
      flags.setC(notcarry != ((u_int8_t) A));
      incrementPc(inst);
      break;
    }
    case Inst::AND:
    {
      A = A & val;
      flags.autoSetZ(A);
      flags.autoSetS(A);
      incrementPc(inst);
      break;
    }
    case Inst::ASL:
    {
      
      flags.setCB(val & 0x80);
      // TODO is this actually an arithmetic shift
      val = val << 1;
      if (admode == Inst::ACCUMULATOR) {
        A = val;
        flags.autoSetS(A);
        flags.autoSetZ(A);
      } else {
        memory.write(val, adr);
        flags.autoSetS(memory.read(adr));
        flags.autoSetZ(memory.read(adr));
      }
      incrementPc(inst);
      break;
    }
    case Inst::BIT:
    {
      // TODO Probably bug
      flags.setSB(0x80 & val);
      flags.setVB(0x40 & val);
      flags.setZB(val & A);
      incrementPc(inst);
      break;
    }
    case Inst::BPL:
    {
      if (!flags.getS()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BMI:
    {
      if(flags.getS()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BVC:
    {
      if(!flags.getV()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BVS:
    {
      if(flags.getV()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BCC:
    {
      if(!flags.getC()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BCS:
    {
      if(flags.getC()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BNE:
    {
      if(!flags.getZ()) {
        pc += val;
      } else {
       incrementPc(inst);
      }
      break;
    }
    case Inst::BEQ:
    {
      if(flags.getZ()) {
        pc += val;
      } else {
        incrementPc(inst);
      }
      break;
    }
    case Inst::BRK:
    {
      flags.setB(true);
      incrementPc(inst);
      break;
    }
    case Inst::CMP:
    {
      flags.setC(A >= val);
      flags.autoSetZ(A - val);
      flags.autoSetS(A - val);
      incrementPc(inst);
      break;
    }
    case Inst::CPX:
    {
      flags.setC(X >= val);
      flags.autoSetZ(X - val);
      flags.autoSetS(X - val);
      incrementPc(inst);
      break;
    }
    case Inst::CPY:
    {
      flags.setC(Y >= val);
      flags.autoSetZ(Y - val);
      flags.autoSetS(Y - val);
      incrementPc(inst);
      break;
    }
    case Inst::DEC:
    {
      memory.write(memory.read(adr) - 1, adr);
      flags.autoSetS(memory.read(adr));
      flags.autoSetZ(memory.read(adr));
      incrementPc(inst);
      break;
    }
    case Inst::EOR:
    {
      A = A ^ val;
      flags.autoSetS(A);
      flags.autoSetZ(A);
      incrementPc(inst);
      break;
    }
    case Inst::CLC:
    {
      flags.setC(false);
      incrementPc(inst);
      break;
    }
    case Inst::SEC:
    {
      flags.setC(true);
      incrementPc(inst);
      break;
    }
    case Inst::CLI:
    {
      flags.setI(false);
      incrementPc(inst);
      break;
    }
    case Inst::SEI:
    {
      flags.setI(true);
      incrementPc(inst);
      break;
    }
    case Inst::CLV:
    {
      flags.setV(false);
      incrementPc(inst);
      break;
    }
    case Inst::CLD:
    {
      flags.setD(false);
      incrementPc(inst);
      break;
    }
    case Inst::SED:
    {
      flags.setD(true);
      incrementPc(inst);
      break;
    }
    case Inst::INC:
    {
      memory.write(memory.read(adr) + 1, adr);
      flags.autoSetS(memory.read(adr));
      flags.autoSetZ(memory.read(adr));
      incrementPc(inst);
      break;
    }
    case Inst::JMP:
    {
      pc = adr;
      break;
    }
    case Inst::JSR:
    {
      memory.push((u_int8_t) (pc + 2));
      memory.push((u_int8_t) ((pc + 2) >> 8));
      pc = adr;
      break;
    }
    case Inst::LDA:
    {
      A = val;
      printf("%d\n", val);
      flags.autoSetS(A);
      flags.autoSetZ(A);
      incrementPc(inst);
      break;
    }
    case Inst::LDX:
      {
      X = val;
      flags.autoSetS(X);
      flags.autoSetZ(X);
      incrementPc(inst);
      break;
      }
    case Inst::LDY:
      {
      Y = val;
      flags.autoSetS(Y);
      flags.autoSetZ(Y);
      incrementPc(inst);
      break;
      }
    case Inst::LSR:
      {
        flags.setCB(0x1 & val);
        // TODO is this actually a logical shift
        val = val >> 1;
        if (admode == Inst::ACCUMULATOR) {
          A = val;
          flags.autoSetS(A);
          flags.autoSetZ(A);
        } else {
          memory.write(val, adr);
          flags.autoSetS(memory.read(adr));
          flags.autoSetZ(memory.read(adr));
        }
        incrementPc(inst);
        break;
      }
    case Inst::NOP:
      {
      incrementPc(inst);
      break;
      }
    case Inst::ORA:
      {
        A = A | val;
        flags.autoSetS(A);
        flags.autoSetZ(A);
        incrementPc(inst);
        break;
      }
    case Inst::TAX:
      {
      X = A;
      flags.autoSetS(X);
      flags.autoSetZ(X);
      incrementPc(inst);
      break;
      }
    case Inst::TXA:
      {
        A = X;
        flags.autoSetS(A);
        flags.autoSetZ(A);
        incrementPc(inst);
        break;
      }
    case Inst::DEX:
      {
        X--;
        flags.autoSetS(X);
        flags.autoSetZ(X);
        incrementPc(inst);
        break;
      }
    case Inst::INX:
      {
        X++;
        flags.autoSetS(X);
        flags.autoSetZ(X);
        incrementPc(inst);
        break;
      }
    case Inst::TAY:
      {
      Y = A;
      flags.autoSetS(Y);
      flags.autoSetZ(Y);
      incrementPc(inst);
      break;
      }
    case Inst::TYA:
      {
        A = Y;
        flags.autoSetS(A);
        flags.autoSetZ(A);
        incrementPc(inst);
        break;
      }
    case Inst::DEY:
      {
        Y--;
        flags.autoSetS(Y);
        flags.autoSetZ(Y);
        incrementPc(inst);
        break;
      }
    case Inst::INY:
      {
        Y++;
        flags.autoSetS(Y);
        flags.autoSetZ(Y);
        incrementPc(inst);
        break;
      }
    case Inst::ROL:
      {
        u_int8_t carryBit = flags.getCB();
        flags.setCB(val & 0x80);
        val = val << 1;
        val += carryBit;
        if (admode == Inst::ACCUMULATOR) {
          A = val;
          flags.autoSetS(A);
          flags.autoSetZ(A);
        } else {
          memory.write(val, adr);
          flags.autoSetS(memory.read(adr));
          flags.autoSetZ(memory.read(adr));
        }
        incrementPc(inst);
        break;
      }
    case Inst::ROR:
      {
        u_int8_t carryBit = flags.getCB();
        flags.setCB(val & 0x1);
        val = val >> 1;
        val += (carryBit << 7);
        if (admode == Inst::ACCUMULATOR) {
          A = val;
          flags.autoSetS(A);
          flags.autoSetZ(A);
        } else {
          memory.write(val, adr);
          flags.autoSetS(memory.read(adr));
          flags.autoSetZ(memory.read(adr));
        }
        incrementPc(inst);
        break;
      }
    case Inst::RTI:
      {
        memory.popFlags(flags);
        // TODO this might be only 1 byte
        u_int16_t newAdr = memory.pop();
        newAdr = newAdr << 8;
        newAdr += memory.pop();
        pc = newAdr;
        break;
      }
    case Inst::RTS:
      {
        u_int16_t newAdr = memory.pop();
        newAdr = newAdr << 8;
        newAdr += memory.pop();
        pc = newAdr + 1;
        // TODO does this need flags
        break;
      }
    case Inst::SBC:
      {
        u_int8_t borrow = flags.getCB();
        A = A - val - borrow;

        // TODO: this logic might not be right
        int16_t notoverflow = ((int16_t) A) - ((int16_t) val) - borrow;
        // TODO: this logic definitely not right
        u_int16_t notcarry = ((u_int16_t) A) - ((u_int16_t) val) - borrow;

        flags.autoSetZ(A);
        flags.autoSetS(A);
        flags.setV(notoverflow != A);
        flags.setC(notcarry != (u_int8_t) A);
        incrementPc(inst);
        break;
      }

    case Inst::STA:
      {
        memory.write(A, val);
        incrementPc(inst);
        break;
      }

    case Inst::TXS:
      {
        memory.SP = X;
        incrementPc(inst);
        break;
      }

    case Inst::TSX:
      {
        X = memory.SP;
        incrementPc(inst);
        break;
      }

    case Inst::PHA:
      {
        memory.push(A);
        incrementPc(inst);
        break;
      }

    case Inst::PLA:
      {
        A = memory.pop();
        incrementPc(inst);
        break;
      }

    case Inst::PHP:
      {
        memory.pushFlags(flags);
        incrementPc(inst);
        break;
      }

    case Inst::PLP:
      {
        memory.popFlags(flags);
        incrementPc(inst);
        break;
      }

    case Inst::STX:
      {
        memory.write(X, adr);
        incrementPc(inst);
        break;
      }

    case Inst::STY:
      {
        memory.write(Y, adr);
        incrementPc(inst);
        break;
      }

    default:
      cout << "Invalid command\n";
      break;
  }
}


void emulate() {
  int numToRun = 100;
  for(int i = 0; i < numToRun; i++) {
    Inst inst = parseInstruction(pc, memory);
    loadVal(inst, memory, X, Y, A, pc);

    cout << hex << pc << ":\t";
    cout << inst.TypeNames[inst.type];
    cout << "\n";

    execute(inst);

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
  unsigned char * buffer = (unsigned char * ) malloc(sizeof(unsigned char) * lSize);
  fread(buffer, 1, lSize, file);

  Nes_Hdr * header = (Nes_Hdr *) (buffer);

  // Convert sizes to # bytes
  Prg_Size = header->Prg_Size * 16384;
  Chr_Size = header->Chr_Size * 8192;

  // TODO: Account for trainer
  prg = (unsigned char *) malloc(sizeof(unsigned char) * Prg_Size);
  chr = (unsigned char *) malloc(sizeof(unsigned char) * Chr_Size);
  // Read into PRG and CHR rom
  for (int i = 0; i < Prg_Size; i++) {
    prg[i] = buffer[i + Nes_HeaderSize];
  }
  for (int i = 0; i < Chr_Size; i++) {
    chr[i] = buffer[i + Nes_HeaderSize + Prg_Size];
  }

  memory.setup(prg, Prg_Size);
  pc = 0x8000; // TODO this isn't always what you want I think
  emulate();
}

