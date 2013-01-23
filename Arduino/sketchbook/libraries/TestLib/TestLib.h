#ifndef TestLib_h
#define TestLib_h

#include "Arduino.h"

class TestLib {
public:
	TestLib();
	TestLib(int, int, int);
	void func1(int, int);
	void func1(int, int, int);
	int* ptrFunc1(int*);
	void printPtr();
	void print_a();
	void ptrInitialize();
};

#endif

