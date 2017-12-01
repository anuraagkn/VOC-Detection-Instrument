#ifndef Controller_h
#define Controller_h
#define OP_SIZE 32

#include "Arduino.h"
#include "FunctionGenerator.h"

class Controller
{
	public:
		Controller(bool w); // constructor
		void operate();
		void getNextOperator();
		void bootInstrument();
	private:
		char op[OP_SIZE];
		bool wait;
};

#endif
