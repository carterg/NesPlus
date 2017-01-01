#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "DebugVerifierLineItem.h"

//std::string defaultGoldenLog = "nesttest.log";

class DebugVerifier {
private:
	std::vector<DebugVerifierLineItem*>* lines;
	std::ifstream* fileStream;

public:
	DebugVerifier();
	DebugVerifier(std::string testFile);
	~DebugVerifier();
	void init(std::string file);
	bool checkInstruction(std::string output, int instructionCount);
	bool verifyLine(unsigned short pc, unsigned char opcode, int line, unsigned char a, unsigned char x, unsigned char y, unsigned char p, unsigned char sp,  unsigned short cyc);
	bool isAtEnd(int line);
};