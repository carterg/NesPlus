#include "DebugVerifierLineItem.h"

DebugVerifierLineItem::DebugVerifierLineItem(unsigned short pc, unsigned char opcode, unsigned char a, unsigned char x, unsigned char y, unsigned char p, unsigned short sp, unsigned short cyc) :
pc(pc), opcode(opcode), a(a), x(x), y(y), p(p), sp(sp), cyc(cyc)
{

}

DebugVerifierLineItem::DebugVerifierLineItem(std::string line) {
	std::stringstream ss;
	ss << std::hex << line.substr(0, 4);
	int t;
	ss >> t;
	pc = t & 0xFFFF;

	ss.str(std::string());
	ss.clear();
	ss << std::hex << line.substr(6, 2);
	ss >> t;
	opcode = t & 0xFF;

	ss.str(std::string());
	ss.clear();
	ss << std::hex << line.substr(50, 2);
	ss >> t;
	a = t & 0xFF;

	ss.str(std::string());
	ss.clear();
	ss << std::hex << line.substr(55, 2);
	ss >> t;
	x = t & 0xFF;

	ss.str(std::string());
	ss.clear();
	ss << std::hex << line.substr(60, 2);
	ss >> t;
	y = t & 0xFF;

	ss.str(std::string());
	ss.clear();
	ss << std::hex << line.substr(65, 2);
	ss >> t;
	p = t & 0xFF;

	ss.str(std::string());
	ss.clear();
	ss << std::hex << line.substr(71, 2);
	ss >> t;
	sp = t & 0xFF;

	ss.str(std::string());
	ss.clear();
	ss << std::dec << line.substr(78, 3);
	ss >> t;
	cyc = t & 0xFFFF;
}