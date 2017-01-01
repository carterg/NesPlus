#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

class DbgWin : public wxFrame {
public:
	DbgWin(const wxString& title, const wxPoint& pos, const wxSize& size);
	void updateARegister(unsigned char val);
	void updateXRegister(unsigned char val);
	void updateYRegister(unsigned char val);
	void updatePC(unsigned short val);
private:
	wxGrid *grid;
	wxString unsignedCharToString(unsigned char val);
	wxString unsignedShortToString(unsigned short val);
	//wxDECLARE_EVENT_TABLE();
};