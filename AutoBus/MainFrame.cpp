#include "MainFrame.h"
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_PAINT(MainFrame::OnPaint)
	EVT_BUTTON(ButtonStart, MainFrame::OnButtonStart)
	EVT_BUTTON(ButtonEnd, MainFrame::OnButtonEnd)
	EVT_BUTTON(ButtonTarget, MainFrame::OnButtonTarget)
	EVT_BUTTON(ButtonCounterclockwise, MainFrame::OnButtonCounterclockwise)
	EVT_BUTTON(ButtonClockwise, MainFrame::OnButtonClockwise)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
	: wxFrame(
		nullptr,
		wxID_ANY,
		title,
		wxPoint(120, 30),
		wxSize(1000, 600),
		wxMINIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN
	)
{
	SetBackgroundColour(*wxWHITE);

	const int buttonX = 768;
	const wxSize buttonSize(100, 30);
	const int spinCtrlX = buttonX + 109;
	const wxSize spinCtrlSize(73, 30);

	wxButton* buttonTarget = new wxButton(
		this,
		ButtonTarget,
		_("Target"),
		wxPoint(buttonX, 290),
		buttonSize
	);
	wxButton* buttonCounterclockwise = new wxButton(
		this,
		ButtonCounterclockwise,
		_("Counter"),
		wxPoint(buttonX, 326),
		buttonSize
	);
	wxButton* buttonClockwise = new wxButton(
		this,
		ButtonClockwise,
		_("Clockwise"),
		wxPoint(buttonX, 362),
		buttonSize
	);
	wxButton* buttonStart = new wxButton(
		this,
		ButtonStart,
		_("Start"),
		wxPoint(buttonX, 398),
		wxSize(180, 30)
	);
	wxButton* buttonEnd = new wxButton(
		this,
		ButtonEnd,
		_("End"),
		wxPoint(buttonX, 434),
		wxSize(180, 30)
	);

	targetSpinCtrl = new wxSpinCtrl(
		this,
		SpinCtrlTarget,
		wxEmptyString,
		wxPoint(spinCtrlX, 290),
		spinCtrlSize,
		wxSP_ARROW_KEYS,
		0,
		9,
		0
	);
	counterclockwiseSpinCtrl = new wxSpinCtrl(
		this,
		SpinCtrlCounterclockwise,
		wxEmptyString,
		wxPoint(spinCtrlX, 326),
		spinCtrlSize,
		wxSP_ARROW_KEYS,
		0,
		9,
		0
	);
	clockwiseSpinCtrl = new wxSpinCtrl(
		this,
		SpinCtrlClockwise,
		wxEmptyString,
		wxPoint(spinCtrlX, 362),
		spinCtrlSize,
		wxSP_ARROW_KEYS,
		0,
		9,
		0
	);
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
	wxBitmap map(wxT("Assets/MapFlat.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap flag(wxT("Assets/Flag.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap panel;
	if (bus.state.last_state != -1) {
		panel.LoadFile(wxT("Assets/PanelStarted.bmp"), wxBITMAP_TYPE_BMP);
	}
	else {
		panel.LoadFile(wxT("Assets/Panel.bmp"), wxBITMAP_TYPE_BMP);
	}

	wxBitmap::Rescale(map, wxSize(800, 561));
	wxBitmap::Rescale(flag, wxSize(50, 100));
	wxBitmap::Rescale(panel, wxSize(250, 561));

	wxPaintDC paintDC(this);
	wxBufferedDC dc(&paintDC, wxSize(1000, 600));
	dc.DrawBitmap(map, wxPoint(-10, 0));
	dc.DrawBitmap(panel, wxPoint(734, 0));
}

void MainFrame::OnButtonStart(wxCommandEvent& event)
{
	Refresh();
}

void MainFrame::OnButtonEnd(wxCommandEvent& event)
{

}

void MainFrame::OnButtonTarget(wxCommandEvent& event)
{

}

void MainFrame::OnButtonCounterclockwise(wxCommandEvent& event)
{

}

void MainFrame::OnButtonClockwise(wxCommandEvent& event)
{

}
