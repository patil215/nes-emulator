#include <stdlib.h>
#include "flags.h"

#ifndef _MEMORY_H_
#define _MEMORY_H_

class Memory {

	private:
		int8_t memory[0xFFFFFFFF];
	public:
		Memory();
		u_int16_t SP;
		void write(int8_t value, u_int16_t adr);
		int8_t read(u_int16_t adr);
		void push(int8_t value);
		int8_t pop();
		void pushFlags(Flags flags);
		void popFlags(Flags & flags);
		void setup(unsigned char * prg, int Prg_Size);
};

#endif