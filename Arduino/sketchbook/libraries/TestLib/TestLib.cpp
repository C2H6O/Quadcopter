#include <TestLib.h>
#include "Wire.h"

int a_priv, b_priv, c_priv;
int* ptrA;

TestLib::TestLib() {
	Serial.print("Hello");
}

TestLib::TestLib(int a, int b, int c) {

	a_priv = b;
	b_priv = c;
	c_priv = a;	

}


void TestLib::func1(int a, int b) {
	Serial.print("a: ");
	Serial.print(a,DEC);
	Serial.println();
	Serial.print("b: ");
	Serial.print(b,DEC);
	Serial.println();
}

void TestLib::func1(int a, int b, int c) {
	Serial.print("a: ");
	Serial.print(a,DEC);
	Serial.println();
	Serial.print("b: ");
	Serial.print(b,DEC);
	Serial.println();
	Serial.print("c: ");
	Serial.print(c,DEC);
	Serial.println();

}

void TestLib::ptrInitialize()
{
	//ptrA = (int*) malloc(3*sizeof(int));
}

int* TestLib::ptrFunc1(int* arrayPtr)
{
	//ptrA = (int*) malloc(3*sizeof(int));
	ptrA = arrayPtr;
	*(ptrA + 0) = 5;
	*(ptrA + 1) = 69;
	*(ptrA + 2) = 70;		// adxl345_angles is size 2
	return ptrA;

}

void TestLib::printPtr() {
	*(ptrA + 1) = *(ptrA + 1) + 2;
	Serial.print(*(ptrA + 0),DEC);
	Serial.println();
	Serial.print(*(ptrA + 1),DEC);
	Serial.println();
	//Serial.print(*(ptrA + 2),DEC);
	//Serial.println();
}

void TestLib::print_a() {

	Serial.print(a_priv,DEC);
}
