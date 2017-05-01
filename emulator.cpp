#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "nes.h"
#include "inst.h"

// ROM associated variables
unsigned char * buffer;
int Prg_Size;
int Chr_Size;

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

void writeMem(int8_t val, u_int16_t adr);
void push(int8_t value) {
  SP--;
  writeMem(value, SP);
}

int8_t pop() {
  int8_t value = memory[SP];
  SP++;
  return value;
}

void writeMem(int8_t val, u_int16_t adr) {
  // Mirroring
  if ((adr >= 0 && adr <= 0x7FF)) {
    memory[adr] = val;
    memory[adr + 0x800] = val;
    memory[adr + 0x1000] = val;
    memory[adr + 0x1800] = val;
    return;
  }
  if ((adr >= 0x800 && adr <= 0xFFF)) {
    memory[adr] = val;
    memory[adr - 0x800] = val;
    memory[adr + 0x800] = val;
    memory[adr + 0x1000] = val;
    return;
  }
  if ((adr >= 0x1000 && adr <= 0x17FF)) {
    memory[adr] = val;
    memory[adr - 0x1000] = val;
    memory[adr - 0x800] = val;
    memory[adr + 0x800] = val;
    return;
  }
  if ((adr >= 0x1800 && adr <= 0x1FFF)) {
    memory[adr] = val;
    memory[adr - 0x1800] = val;
    memory[adr - 0x1000] = val;
    memory[adr - 0x800] = val;
    return;
  }
  if ((adr >= 0x2000 && adr <= 0x3FFF)) {
    for (int i = 0x2000; i <= 0x3FFF; i++) {
      if ((i % 8) == (adr % 8)) {
        memory[i] = val;
      }
    }
    return;
  }
  if ((adr >= 0x8000 && adr <= 0xBFFF)) {
    memory[adr] = val;
    memory[adr + 0xC000] = val;
    return;
  }
  if ((adr >= 0xC000 && adr <= 0xFFFF)) {
    memory[adr] = val;
    memory[adr - 0xC000] = val;
    return;
  }
  memory[adr] = val;
  // TODO single, ASIC, PPU, PRG rom mirroring
}

