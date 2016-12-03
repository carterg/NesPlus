#include "CPU.h"

CPU::CPU(): debug(true), verifyDebug(true), printDebugTrace(true), a(0), x(0), y(0), p(0), sp(0), pc(0), instructionCount(0){
	foundBadInstruction = false;
	verifyAgainstLog = true;
	pc = 0xC000;
	sp = 0xFD;
	p = 0x24;
	cycle = 0;
	pageBoundaryCrossed = false;
	mem = new unsigned char[0xFFFF + 1];
	for (int i = 0; i < 0xFFFF; i++) {
		mem[i] = 0;
	}
}

CPU::~CPU() {
	delete[] mem;
}

/*
	FIXME: does not yet duplicate the progrom if it is only 0x4000 in size
*/
void CPU::setProgramRom(unsigned char* programRom, int size) {
	memcpy(mem + 0xc000,programRom, size);
}
void CPU::start() {
	while (!foundBadInstruction) {
		executeCycle();
	}
}

void CPU::executeCycle() {
	pageBoundaryCrossed = false;
	unsigned char op = mem[pc];
	if (verifyAgainstLog) {
		if (!debugVerifier->verifyLine(pc, op, instructionCount, a, x, y, p, sp, (( cycle * 3) % 341))) {
			foundBadInstruction = true;
			dumpMemoryToFile();
			return;
		}
		instructionCount++;
	}

	switch (op) {
	case 0x01:
		ora(mem[getIndirectX()], 2); 
		cycle += 6;	
		break;
	case 0x03:
		slo(getIndirectX(), 2); 
		cycle += 8;
		break;
	case 0x04:
		nop(2); 
		cycle += 3;
		break;
	case 0x05:
		ora(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0x06:
		asl(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0x07:
		slo(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0x08:
		php(); 
		cycle += 3;
		break;
	case 0x09:
		ora(getImmediate(), 2); 
		cycle += 2;
		break;
	case 0x0a:
		asl_a(); 
		cycle += 2;
		break;
	case 0x0c:
		nop(3);
		cycle += 4;
		break;
	case 0x0d:
		ora(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0x0e:
		asl(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x0f:
		slo(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x10:
		bpl(); 
		cycle += 2;
		break;
	case 0x11:
		ora(mem[getIndirectY()], 2); 
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0x13:
		slo(getIndirectY(), 2); 
		cycle += 8;
		break;
	case 0x14:
		nop(2); 
		cycle += 4;
		break;
	case 0x15:
		ora(mem[getZeroPageX()], 2); 
		cycle += 4; 
		break;
	case 0x16:
		asl(getZeroPageX(), 2);
		cycle += 6;
		break;
	case 0x17:
		slo(getZeroPageX(), 2); 
		cycle += 6;
		break;
	case 0x18:
		clc(); 
		cycle += 2;
		break;
	case 0x19:
		ora(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x1a:
		nop(1); 
		cycle += 2;
		break;
	case 0x1b:
		slo(getAbsoluteY(), 3);
		cycle += 7;
		break;
	case 0x1c:
		getAbsoluteX();
		nop(3);
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x1d:
		ora(mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x1e:
		asl(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0x1f:
		slo(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0x20:
		jsr(getAbsolute()); 
		cycle += 6;
		break;
	case 0x21:
		and (mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0x23:
		rla(getIndirectX(), 2);
		cycle += 8;
		break;
	case 0x24:
		bit(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0x25:
		and (mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0x26:
		rol(getZeroPage(), 2);
		cycle += 5;
		break;
	case 0x27:
		rla(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0x28:
		plp(); 
		cycle += 4;
		break;
	case 0x29:
		and (getImmediate(), 2); 
		cycle += 2;
		break;
	case 0x2a:
		rol_a(); 
		cycle += 2;
		break;
	case 0x2c:
		bit(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0x2d:
		and (mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0x2e:
		rol(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x2f:
		rla(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x30:
		bmi(); 
		cycle += 2;
		break;
	case 0x31:
		and(mem[getIndirectY()], 2); 
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0x33:
		rla(getIndirectY(), 2); 
		cycle += 8;
		break;
	case 0x34:
		nop(2); 
		cycle += 4;
		break;
	case 0x35:
		and(mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0x36:
		rol(getZeroPageX(), 2); 
		cycle += 6;
		break;
	case 0x37:
		rla(getZeroPageX(), 2); 
		cycle += 6;
		break;
	case 0x38:
		sec(); 
		cycle += 2;
		break;
	case 0x39:
		and(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x3a:
		nop(1); 
		cycle += 2;
		break;
	case 0x3b:
		rla(getAbsoluteY(), 3); 
		cycle += 7;
		break;
	case 0x3c:
		nop(3); 
		getAbsoluteX();
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x3d:
		and(mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x3e:
		rol(getAbsoluteX(), 3);
		cycle += 7;
		break;
	case 0x3f:
		rla(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0x40:
		rti();
		cycle += 6;
		break;
	case 0x41:
		eor(mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0x43:
		sre(getIndirectX(), 2); 
		cycle += 8;
		break;
	case 0x44:
		nop(2); 
		cycle += 3;
		break;
	case 0x45:
		eor(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0x46:
		lsr(getZeroPage(), 2);
		cycle += 5;
		break;
	case 0x47:
		sre(getZeroPage(), 2);
		cycle += 5;
		break;
	case 0x48:
		pha(); 
		cycle += 3;
		break;
	case 0x4a:
		lsr_a(); 
		cycle += 2;
		break;
	case 0x4c:
		jump(getAbsolute()); 
		cycle += 3;
		break;
	case 0x4d:
		eor(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0x4e:
		lsr(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x4f:
		sre(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x49:
		eor(getImmediate(), 2); 
		cycle += 2;
		break;
	case 0x50:
		bvc(); 
		cycle += 2;
		break;
	case 0x51:
		eor(mem[getIndirectY()], 2);
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0x53:
		sre(getIndirectY(), 2); 
		cycle += 8;
		break;
	case 0x54:
		nop(2); 
		cycle += 4;
		break;
	case 0x55:
		eor(mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0x56:
		lsr(getZeroPageX(), 2);
		cycle += 6;
		break;
	case 0x57:
		sre(getZeroPageX(), 2);
		cycle += 6;
		break;
	case 0x59:
		eor(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x5a:
		nop(1); 
		cycle += 2;
		break;
	case 0x5b:
		sre(getAbsoluteY(), 3); 
		cycle += 7;
		break;
	case 0x5c:
		nop(3); 
		getAbsoluteX();
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x5d:
		eor(mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x5e:
		lsr(getAbsoluteX(), 3);
		cycle += 7;
		break;
	case 0x5f:
		sre(getAbsoluteX(), 3);
		cycle += 7;
		break;
	case 0x60:
		rts(); 
		cycle += 6;
		break;
	case 0x61:
		adc(mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0x63:
		rra(getIndirectX(), 2); 
		cycle += 8;
		break;
	case 0x64:
		nop(2); 
		cycle += 3;
		break;
	case 0x65:
		adc(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0x66:
		ror(getZeroPage(), 2);
		cycle += 5;
		break;
	case 0x67:
		rra(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0x68:
		pla(); 
		cycle += 4;
		break;
	case 0x69:
		adc(getImmediate(), 2); 
		cycle += 2;
		break;
	case 0x6a:
		ror_a(); 
		cycle += 2;
		break;
	case 0x6c:
		jump(getIndirect()); 
		cycle += 5;
		break;
	case 0x6d:
		adc(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0x6e:
		ror(getAbsolute(), 3);
		cycle += 6;
		break;
	case 0x6f:
		rra(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0x70:
		bvs(); 
		cycle += 2;
		break;
	case 0x71:
		adc(mem[getIndirectY()], 2); 
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0x73:
		rra(getIndirectY(), 2); 
		cycle += 8;
		break;
	case 0x74:
		nop(2); 
		cycle += 4;
		break;
	case 0x75:
		adc(mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0x76:
		ror(getZeroPageX(), 2);
		cycle += 6;
		break;
	case 0x77:
		rra(getZeroPageX(), 2);
		cycle += 6;
		break;
	case 0x78:
		sei(); 
		cycle += 2;
		break;
	case 0x79:
		adc(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x7a:
		nop(1); 
		cycle += 2;
		break;
	case 0x7b:
		rra(getAbsoluteY(), 3);
		cycle += 7;
		break;
	case 0x7c:
		nop(3); 
		getAbsoluteX();
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x7d:
		adc(mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0x7e:
		ror(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0x7f:
		rra(getAbsoluteX(), 3);
		cycle += 7;
		break;
	case 0x80:
		nop(2); 
		cycle += 2;
		break;
	case 0x81:
		sta(getIndirectX(), 2); 
		cycle += 6;
		break;
	case 0x82:
		nop(2); 
		cycle += 2;
		break;
	case 0x83:
		sax(getIndirectX(), 2); 
		cycle += 6;
		break;
	case 0x84:
		sty(getZeroPage(), 2); 
		cycle += 3;
		break;
	case 0x85:
		sta(getImmediate(), 2); 
		cycle += 3;
		break;
	case 0x86:
		stx(getZeroPage(), 2); 
		cycle += 3;
		break;
	case 0x87:
		sax(getZeroPage(), 2);
		cycle += 3;
		break;
	case 0x88:
		dey(); 
		cycle += 2;
		break;
	case 0x89:
		nop(2);
		cycle += 9;
		break;
	case 0x8a:
		txa(); 
		cycle += 2;
		break;
	case 0x8c:
		sty(getAbsolute(), 3); 
		cycle += 4;
		break;
	case 0x8d:
		sta(getAbsolute(), 3); 
		cycle += 4;
		break;
	case 0x8e:
		stx(getAbsolute(), 3); 
		cycle += 4;
		break;
	case 0x8f:
		sax(getAbsolute(), 3);
		cycle += 4;
		break;
	case 0x90:
		bcc(); 
		cycle += 2;
		break;
	case 0x91:
		sta(getIndirectY(), 2); 
		cycle += 6;
		break;
	case 0x94:
		sty(getZeroPageX(), 2); 
		cycle += 4;
		break;
	case 0x95:
		sta(getZeroPageX(), 2); 
		cycle += 4;
		break;
	case 0x96:
		stx(getZeroPageY(), 2); 
		cycle += 4;
		break;
	case 0x97:
		sax(getZeroPageY(), 2); 
		cycle += 4;
		break;
	case 0x98:
		tya();
		cycle += 2;
		break;
	case 0x99:
		sta(getAbsoluteY(), 3); 
		cycle += 5;
		break;
	case 0x9a:
		txs(); 
		cycle += 2;
		break;
	case 0x9d:
		sta(getAbsoluteX(), 3); 
		cycle += 5;
		break;
	case 0xa0:
		ldy(getImmediate(), 2); 
		cycle += 2;
		break;
	case 0xa1:
		lda(mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0xa2:
		ldx(getImmediate(), 2); 
		cycle += 2;
		break;
	case 0xa3:
		lax(mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0xa4:
		ldy(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xa5:
		lda(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xa6:
		ldx(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xa7:
		lax(mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xa8:
		tay(); 
		cycle += 2;
		break;
	case 0xa9:
		lda(getImmediate(), 2); 
		cycle += 2;
		break;
	case 0xaa:
		tax(); 
		cycle += 2;
		break;
	case 0xac:
		ldy(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xad:
		lda(mem[getAbsolute()], 3);
		cycle += 4;
		break;
	case 0xae:
		ldx(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xaf:
		lax(mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xb0:
		bcs(); 
		cycle += 2;
		break;
	case 0xb1:
		lda(mem[getIndirectY()], 2);
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0xb3:
		lax(mem[getIndirectY()], 2); 
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0xba:
		tsx(); 
		cycle += 2;
		break;
	case 0xbc:
		ldy(mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xb4:
		ldy(mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0xb5:
		lda(mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0xb6:
		ldx(mem[getZeroPageY()], 2);
		cycle += 4;
		break;
	case 0xb7:
		lax(mem[getZeroPageY()], 2); 
		cycle += 4;
		break;
	case 0xb8:
		clv(); 
		cycle += 2;
		break;
	case 0xb9:
		lda(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xbd:
		lda(mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xbe:
		ldx(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xbf:
		lax(mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xc1:
		compare(a, mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0xca:
		dex(); 
		cycle += 2;
		break;
	case 0xc0:
		compare(y, getImmediate(), 2); 
		cycle += 2;
		break;
	case 0xc2:
		nop(2); 
		cycle += 2;
		break;
	case 0xc3:
		dcp(getIndirectX(), 2); 
		cycle += 8;
		break;
	case 0xc4:
		compare(y, mem[getZeroPage()], 2);
		cycle += 3;
		break;
	case 0xc5:
		compare(a, mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xc6:
		dec(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0xc7:
		dcp(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0xc8:
		iny(); 
		cycle += 2;
		break;
	case 0xc9:
		compare(a, getImmediate(), 2); 
		cycle += 2;
		break;
	case 0xcc:
		compare(y, mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xcd:
		compare(a, mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xce:
		dec(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0xcf:
		dcp(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0xd0:
		bne(); 
		cycle += 2;
		break;
	case 0xd1:
		compare(a, mem[getIndirectY()], 2); 
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0xd3:
		dcp(getIndirectY(), 2); 
		cycle += 8;
		break;
	case 0xd4:
		nop(2); 
		cycle += 4;
		break;
	case 0xd5:
		compare(a, mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0xd6:
		dec(getZeroPageX(), 2); 
		cycle += 6;
		break;
	case 0xd7:
		dcp(getZeroPageX(), 2); 
		cycle += 6;
		break;
	case 0xd8:
		cld(); 
		cycle += 2;
		break;
	case 0xd9:
		compare(a, mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xda:
		nop(1);
		cycle += 2;
		break;
	case 0xdb:
		dcp(getAbsoluteY(), 3); 
		cycle += 7;
		break;
	case 0xdc:
		nop(3); 
		getAbsoluteX();
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xdd:
		compare(a, mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xde:
		dec(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0xdf:
		dcp(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0xe0:
		compare(x, getImmediate(), 2); 
		cycle += 2;
		break;
	case 0xe1:
		adc(~mem[getIndirectX()], 2); 
		cycle += 6;
		break;
	case 0xe2:
		nop(2); 
		cycle += 2;
		break;
	case 0xe3:
		isc(getIndirectX(), 2);
		cycle += 8;
		break;
	case 0xe4:
		compare(x, mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xe5:
		adc(~mem[getZeroPage()], 2); 
		cycle += 3;
		break;
	case 0xe6:
		inc(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0xe7:
		isc(getZeroPage(), 2); 
		cycle += 5;
		break;
	case 0xe8:
		inx();
		cycle += 2;
		break;
	case 0xe9:
		adc(~(getImmediate()), 2); 
		cycle += 2;
		break;
	case 0xea:
		nop(1); 
		cycle += 2;
		break;
	case 0xeb:
		adc(~(getImmediate()), 2);
		cycle += 2;
		break;
	case 0xec:
		compare(x, mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xed:
		adc(~mem[getAbsolute()], 3); 
		cycle += 4;
		break;
	case 0xee:
		inc(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0xef:
		isc(getAbsolute(), 3); 
		cycle += 6;
		break;
	case 0xf0:
		beq(); 
		cycle += 2;
		break;
	case 0xf1:
		adc(~mem[getIndirectY()], 2); 
		cycle += (pageBoundaryCrossed ? 6 : 5);
		break;
	case 0xf3:
		isc(getIndirectY(), 2);
		cycle += 8;
		break;
	case 0xf4:
		nop(2); 
		cycle += 4;
		break;
	case 0xf5:
		adc(~mem[getZeroPageX()], 2); 
		cycle += 4;
		break;
	case 0xf6:
		inc(getZeroPageX(), 2);
		cycle += 6;
		break;
	case 0xf7:
		isc(getZeroPageX(), 2); 
		cycle += 6;
		break;
	case 0xf8:
		sed(); 
		cycle += 2;
		break;
	case 0xf9:
		adc(~mem[getAbsoluteY()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xfa:
		nop(1); 
		cycle += 2;
		break;
	case 0xfb:
		isc(getAbsoluteY(), 3); 
		cycle += 7;
		break;
	case 0xfc:
		nop(3); 
		getAbsoluteX();
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xfd:
		adc(~mem[getAbsoluteX()], 3); 
		cycle += (pageBoundaryCrossed ? 5 : 4);
		break;
	case 0xfe:
		inc(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	case 0xff:
		isc(getAbsoluteX(), 3); 
		cycle += 7;
		break;
	default:
		std::cerr << "No opcode implemented for: " << std::hex << (int)op << std::dec << std::endl;
		foundBadInstruction = true;
		break;
	}
}

unsigned short CPU::getAbsolute() {
	unsigned char low = mem[pc + 1];
	unsigned char high = mem[pc + 2];

	unsigned short val = (unsigned short)((high << 8) | low);
	return (unsigned short)((high << 8) | low);
}

unsigned short CPU::getAbsoluteX() {
	unsigned short addr = getAbsolute() + x;
	if ((getAbsolute() & 0xFF00) != ((getAbsolute() + x) & 0xFF00))
		pageBoundaryCrossed = true;
	return addr;
}

unsigned short CPU::getAbsoluteY() {
	unsigned short addr = getAbsolute() + y;
	if ((getAbsolute() & 0xFF00) != ((getAbsolute() + y) & 0xFF00))
		pageBoundaryCrossed = true;
	return addr;
}

unsigned char CPU::getImmediate() {
	return mem[pc + 1];
}

unsigned char CPU::getZeroPage() {
	unsigned char addr = mem[pc + 1];
	return addr;
}

unsigned char CPU::getZeroPageX() {
	unsigned char addr = (mem[pc + 1] + x) % 256;
	return addr;
}

unsigned char CPU::getZeroPageY() {
	unsigned char addr = (mem[pc + 1] + y) % 256;
	return addr;
}

unsigned short CPU::getIndirect() {
	bool bug_hit = false;
	unsigned char low = mem[pc + 1];
	unsigned char high = mem[pc + 2];

	if (low == 0xFF) bug_hit = true; 

	unsigned short addr = (((high << 8) & 0xFF00) + low);

	unsigned short ret_low = mem[addr];
	unsigned short ret_high;
	if (bug_hit) {
		ret_high = mem[addr & 0xFF00];
	}
	else {
		ret_high = mem[addr + 1];
	}

	unsigned short ret = (((ret_high << 8) & 0xFF00) + ret_low);
	return ret;
}

unsigned short CPU::getIndirectX() {
	unsigned char addr = mem[pc + 1];
	unsigned short lookup = ((addr + x) % 0x100);
	unsigned char low = mem[lookup];
	lookup = ((addr + x + 1) % 0x100);
	unsigned char high = mem[lookup];
	unsigned short comp = (((high << 8) & 0xFF00) + low);
	return comp;
}

unsigned short CPU::getIndirectY() {
	unsigned char addr = mem[pc + 1];
	unsigned char low = mem[addr % 0x100];
	unsigned char high = mem[(addr + 1) % 0x100];
	unsigned short before_y = (((high << 8) & 0xFF00) + low);
	unsigned short ret = before_y + y;
	if ((before_y & 0xFF00) != ((ret) & 0xFF00))
		pageBoundaryCrossed = true;
	return ret;
}

void CPU::jump(short address) {
	pc = address;
}

void CPU::adc(unsigned char val, int length) {
	unsigned char t = a + val + (isCarrySet() ? 1 : 0);

	if (((a ^ t) & (t ^ val) & 0x80) == 0x80) {
		setOverflow(true);
	}
	else {
		setOverflow(false);
	}
	setCarry((a + val + (isCarrySet() ? 1 : 0) > 255) ? 1 : 0);
	a = t & 0xFF;
	setNegative(a);
	setZero(t);
	pc += length;
}

void CPU::and(unsigned char val, int length) {
	a = a & val;
	setZero(a);
	setSign(a);
	pc += length;
}

void CPU::asl(unsigned short address, int length) {
	unsigned char memVal = mem[address];
	setCarry(((memVal & 0x80) > 0) ? true : false);
	memVal = memVal << 1;
	mem[address] = memVal;
	setNegative(memVal);
	setZero(memVal);
	pc += length;
}

void CPU::asl_a() {
	setCarry(((a & 0x80) > 0) ? true : false);
	a = a << 1;
	setNegative(a);
	setZero(a);
	pc += 1;
}

void CPU::bcc() {
	if (!isCarrySet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == ((pc + 2) & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::bcs() {
	if (isCarrySet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == (pc + 2 & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::beq() {
	if (isZeroSet()) {
		char addr = getImmediate();

		if (((pc + addr + 2) & 0xFF00) == (pc + 2 & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::bmi() {
	if (isNegativeSet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == (pc + 2 & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}
void CPU::bpl() {
	if (!isNegativeSet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == (pc + 2 & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::bvc() {
	if (!isOverflowSet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == (pc + 2 & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::bvs() {
	if (isOverflowSet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == (pc + 2 & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::bit(unsigned char val, int length) {
	unsigned char t = val & a;
	setZero(t);
	setSign(val);
	setOverflow(val);

	pc += length;
}

void CPU::bne() {
	if (!isZeroSet()) {
		char addr = getImmediate();
		if (((pc + addr + 2) & 0xFF00) == (pc & 0xFF00))
			cycle += 1;
		else
			cycle += 2;
		branch(addr);
	}
	else {
		pc += 2;
	}
}

void CPU::branch(unsigned short relative) {
	pc = pc + relative + 2;
}

void CPU::clc() {
	p = p & 0b11111110;
	pc += 1;
}

void CPU::cld() {
	p = p & 0b11110111;
	pc += 1;
}

void CPU::clv() {
	p = p & 0b10111111;
	pc += 1;
}

void CPU::compare(unsigned char reg, unsigned char val, int length) {
	unsigned char t = reg - val;
	setCarry(reg >= val);
	setZero(t);
	setSign(t);

	pc += length;
}

void CPU::dcp(unsigned short address, int length) {
	unsigned char val = mem[address];
	val--;
	mem[address] = val;

	unsigned char t = a - val;
	setZero(t);
	setSign(t);

	pc += length;
}

void CPU::dec(unsigned short address, int length) {
	unsigned char val = mem[address];
	val = val - 1;
	setSign(val);
	setZero(val);
	mem[address] = val;
	pc += length;
}
void CPU::dex() {
	x -= 1;
	setZero(x);
	setNegative(x);
	pc += 1;
}

void CPU::dey() {
	y -= 1;
	setZero(y);
	setNegative(y);
	pc += 1;
}

void CPU::eor(unsigned char val, int length) {
	a = a ^ val;
	setNegative(a);
	setZero(a);
	pc += length;
}

void CPU::inc(unsigned short address, int length) {
	unsigned char val = mem[address];
	val++;
	setSign(val);
	setZero(val);
	mem[address] = val;
	pc += length;
}

void CPU::iny() {
	y += 1;
	setZero(y);
	setNegative(y);
	pc += 1;
}

void CPU::inx() {
	x += 1;
	setZero(x);
	setNegative(x);
	pc += 1;
}

void CPU::isc(unsigned short address, int length) {
	unsigned char val = mem[address];
	val++;
	mem[address] = val;
	adc(~val, 0);
	pc += length;
}

void CPU::jsr(unsigned short address) {
	pushOnStack((unsigned short)(pc + 2));
	pc = address;

}

void CPU::lax(unsigned char val, int length) {
	a = val;
	x = val;
	setSign(val);
	setZero(val);
	pc += length;
}

void CPU::lda(unsigned char val, int length) {
	a = val;
	pc += length;
	setSign(val);
	setZero(val);
}

void CPU::ldx(unsigned char val, int length) {
	x = val;
	pc += length;
	setSign(val);
	setZero(val);
}

void CPU::ldy(unsigned char val, int length) {
	y = val;
	pc += length;
	setSign(val);
	setZero(val);
}

void CPU::lsr_a() {
	setCarry(((a & 0x01) == 1) ? true : false);
	a = a >> 1;
	setNegative(0);
	setZero(a);
	pc += 1;
}

void CPU::lsr(unsigned short memoryAddress, int size) {
	unsigned char val = mem[memoryAddress];
	setCarry(((val & 0x01) == 1) ? true : false);
	val = val >> 1;
	mem[memoryAddress] = val;
	setNegative(val);
	setZero(val);
	pc += size;
}

void CPU::nop(int length) {
	pc += length;
}

void CPU::ora(unsigned char val, int length) {
	a = a | val;
	setNegative(a);
	setZero(a);
	pc += length;
}

void CPU::pha() {
	pushOnStack(a);
	pc += 1;
}
void CPU::php() {
	unsigned char val = p | 0b00010000;
	pushOnStack(val);
	pc += 1;
}

void CPU::pla() {
	a = pullOffStackChar();
	setSign(a);
	setZero(a);
	pc += 1;
}

void CPU::plp() {
	p = (unsigned char)(pullOffStackChar() & 0b11101111);
	p = p | 0b00100000; // bit 5 alway set to 1
	pc += 1;
}

void CPU::rla(unsigned short address, int length) {
	rol(address, 0);
	and (mem[address], 0);
	pc += length;
}

void CPU::rol(unsigned short address, int length) {
	unsigned char val = mem[address];
	bool bit7Set = ((val & 0x80) == 0x80) ? true : false;
	val = val << 1;
	if (isCarrySet()) {
		val = val | 0x01;
	}
	setCarry(bit7Set);
	setZero(val);
	setSign(val);
	mem[address] = val;
	pc += length;
}

void CPU::rol_a() {
	bool bit7Set = ((a & 0x80) == 0x80) ? true : false;
	a = a << 1;
	if (isCarrySet()) {
		a = a | 0x01;
	}
	setCarry(bit7Set);
	setZero(a);
	setSign(a);
	pc += 1;
}

void CPU::ror(unsigned short address, int length) {
	unsigned char val = mem[address];
	bool bit0Set = ((val & 0x01) == 1) ? true : false;
	val = val >> 1;
	if (isCarrySet()) {
		val = val | 0x80;
	}
	mem[address] = val;
	setCarry(bit0Set);
	setZero(val);
	setSign(val);
	pc += length;
}

void CPU::ror_a() {
	bool bit0Set = ((a & 0x01) == 1) ? true : false;
	a = a >> 1;
	if (isCarrySet()) {
		a = a | 0x80;
	}
	setCarry(bit0Set);
	setZero(a);
	setSign(a);
	pc += 1;
}

void CPU::rra(unsigned short address, int length) {
	ror(address, 0);
	adc(mem[address], 0);
	pc += length;
}

void CPU::rti() {
	p = pullOffStackChar();
	p = p | 0b00100000;  // bit 5 always set to 1
	pc = pullOffStackShort();
}

void CPU::rts() {
	pc = pullOffStackShort() + 1;
}

void CPU::sax(unsigned short address, int length) {
	unsigned char val = a & x;
	mem[address] = val;
	pc += length;
}

void CPU::sec() {
	p = p | 0b00000001;
	pc += 1;
}

void CPU::sed() {
	p = p | 0b00001000;
	pc += 1;
}
void CPU::sei() {
	p = p | 0b00000100;
	pc += 1;
}

void CPU::slo(unsigned short address, int length) {
	unsigned char val = mem[address];
	setCarry(((val & 0x80) > 0) ? true : false);
	mem[address] = val << 1;

	a = a | mem[address];
	setSign(a);
	setZero(a);
	pc += length;
}

void CPU::sre(unsigned short address, int length) {
	lsr(address, 0);
	eor(mem[address], 0);
	pc += length;
}

void CPU::store(unsigned short address, unsigned char val) {
	mem[address] = val;
}

void CPU::sta(unsigned short address, int length) {
	store(address, a);
	pc += length;
}

void CPU::stx(unsigned short val, int length) {
	store(val, x);
	pc += length;
}

void CPU::sty(unsigned short val, int length) {
	store(val, y);
	pc += length;
}

void CPU::tax() {
	x = a;
	setNegative(x);
	setZero(x);
	pc += 1;
}

void CPU::tay() {
	y = a;
	setNegative(y);
	setZero(y);
	pc += 1;
}

void CPU::tsx() {
	x = sp;
	setNegative(x);
	setZero(x);
	pc += 1;
}

void CPU::txa() {
	a = x;
	setNegative(a);
	setZero(a);
	pc += 1;
}

void CPU::txs() {
	sp = x;
	pc += 1;
}

void CPU::tya() {
	a = y;
	setNegative(a);
	setZero(a);
	pc += 1;
}

unsigned short CPU::pullOffStackShort() {
	unsigned char val1 = mem[0x100 + sp + 1];
	unsigned char val2 = mem[0x100 + sp + 2];
	unsigned short val = (val2 << 8) + val1;

	sp += 2;

	return val;
}

unsigned char CPU::pullOffStackChar() {
	unsigned char val = mem[0x100 + sp + 1];
	sp += 1;
	return val;
}

void CPU::pushOnStack(unsigned char val) {
	mem[0x100 + sp] = val;
	sp -= 1;
}
void CPU::pushOnStack(unsigned short val) {
	short addr = 0x100 + sp;
	mem[addr] = (unsigned char)(val >> 8);
	mem[addr - 1] = (unsigned char)(val & 0xFF);
	sp -= 2;
}

void CPU::setCarry(bool val) {
	if (val) {
		p = p | 0b00000001;
	}
	else {
		p = p & 0b11111110;
	}
}

void CPU::setNegative(unsigned char val) {
	if (val & 0b10000000) {
		p = p | 0b10000000;
	}
	else {
		p = p & 0b01111111;
	}
}

void CPU::setOverflow(bool val) {
	if (val) {
		p = p | 0b01000000;
	}
	else {
		p = p & 0b10111111;
	}
}

void CPU::setOverflow(unsigned char val) {
	if (val & 0b01000000) {
		p = p | 0b01000000;
	}
	else {
		p = p & 0b10111111;
	}
}
void CPU::setSign(unsigned char val) {
	if (val >> 7 == 1) {
		p = p | 0b10000000;
	}
	else {
		p = p & 0b01111111;
	}
}

void CPU::setZero(unsigned char val) {
	if (val == 0) {
		p = p | 0b00000010;
	}
	else {
		p = p & 0b11111101;
	}
}

bool CPU::isCarrySet() {
	return (p & 0b00000001) == 1;
}

bool CPU::isNegativeSet() {
	return (p & 0b10000000) > 0;
}

bool CPU::isOverflowSet() {
	return (p & 0b01000000) > 0;
}

bool CPU::isZeroSet() {
	return (p & 0b00000010) == 2;
}


void CPU::dumpMemoryToFile() {
	std::cout << "Dumping contents of CPU memory to file..." << std::endl;
	std::ofstream dump;
	dump.open("dump.txt");
	for (int i = 0; i <= 0xFFFF; i++) {
		if (i % 8 == 0) {
			dump << std::endl << std::hex << i << "   ";
		}
		dump << std::hex << (int)mem[i] << " ";
	}
	dump.close();
	std::cout << "Done dumping memory contents of CPU" << std::endl;
}

void CPU::printStack() {
	std::ofstream stackDump;
	stackDump.open("stackDump.txt");

	for (int i = 0; i < 0xFF; i++) {
		unsigned short addr = 0x100 + i;
		stackDump << std::hex << addr << " " << (int) mem[addr] << std::endl << std::dec << std::flush;
	}

	stackDump.close();
}