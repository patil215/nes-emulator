#include <stdlib.h>
#include "memory.h"

Memory::Memory() {
   SP = 0x1FF;
}

void Memory::setup(unsigned char * prg, int prg_size) {
   for (int i = 0; i < prg_size; i++) {
    write(prg[i], i + 0x8000);
  }
}

void Memory::write(int8_t val, u_int16_t adr) {
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
}

int8_t Memory::read(u_int16_t adr) {
	return memory[adr];
}

void Memory::push(int8_t value) {
  SP--;
  memory[SP] = value;
}

int8_t Memory::pop() {
  int8_t value = memory[SP];
  SP++;
  return value;
}

void Memory::pushFlags(Flags flags) {
  u_int8_t f = 0;
  f += flags.getSB();
  f = f << 1;
  f += flags.getVB();
  f = f << 1;
  f = f << 1;
  f += flags.getBB();
  f = f << 1;
  f += flags.getDB();
  f = f << 1;
  f += flags.getIB();
  f = f << 1;
  f += flags.getZB();
  f = f << 1;
  f += flags.getCB();
}

void Memory::popFlags(Flags & flags) {
  u_int8_t f = pop();
  flags.setS((f & 0x80) > 0);
  flags.setV((f & 0x40) > 0);
  flags.setB((f & 0x10) > 0);
  flags.setD((f & 0x8) > 0);
  flags.setI((f & 0x4) > 0);
  flags.setZ((f & 0x2) > 0);
  flags.setC((f & 0x1) > 0);
}