void setupMem() {
  for (int i = 0; i < Prg_Size; i++) {
    writeMem(buffer[i + 16], i + 0x8000);
  }
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

void execute(Inst inst) {
  int8_t val = inst.val;
  u_int16_t adr = inst.adr;
  Inst::Admode admode = inst.admode;
  switch (inst.type) {
    case Inst::ADC:
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
      incrementPc(inst);
      break;
    }
    case Inst::AND:
    {
      A = A & val;
      setZFlag(A);
      setSFlag(A);
      incrementPc(inst);
      break;
    }
    case Inst::ASL:
    {
      
      setCFlag(val & 0x80);
      // TODO is this actually an arithmetic shift
      val = val << 1;
      if (admode == Inst::ACCUMULATOR) {
        A = val;
        setSFlag(A);
        setZFlag(A);
      } else {
        writeMem(val, adr);
        setSFlag(memory[adr]);
        setZFlag(memory[adr]);
      }
      incrementPc(inst);
      break;
    }
    case Inst::BIT:
    {
      // TODO Probably bug
      flags[7] = (0x80 & val) > 0;
      flags[6] = (0x40 & val) > 0;
      setZFlag(val & A);
      incrementPc(inst);
      break;
    }
    case Inst::BPL:
    {
      if (!getSFlag()) {
        pc += val;
      } 
      incrementPc(inst);
      break;
    }
    case Inst::BMI:
    {
      if(getSFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BVC:
    {
      if(!getVFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BVS:
    {
      if(getVFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BCC:
    {
      if(!getCFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BCS:
    {
      if(getCFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BNE:
    {
      if(!getZFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BEQ:
    {
      if(getZFlag()) {
        pc += val;
      }
      incrementPc(inst);
      break;
    }
    case Inst::BRK:
    {
      setBFlag(true);
      incrementPc(inst);
      break;
    }
    case Inst::CMP:
    {
      setCFlag(A >= val);
      setZFlag(A - val);
      setSFlag(A - val);
      incrementPc(inst);
      break;
    }
    case Inst::CPX:
    {
      setCFlag(X >= val);
      setZFlag(X - val);
      setSFlag(X - val);
      incrementPc(inst);
      break;
    }
    case Inst::CPY:
    {
      setCFlag(Y >= val);
      setZFlag(Y - val);
      setSFlag(Y - val);
      incrementPc(inst);
      break;
    }
    case Inst::DEC:
    {
      writeMem(memory[adr] - 1, adr);
      setSFlag(memory[adr]);
      setZFlag(memory[adr]);
      incrementPc(inst);
      break;
    }
    case Inst::EOR:
    {
      A = A ^ val;
      setSFlag(A);
      setZFlag(A);
      incrementPc(inst);
      break;
    }
    case Inst::CLC:
    {
      setCFlag(false);
      incrementPc(inst);
      break;
    }
    case Inst::SEC:
    {
      setCFlag(true);
      incrementPc(inst);
      break;
    }
    case Inst::CLI:
    {
      setIFlag(false);
      incrementPc(inst);
      break;
    }
    case Inst::SEI:
    {
      setIFlag(true);
      incrementPc(inst);
      break;
    }
    case Inst::CLV:
    {
      setVFlag(false);
      incrementPc(inst);
      break;
    }
    case Inst::CLD:
    {
      setDFlag(false);
      incrementPc(inst);
      break;
    }
    case Inst::SED:
    {
      setDFlag(true);
      incrementPc(inst);
      break;
    }
    case Inst::INC:
    {
      writeMem(memory[adr] + 1, adr);
      setSFlag(memory[adr]);
      setZFlag(memory[adr]);
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
      push((u_int8_t) (pc + 2));
      push((u_int8_t) ((pc + 2) >> 8));
      pc = adr;
      break;
    }
    case Inst::LDA:
    {
      A = val;
      setSFlag(A);
      setZFlag(A);
      incrementPc(inst);
      break;
    }
    case Inst::LDX:
      {
      X = val;
      setSFlag(X);
      setZFlag(X);
      incrementPc(inst);
      break;
      }
    case Inst::LDY:
      {
      Y = val;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc(inst);
      break;
      }
    case Inst::LSR:
      {
        setCFlag((0x1 & val) > 0);
        // TODO is this actually a logical shift
        val = val >> 1;
        if (admode == Inst::ACCUMULATOR) {
          A = val;
          setSFlag(A);
          setZFlag(A);
        } else {
          writeMem(val, adr);
          setSFlag(memory[adr]);
          setZFlag(memory[adr]);
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
        setSFlag(A);
        setZFlag(A);
        incrementPc(inst);
        break;
      }
    case Inst::TAX:
      {
      X = A;
      setSFlag(X);
      setZFlag(X);
      incrementPc(inst);
      break;
      }
    case Inst::TXA:
      {
        A = X;
        setSFlag(A);
        setZFlag(A);
        incrementPc(inst);
        break;
      }
    case Inst::DEX:
      {
        X--;
        setSFlag(X);
        setZFlag(X);
        incrementPc(inst);
        break;
      }
    case Inst::INX:
      {
        X++;
        setSFlag(X);
        setZFlag(X);
        incrementPc(inst);
        break;
      }
    case Inst::TAY:
      {
      Y = A;
      setSFlag(Y);
      setZFlag(Y);
      incrementPc(inst);
      break;
      }
    case Inst::TYA:
      {
        A = Y;
        setSFlag(A);
        setZFlag(A);
        incrementPc(inst);
        break;
      }
    case Inst::DEY:
      {
        Y--;
        setSFlag(Y);
        setZFlag(Y);
        incrementPc(inst);
        break;
      }
    case Inst::INY:
      {
        Y++;
        setSFlag(Y);
        setZFlag(Y);
        incrementPc(inst);
        break;
      }
    case Inst::ROL:
      {
        u_int8_t carryBit = getCFlag() ? 1 : 0;
        setCFlag((val & 0x80) > 0);
        val = val << 1;
        val += carryBit;
        if (admode == Inst::ACCUMULATOR) {
          A = val;
          setSFlag(A);
          setZFlag(A);
        } else {
          writeMem(val, adr);
          setSFlag(memory[adr]);
          setZFlag(memory[adr]);
        }
        incrementPc(inst);
        break;
      }
    case Inst::ROR:
      {
        u_int8_t carryBit = getCFlag() ? 0x80 : 0;
        setCFlag((val & 0x1) > 0);
        val = val >> 1;
        val += (carryBit << 7);
        if (admode == Inst::ACCUMULATOR) {
          A = val;
          setSFlag(A);
          setZFlag(A);
        } else {
          writeMem(val, adr);
          setSFlag(memory[adr]);
          setZFlag(memory[adr]);
        }
        incrementPc(inst);
        break;
      }
    case Inst::RTI:
      {
        popFlags();
        // TODO this might be only 1 byte
        u_int16_t newAdr = pop();
        newAdr = newAdr << 8;
        newAdr += pop();
        pc = newAdr;
        break;
      }
    case Inst::RTS:
      {
        u_int16_t newAdr = pop();
        newAdr = newAdr << 8;
        newAdr += pop();
        pc = newAdr + 1;
        break;
      }
    case Inst::SBC:
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
        incrementPc(inst);
        break;
      }

    case Inst::STA:
      {
        writeMem(A, val);
        incrementPc(inst);
        break;
      }

    case Inst::TXS:
      {
        SP = X;
        incrementPc(inst);
        break;
      }

    case Inst::TSX:
      {
        X = SP;
        incrementPc(inst);
        break;
      }

    case Inst::PHA:
      {
        push(A);
        incrementPc(inst);
        break;
      }

    case Inst::PLA:
      {
        A = pop();
        incrementPc(inst);
        break;
      }

    case Inst::PHP:
      {
        pushFlags();
        incrementPc(inst);
        break;
      }

    case Inst::PLP:
      {
        popFlags();
        incrementPc(inst);
        break;
      }

    case Inst::STX:
      {
        writeMem(X, adr);
        incrementPc(inst);
        break;
      }

    case Inst::STY:
      {
        writeMem(Y, adr);
        memory[adr] = Y;
        incrementPc(inst);
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



void emulate() {
  int numToRun = 100;
  for(int i = 0; i < numToRun; i++) {
    Inst inst = parseInstruction(pc, buffer);

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

    cout << "PC: " << hex << pc << "\n";

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
  buffer = (unsigned char * ) malloc(sizeof(unsigned char) * lSize);
  fread(buffer, 1, lSize, file);

  Nes_Hdr * header = (Nes_Hdr *) (buffer);

  // Convert sizes to # bytes
  Prg_Size = header->Prg_Size * 16384;
  Chr_Size = header->Chr_Size * 8192;


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
  setupMem();
  emulate();
}

