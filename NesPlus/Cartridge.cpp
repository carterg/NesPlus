#include "Cartridge.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

Cartridge::Cartridge(std::string romName) {
	std::cout << "Loading rom " << romName << std::endl;
	loadRom(romName);
}

Cartridge::~Cartridge() {
	delete[] prgROM;
	delete[] chrROM;
}

bool Cartridge::loadRom(std::string romPath) {
	std::ifstream fileStream = std::ifstream(romPath);
	FILE *file = NULL;

	if((file = fopen(romPath.c_str(), "rb")) == NULL) {
		std::cerr << "Could not open file: " << romPath << std::endl;
		return false;
	}
	
	unsigned char header[16];
	fread(header, 16, 1, file);
	if (!verifyHeader(header)) {
		std::cerr << "Header invalid" << std::endl;
	}
	else {
		std::cout << "Header was valid." << std::endl;
	}
	
	int sizeOfPRGROM = header[4];
	prgROMSize = sizeOfPRGROM * 16 * 1024;
	prgROM = new unsigned char[prgROMSize];

	int sizeOfCHRROM = header[5];
	chrROM = new unsigned char[sizeOfCHRROM * 8 * 1024];
	
	size_t read = fread(prgROM, 1, sizeOfPRGROM * 16 * 1024, file);
	
	return true;
}

bool Cartridge::verifyHeader(unsigned char header[16]) {
	return header[0] == 0x4e && header[1] == 0x45 &&
		header[2] == 0x53 && header[3] == 0x1a;
}