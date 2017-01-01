#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "DebugVerifier.h"
#include "CPU.h"
#include "Cartridge.h"

#include "DbgWin.h"

#include "EasyLogging++.h"

INITIALIZE_EASYLOGGINGPP

DWORD WINAPI TestCPU(LPVOID lpParam);

DbgWin* dbgWin;

class NesPlusApp : public wxApp
{
public:
	virtual bool OnInit();
};
class NesPlusFrame : public wxFrame
{
public:
	NesPlusFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
	void OnHello(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnDbgWin(wxCommandEvent& event);
	void LoadRom(wxCommandEvent& event);
	void NESTEST(wxCommandEvent& event);
	void INSTRTEST(wxCommandEvent& event);
	wxDECLARE_EVENT_TABLE();
};
enum
{
	ID_Hello = 1,
	ID_NESTEST = 2,
	ID_INSTRTEST = 3,
	ID_LOAD = 4,
	ID_DBGWIN = 5
};
wxBEGIN_EVENT_TABLE(NesPlusFrame, wxFrame)
EVT_MENU(ID_LOAD, NesPlusFrame::LoadRom)
EVT_MENU(ID_Hello, NesPlusFrame::OnHello)
EVT_MENU(wxID_EXIT, NesPlusFrame::OnExit)
EVT_MENU(wxID_ABOUT, NesPlusFrame::OnAbout)
EVT_MENU(ID_NESTEST, NesPlusFrame::NESTEST)
EVT_MENU(ID_INSTRTEST, NesPlusFrame::INSTRTEST)
EVT_MENU(ID_DBGWIN, NesPlusFrame::OnDbgWin)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(NesPlusApp);

bool NesPlusApp::OnInit()
{
	LOG(DEBUG) << "Initializing NesPlus...";
	NesPlusFrame *frame = new NesPlusFrame("Hello World", wxPoint(50, 50), wxSize(450, 340));
	frame->Show(true);
	return true;
}
NesPlusFrame::NesPlusFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_LOAD, "&Load Rom", "Load NES rom");
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
		"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenu *menuDev = new wxMenu;
	menuDev->Append(ID_DBGWIN, "Debug Window", "Debug Window");
	menuDev->Append(ID_NESTEST, "&NesTest.nes", "Test");
	menuDev->Append(ID_INSTRTEST, "InstrTes.nes", "InstructionTest");
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	menuBar->Append(menuDev, "&Dev");
	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
}
void NesPlusFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void NesPlusFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets' Hello world sample",
		"About Hello World", wxOK | wxICON_INFORMATION);
}
void NesPlusFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}

void NesPlusFrame::NESTEST(wxCommandEvent& event) {
	Cartridge *cartridge = new Cartridge("nestest.nes");
	DWORD threadId;
	HANDLE cpuThread = CreateThread(NULL, 0, TestCPU, cartridge, 0, &threadId);
}

void NesPlusFrame::INSTRTEST(wxCommandEvent& event) {

}

void NesPlusFrame::OnDbgWin(wxCommandEvent& event) {
	dbgWin = new DbgWin("Debug", wxPoint(100, 100), wxSize(450, 200));
	dbgWin->Show(true);
	return;
}

void NesPlusFrame::LoadRom(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, _("Open NES file"), "", "", "Nes Files (*.nes)|*.nes", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...
	wxString path = openFileDialog.GetPath();

	Cartridge *cartridge = new Cartridge(path.ToStdString());
	CPU * cpu = new CPU();

	cpu->setProgramRom(cartridge->getProgramRom(), cartridge->getProgramRomSize());
	cpu->start();


	delete cartridge;
	
}

DWORD WINAPI TestCPU(LPVOID lpParam) {
	LOG(INFO) << "Performing CPU Test...";
	Cartridge *cartridge = (Cartridge*)lpParam;
	DebugVerifier *debugVerifier = new DebugVerifier();

	CPU *cpu = new CPU();
	cpu->setDbgWin(dbgWin);
	cpu->setDebugVerifier(debugVerifier);
	cpu->setProgramRom(cartridge->getProgramRom(), cartridge->getProgramRomSize());
	cpu->start();

	if (cpu->getTestSuccess()) {
		LOG(INFO) << "Test Succeeded";
		wxMessageBox("CPU Test Succeeded!",
			"NesPlus", wxOK | wxICON_INFORMATION);
	}
	else {
		LOG(INFO) << "Test Failed";
		wxMessageBox("CPU Test Failed", "NesPlus", wxOK | wxICON_INFORMATION);
	}

	delete cpu;

	delete debugVerifier;

	delete cartridge;

	return 0;
}