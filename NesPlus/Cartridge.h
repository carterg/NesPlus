#pragma once
#include <string>
#include <vector>

class Cartridge {
private:
	bool loadRom(std::string romPath);
	bool verifyHeader(unsigned char header[16]);

	unsigned char *prgROM;
	unsigned char *chrROM;
	int prgROMSize;
	std::string romName;
public:
	Cartridge(std::string romName);
	~Cartridge();

	inline unsigned char* getProgramRom() {return prgROM;}
	inline int getProgramRomSize() { return prgROMSize; }
};