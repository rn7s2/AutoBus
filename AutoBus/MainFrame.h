#pragma once

#include "AutoBus.h"
#include <wx/frame.h>
#include <wx/spinctrl.h>

enum ControlID {
	ButtonTarget = wxID_HIGHEST + 1,
	ButtonCounterclockwise,
	ButtonClockwise,
	ButtonStart,
	ButtonEnd,
	SpinCtrlTarget,
	SpinCtrlCounterclockwise,
	SpinCtrlClockwise
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	void OnPaint(wxPaintEvent& event);
	void OnButtonStart(wxCommandEvent& event);
	void OnButtonEnd(wxCommandEvent& event);
	void OnButtonTarget(wxCommandEvent& event);
	void OnButtonCounterclockwise(wxCommandEvent& event);
	void OnButtonClockwise(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();

private:
	AutoBus bus;
	wxSpinCtrl* targetSpinCtrl;
	wxSpinCtrl* counterclockwiseSpinCtrl;
	wxSpinCtrl* clockwiseSpinCtrl;
};
