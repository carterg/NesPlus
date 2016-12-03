#pragma once
#include <string>
#include <sstream>
#include <iostream>

class DebugVerifierLineItem {
private:
	unsigned short pc;
	unsigned char opcode;

	unsigned char a;
	unsigned char x;
	unsigned char y;
	unsigned char p;
	unsigned char sp;
	unsigned short cyc;

public:
	DebugVerifierLineItem(unsigned short pc, unsigned char opcode, unsigned char a, unsigned char x, unsigned char y, unsigned char p, unsigned short sp, unsigned short cyc);
	DebugVerifierLineItem(std::string line);

	unsigned short getPC() { return pc; }
	unsigned char getOpcode() { return opcode; }
	unsigned char getX() { return x; }
	unsigned char getY() { return y; }
	unsigned char getA() { return a; }
	unsigned char getP() { return p; }
	unsigned char getSP() { return sp; }
	unsigned short getCyc() { return cyc;  }

};