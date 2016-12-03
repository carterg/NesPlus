#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "DebugVerifier.h"

class CPU {
private:
	bool debug;
	bool verifyDebug;
	bool printDebugTrace;
	unsigned char a;
	unsigned char x;
	unsigned char y;
	unsigned char p;
	unsigned char sp;
	unsigned short pc;
	unsigned short cycle;


	unsigned char* mem;


	/* state variables */
	bool pageBoundaryCrossed;

	/* Debug Variables */
	bool foundBadInstruction;
	bool verifyAgainstLog;
	std::string debugLine;
	DebugVerifier *debugVerifier;
	int instructionCount;
public:
	CPU();
	~CPU();
	void setProgramRom(unsigned char* prgRom, int size);
	void start();
	void executeCycle();

	void adc(unsigned char val, int length);
	void and(unsigned char val, int length);
	void asl(unsigned short address, int length);
	void asl_a();
	void bcc();
	void bcs();
	void bmi();
	void bpl();
	void bvc();
	void bvs();
	void beq();
	void bit(unsigned char relative, int length);
	void bne();
	void branch(unsigned short relative);
	void clc();
	void cld();
	void clv();
	void compare(unsigned char reg, unsigned char val, int length);
	void dcp(unsigned short addr, int length);
	void dec(unsigned short addr, int length);
	void dex();
	void dey();
	void eor(unsigned char val, int length);
	void inc(unsigned short addr, int length);
	void inx();
	void iny();
	void isc(unsigned short address, int length);
	void lax(unsigned char val, int length);
	void lda(unsigned char val, int length);
	void ldx(unsigned char val, int length);
	void ldy(unsigned char val, int length);
	void lsr_a();
	void lsr(unsigned short address, int size);
	void pha();
	void php();
	void pla();
	void plp();
	void nop(int length);
	void ora(unsigned char val, int length);
	void rla(unsigned short address, int length);
	void rol(unsigned short address, int length);
	void rol_a();
	void ror(unsigned short address, int length);
	void ror_a();
	void rra(unsigned short address, int length);
	void rti();
	void rts();
	void sax(unsigned short address, int length);
	void sec();
	void sed();
	void sei();
	void slo(unsigned short address, int length);
	void sre(unsigned short address, int length);
	void store(unsigned short address, unsigned char val);
	void sta(unsigned short address, int length);
	void stx(unsigned short val, int length);
	void sty(unsigned short val, int length);
	void tax();
	void tay();
	void tsx();
	void txa();
	void txs();
	void tya();
	void jsr(unsigned short address);
	void jump(short address);

	// stack modifier methods
	void pushOnStack(unsigned char val);
	void pushOnStack(unsigned short val);
	unsigned short pullOffStackShort();
	unsigned char pullOffStackChar();

	// status register mods
	void setSign(unsigned char val);
	void setZero(unsigned char val);
	void setOverflow(unsigned char val);
	void setOverflow(bool val);
	void setCarry(bool val);
	void setNegative(unsigned char val);

	bool isCarrySet();
	bool isNegativeSet();
	bool isOverflowSet();
	bool isZeroSet();


	unsigned short getAbsolute();
	unsigned short getAbsoluteX();
	unsigned short getAbsoluteY();
	unsigned char getImmediate();
	unsigned char getZeroPage();
	unsigned char getZeroPageX();
	unsigned char getZeroPageY();
	unsigned short getIndirect();
	unsigned short getIndirectX();
	unsigned short getIndirectY();

	/** Debug Methods **/
	void setDebugVerifier(DebugVerifier *dv) { this->debugVerifier = dv; }
	void dumpMemoryToFile();
	void printStack();


};