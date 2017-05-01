#include <stdlib.h>

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
		void setCB(int result);
		void setZB(int result);
		void setIB(int result);
		void setDB(int result);
		void setBB(int result);
		void setVB(int result);
		void setSB(int result);
		void autoSetS(int8_t result);
		void autoSetZ(int8_t result);
};