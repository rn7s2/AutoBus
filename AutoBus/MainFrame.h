#pragma once

#include "AutoBus.h"
#include <wx/frame.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include <wx/bitmap.h>

enum ControlID {
	ButtonTarget = wxID_HIGHEST + 1,
	ButtonCounterclockwise,
	ButtonClockwise,
	ButtonStart,
	ButtonEnd,
	SpinCtrlTarget,
	SpinCtrlCounterclockwise,
	SpinCtrlClockwise,
	AppTimer,
	BusTimer
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	~MainFrame();

private:
	wxPoint LengthToPoint(int l);
	void Draw();
	void OnPaint(wxPaintEvent& event);
	void OnButtonStart(wxCommandEvent& event);
	void OnButtonEnd(wxCommandEvent& event);
	void OnButtonTarget(wxCommandEvent& event);
	void OnButtonCounterclockwise(wxCommandEvent& event);
	void OnButtonClockwise(wxCommandEvent& event);
	void OnAppTimer(wxTimerEvent& event);
	void OnBusTimer(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();

private:
	AutoBus* bus = nullptr;
	wxButton* targetButton = nullptr;
	wxButton* counterclockwiseButton = nullptr;
	wxButton* clockwiseButton = nullptr;
	wxButton* startButton = nullptr;
	wxButton* endButton = nullptr;
	wxSpinCtrl* targetSpinCtrl = nullptr;
	wxSpinCtrl* counterclockwiseSpinCtrl = nullptr;
	wxSpinCtrl* clockwiseSpinCtrl = nullptr;
	wxTimer* appTimer = nullptr;
	wxTimer* busTimer = nullptr;
	int frames = 0, time = 0;

	wxBitmap map;
	wxBitmap flag;
	wxBitmap panel, panelStarted;
	wxBitmap vehicle[8];
};
