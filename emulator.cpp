#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "nes.h"
#include "flags.h"
#include "inst.h"
#include "memory.h"

// ROM associated variables
u_int8_t * prg;
int Prg_Size;

// Hardware associated variables
// TODO: Might have to initialize variables
u_int16_t pc;
u_int8_t X;
u_int8_t Y;
u_int8_t A;
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
      pc += 2;
      break;
    case Inst::ZERO_PAGE:
      pc += 2;
      break;
    default:
      cout << "GOING TO DEFAULT - THIS PROBABLY ISN'T GOOD" << "\n";
      break;
  }
}


void execute(Inst inst) {
  u_int8_t val = inst.val;
  u_int16_t adr = inst.adr;
  Inst::Admode admode = inst.admode;
  switch (inst.type) {
    case Inst::ADC:
    {
      u_int8_t carry = flags.getCB();
      u_int8_t result = A + val + carry;

      A = result;

      bool isOverflow = ((A ^ result) & (val ^ result) & 0x80) != 0;
      bool isCarry = (((u_int16_t) A) + ((u_int16_t) val) + ((u_int16_t) carry)) > 255;

      flags.setV(isOverflow);
      flags.setC(isCarry);
      flags.autoSetS(result);
      flags.autoSetZ(result);

      incrementPc(inst);
      break;
    }
    case Inst::AND:
    {
      u_int8_t result = A & val;

      A = result;

      flags.autoSetZ(result);
      flags.autoSetS(result);

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
      flags.setSB(0x80 & val);
      flags.setVB(0x40 & val);
      flags.autoSetZ(val & A);
      incrementPc(inst);
      break;
    }
    case Inst::BPL:
    {
      if (!flags.getS()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BMI:
    {
      if(flags.getS()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BVC:
    {
      if(!flags.getV()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BVS:
    {
      if(flags.getV()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BCC:
    {
      if(!flags.getC()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BCS:
    {
      if(flags.getC()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BNE:
    {
      if(!flags.getZ()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BEQ:
    {
      if(flags.getZ()) {
        pc += ((int8_t) val);
      }
      incrementPc(inst);
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
      // TODO
      flags.setC(A >= val);
      flags.autoSetZ(A - val);
      flags.autoSetS(A - val);
      incrementPc(inst);
      break;
    }
    case Inst::CPX:
    {
      // TODO
      flags.setC(X >= val);
      flags.autoSetZ(X - val);
      flags.autoSetS(X - val);
      incrementPc(inst);
      break;
    }
    case Inst::CPY:
    {
      // TODO
      flags.setC(Y >= val);
      flags.autoSetZ(Y - val);
      flags.autoSetS(Y - val);
      incrementPc(inst);
      break;
    }
    case Inst::DEC:
    {
      u_int8_t result = memory.read(adr) - 1;
      memory.write(result, adr);
      flags.autoSetS(result);
      flags.autoSetZ(result);
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
      u_int8_t result = memory.read(adr) + 1;
      memory.write(result, adr);
      flags.autoSetS(result);
      flags.autoSetZ(result);
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
      flags.autoSetS(val);
      flags.autoSetZ(val);
      incrementPc(inst);
      break;
    }
    case Inst::LDX:
      {
      X = val;
      flags.autoSetS(val);
      flags.autoSetZ(val);
      incrementPc(inst);
      break;
      }
    case Inst::LDY:
      {
      Y = val;
      flags.autoSetS(val);
      flags.autoSetZ(val);
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
        break;
      }
    case Inst::SBC:
      {
        // SBC of A + B + carry is same as A + (ones complement of B) + carry
        u_int8_t borrow = flags.getCB();

        u_int8_t result = A + (255 - val) + borrow;

        bool isOverflow = ((A ^ result) & ((255 - val) ^ result) & 0x80) != 0;
        bool isCarry = (((u_int16_t) A) + ((u_int16_t) (255 - val)) + ((u_int16_t) borrow)) > 255;

        A = result;

        flags.setV(isOverflow);
        flags.setC(isCarry);
        flags.autoSetS(result);
        flags.autoSetZ(result);

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
  int numToRun = 1000;
  for(int i = 0; i < numToRun; i++) {
    Inst inst = parseInstruction(pc, memory);
    loadVal(inst, memory, X, Y, A, pc);

    cout << hex << pc << ":\t";
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
  u_int8_t * buffer = (u_int8_t * ) malloc(sizeof(u_int8_t) * lSize);
  fread(buffer, 1, lSize, file);
  prg = (u_int8_t *) malloc(sizeof(u_int8_t) * Prg_Size);

  for (int i = 0; i < lSize; i++) {
    prg[i] = buffer[i];
  }

  int start = 0xF000;
  memory.setup(prg, lSize, start);
  pc = start; // TODO this may not necessarily be what is wanted
  emulate();
}

