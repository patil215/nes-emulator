#include <stdlib.h>
#include "flags.h"

#ifndef _MEMORY_H_
#define _MEMORY_H_

class Memory {

	private:
		u_int8_t memory[0xFFFFFFFFF];
	public:
		Memory();
		u_int8_t SP;
		void write(u_int8_t value, u_int16_t adr);
		u_int8_t read(u_int16_t adr);
		void push(u_int8_t value);
		u_int8_t pop();
		void pushFlags(Flags flags);
		void popFlags(Flags & flags);
		void setup(unsigned char * prg, int Prg_Size, int prgOffset);
		void writeAndMirror(u_int8_t val, u_int16_t adr, u_int16_t startAddrs[], int startAddrsSize);
};

#endif