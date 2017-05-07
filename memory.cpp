#include <stdlib.h>
#include "memory.h"
#include <iostream>

Memory::Memory() {
   SP = 0;
}

/*
  Memory Structure:
  0000-002C  TIA Write
  0000-000D  TIA Read (sometimes mirrored at 0030-003D)
  0080-00FF  PIA RAM (128 bytes)
  0280-0297  PIA Ports and Timer
  F000-FFFF  Cartridge Memory (4 Kbytes area)
*/

void Memory::setup(unsigned char * prg, int prg_size, int offset) {
  // Load ROM into memory
  for (int i = 0; i < prg_size; i++) {
    write(prg[i], i + offset);
  }
}


void Memory::writeAndMirror(u_int8_t val, u_int16_t adr, u_int16_t startAddrs[], int startAddrsSize) {
  memory[adr] = val;

  if (startAddrsSize < 2) {
    return;
  }

  // Find where the address occurs
  u_int16_t step = startAddrs[1] - startAddrs[0];
  int index = 0;
  for(int i = 0; i < startAddrsSize; i++) {
    if (adr >= startAddrs[i] && adr < (startAddrs[i] + step)) {
      index = i;
      break;
    }
  }


  // Write all the mirrored addresses under adr
  for (int i = 0; i < index; i++) {
    memory[adr - (step * (i + 1))] = val;\
  }

  // Write all the mirrored address over adr
  for (int i = 0; i < startAddrsSize - index - 1; i++) {
    memory[adr + (step * (i + 1))] = val;
  }

}

void Memory::write(u_int8_t val, u_int16_t adr) {
  // Default case: no other mirroring
  // All memory is mirrored in steps of 2000h (ie. at 2000h-3FFFh, 4000h-5FFFh, and so on, up to E000h-FFFFh)
  u_int16_t memoryMirrorAddrs[] = { 0x0000, 0x2000, 0x4000, 0x6000, 0x8000, 0xa000, 0xc000, 0xe000 };
  writeAndMirror(val, adr, memoryMirrorAddrs, 8);

  // TODO: There's a lot of other mirroring... we'll worry about it later
}

u_int8_t Memory::read(u_int16_t adr) {
	return memory[adr];
}

void Memory::push(u_int8_t value) {
  SP--;
  memory[SP + 0x0100] = value;
}

u_int8_t Memory::pop() {
  u_int8_t value = memory[SP + 0x0100];
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