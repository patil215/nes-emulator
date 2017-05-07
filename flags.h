#include <stdlib.h>

#ifndef _FLAGS_H_
#define _FLAGS_H_

class Flags {
	private:
		bool flags[8];
	public:
		bool getC();
		bool getZ();
		bool getI();
		bool getD();
		bool getB();
		bool getV();
		bool getS();
		u_int8_t getCB();
		u_int8_t getZB();
		u_int8_t getIB();
		u_int8_t getDB();
		u_int8_t getBB();
		u_int8_t getVB();
		u_int8_t getSB();
		void setC(bool result);
		void setZ(bool result);
		void setI(bool result);
		void setD(bool result);
		void setB(bool result);
		void setV(bool result);
		void setS(bool result);
		void setCB(u_int8_t result);
		void setZB(u_int8_t result);
		void setIB(u_int8_t result);
		void setDB(u_int8_t result);
		void setBB(u_int8_t result);
		void setVB(u_int8_t result);
		void setSB(u_int8_t result);
		void autoSetS(u_int8_t result);
		void autoSetZ(u_int8_t result);
};

#endif