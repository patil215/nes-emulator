#include <stdlib.h>
#include "flags.h"

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
};