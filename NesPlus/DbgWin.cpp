#include "DbgWin.h"
#include <sstream>

DbgWin::DbgWin(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size) {

	grid = new wxGrid(this, -1, wxPoint(0, 0), wxSize(100, 100));
	grid->CreateGrid(3, 6);
	grid->SetRowLabelSize(0);
	grid->SetColLabelSize(0);
	grid->DisableCellEditControl();

	grid->SetCellValue(0, 0, "A: ");
	grid->SetCellValue(0, 2, "X: ");
	grid->SetCellValue(0, 4, "Y: ");

	grid->SetCellValue(1, 0, "P: ");
	grid->SetCellValue(1, 2, "SP: ");
	grid->SetCellValue(1, 4, "PC: ");

}

wxString DbgWin::unsignedCharToString(unsigned char val) {
	std::ostringstream oss;
	oss << std::hex << (int)val;
	return wxString(oss.str());
}

wxString DbgWin::unsignedShortToString(unsigned short val) {
	std::ostringstream oss;
	oss << std::hex << (int)val;
	return wxString(oss.str());
}

void DbgWin::updateARegister(unsigned char val) {
	grid->SetCellValue(0, 1, unsignedCharToString(val));
}

void DbgWin::updateXRegister(unsigned char val) {
	grid->SetCellValue(0, 3, unsignedCharToString(val));
}

void DbgWin::updateYRegister(unsigned char val) {
	grid->SetCellValue(0, 5, unsignedCharToString(val));
}

void DbgWin::updatePC(unsigned short val) {
	grid->SetCellValue(1, 5, unsignedShortToString(val));
}