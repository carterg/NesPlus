#include "DebugVerifier.h"
#include <iostream>

DebugVerifier::DebugVerifier() : lines(NULL) {
	init("nestest.log");
}

DebugVerifier::DebugVerifier(std::string fileName) : lines(NULL) {
	init(fileName);
}
DebugVerifier::~DebugVerifier() {
	if (lines != NULL) {
		for (int i = 0; i < lines->size(); i++)
			delete lines->at(i);
		delete lines;
	}
}

void DebugVerifier::init(std::string fileName) {
	lines = new std::vector<DebugVerifierLineItem*>();

	fileStream = new std::ifstream(fileName);
	if (!fileStream->is_open()) {
		std::cerr << "Could not locate file: " << fileName << "." << std::endl;
	}

	std::cout << "Beginning to read the debug verifier file." << std::endl;
	std::string line;
	while (std::getline(*fileStream, line)) {
		lines->push_back(new DebugVerifierLineItem(line));
	}

	std::cout << "Read " << lines->size() << " lines from debug verifier file." << std::endl ;
	fileStream->close();
	delete fileStream;
}

bool DebugVerifier::checkInstruction(std::string output, int instructionCount) {

	std::string substringToCheck = output.substr(0, 73);
	if (substringToCheck.compare(output) != 0) {
		std::cout << "Emulator output   : " << output << std::endl;
		std::cout << "Golden Log output : " << substringToCheck << std::endl;
		std::cout << "!!! NOT A MATCH !!!" << std::endl;
		std::cout << "Instruction Count :" << instructionCount + 1 << std::endl;
	}

	return false;
}

bool DebugVerifier::verifyLine(unsigned short pc, unsigned char opcode, int line, unsigned char a, unsigned char x, unsigned char y, unsigned char p, unsigned char sp, unsigned short cyc) {
	DebugVerifierLineItem *lineItem = lines->at(line);
	if (pc != lineItem->getPC()) {
		std::cout << "-- FAIL --  PC Does not match.  Found: " << pc << " Expected: " << lineItem->getPC() << " on line: " << std::dec << line << std::endl;
		return false;
	}
	else if (opcode != lineItem->getOpcode()) {
		std::cout << "-- FAIL --  OPCODE Does not match.  Found: " << std::hex << (int)opcode << " Expected: " << std::hex << (int)lineItem->getOpcode() << std::dec << " on line: " << line << std::endl;
		return false;
	}
	else if (a != lineItem->getA()) {
		std::cout << "-- FAIL -- A Does not match.  Found: " << std::hex << (int)a << " Expected: " << std::hex << (int)lineItem->getA() << " on line: " << std::dec << line << std::endl;
		return false;
	}
	else if (x != lineItem->getX()) {
		std::cout << "-- FAIL -- X Does not match.  Found: " << std::hex << (int)x << " Expected: " << std::hex << (int)lineItem->getX() << " on line: " << std::dec << line << std::endl;
		return false;
	}
	else if (y != lineItem->getY()) {
		std::cout << "-- FAIL -- Y Does not match.  Found: " << std::hex << (int)y << " Expected: " << std::hex << (int)lineItem->getY() << " on line: " << std::dec << line << std::endl;
		return false;
	}
	else if (p != lineItem->getP()) {
		std::cout << "-- FAIL -- P Does not match.  Found: " << std::hex << (int)p << " Expected: " << std::hex << (int)lineItem->getP() << " on line: " << std::dec << line << std::endl;
		return false;
	}
	else if (sp != lineItem->getSP()) {
		std::cout << "-- FAIL -- SP Does not match.  Found: " << std::hex << (int)sp << " Expected: " << std::hex << (int)lineItem->getSP() << " on line: " << std::dec << line << std::endl;
		return false;
	}
	else if (cyc != lineItem->getCyc()) {
		std::cout << "-- FAIL -- CYC Does not match.  Found: " << cyc << " Expected: " << lineItem->getCyc() << " on line: " << std::dec << line << std::endl;
		return false;
	}

	return true;
}
