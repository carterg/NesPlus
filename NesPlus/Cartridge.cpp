#include "Cartridge.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include<Windows.h>
#include "EasyLogging++.h"

Cartridge::Cartridge(std::string romName) {
	this->romName = romName;
	LOG(INFO) << "Loading Rom " << romName;
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
		OutputDebugString(L"Could not open file\n ");
		return false;
	}
	
	unsigned char header[16];
	fread(header, 16, 1, file);
	if (!verifyHeader(header)) {
		OutputDebugString(L"Header was invalid\n ");
	}
	else {
		OutputDebugString(L"Header was valid\n ");
	}
	
	int sizeOfPRGROM = header[4];
	prgROMSize = sizeOfPRGROM * 16 * 1024;
	prgROM = new unsigned char[prgROMSize];
	
	OutputDebugString(L"created prgROM\n ");

	int sizeOfCHRROM = header[5];
	chrROM = new unsigned char[sizeOfCHRROM * 8 * 1024];
	
	size_t read = fread(prgROM, 1, sizeOfPRGROM * 16 * 1024, file);
	
	return true;
}

bool Cartridge::verifyHeader(unsigned char header[16]) {
	return header[0] == 0x4e && header[1] == 0x45 &&
		header[2] == 0x53 && header[3] == 0x1a;
}