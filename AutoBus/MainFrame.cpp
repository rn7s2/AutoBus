#include "MainFrame.h"
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/msgdlg.h>

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_PAINT(OnPaint)
EVT_BUTTON(ButtonStart, MainFrame::OnButtonStart)
EVT_BUTTON(ButtonEnd, MainFrame::OnButtonEnd)
EVT_BUTTON(ButtonTarget, MainFrame::OnButtonTarget)
EVT_BUTTON(ButtonCounterclockwise, MainFrame::OnButtonCounterclockwise)
EVT_BUTTON(ButtonClockwise, MainFrame::OnButtonClockwise)
EVT_TIMER(AppTimer, OnAppTimer)
EVT_TIMER(BusTimer, OnBusTimer)
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

	appTimer = new wxTimer(this, AppTimer);
	appTimer->Start(1000);

	busTimer = new wxTimer(this, BusTimer);
}

MainFrame::~MainFrame()
{
	appTimer->Stop();
	if (bus != nullptr) {
		busTimer->Stop();
		delete bus, bus = nullptr;
	}
}

void MainFrame::Draw()
{
	wxBitmap map(wxT("Assets/MapFlat.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap flag(wxT("Assets/Flag.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap panel;
	if (bus != nullptr)
		panel.LoadFile(wxT("Assets/PanelStarted.bmp"), wxBITMAP_TYPE_BMP);
	else
		panel.LoadFile(wxT("Assets/Panel.bmp"), wxBITMAP_TYPE_BMP);

	wxBitmap::Rescale(map, wxSize(800, 561));
	wxBitmap::Rescale(flag, wxSize(50, 100));
	wxBitmap::Rescale(panel, wxSize(250, 561));

	wxClientDC clientDC(this);
	wxBufferedDC dc(&clientDC, wxSize(1000, 600));
	dc.DrawBitmap(map, wxPoint(-10, 0));
	dc.DrawBitmap(panel, wxPoint(734, 0));
	dc.SetBrush(wxBrush(*wxBLACK));
	dc.DrawRectangle(wxRect(795, 4, 180, 18));
	dc.SetTextForeground(*wxYELLOW);
	dc.DrawText(
		_("App running: ") + wxString::Format(wxT("%d"), time) + _(" sec(s)"),
		wxPoint(795, 4)
	);
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
	wxBitmap map(wxT("Assets/MapFlat.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap flag(wxT("Assets/Flag.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap panel(wxT("Assets/Panel.bmp"), wxBITMAP_TYPE_BMP);

	wxBitmap::Rescale(map, wxSize(800, 561));
	wxBitmap::Rescale(flag, wxSize(50, 100));
	wxBitmap::Rescale(panel, wxSize(250, 561));

	wxBufferedPaintDC dc(this);
	dc.DrawBitmap(map, wxPoint(-10, 0));
	dc.DrawBitmap(panel, wxPoint(734, 0));
	dc.SetBrush(wxBrush(*wxBLACK));
	dc.DrawRectangle(wxRect(795, 4, 180, 18));
	dc.SetTextForeground(*wxYELLOW);
	dc.DrawText(
		_("App running: ") + wxString::Format(wxT("%d"), time) + _(" sec(s)"),
		wxPoint(795, 4)
	);
}

void MainFrame::OnButtonStart(wxCommandEvent& event)
{
	if (bus != nullptr) {
		wxMessageBox(_("Bus had started."));
		return;
	}
	bus = new AutoBus();
	busTimer->Start(20);
}

void MainFrame::OnButtonEnd(wxCommandEvent& event)
{
	if (bus == nullptr) {
		wxMessageBox("Bus not started.");
		return;
	}
	busTimer->Stop();
	delete bus, bus = nullptr;
	frames = 0;
	Refresh();
}

void MainFrame::OnButtonTarget(wxCommandEvent& event)
{
	if (bus == nullptr) {
		wxMessageBox("Bus not started.");
		return;
	}

	Instruction t;
	t.type = 4;
	t.station = targetSpinCtrl->GetValue();
	bus->Dispatch(t);
}

void MainFrame::OnButtonCounterclockwise(wxCommandEvent& event)
{
	if (bus == nullptr) {
		wxMessageBox("Bus not started.");
		return;
	}

	Instruction t;
	t.type = 2;
	t.station = counterclockwiseSpinCtrl->GetValue();
	bus->Dispatch(t);
}

void MainFrame::OnButtonClockwise(wxCommandEvent& event)
{
	if (bus == nullptr) {
		wxMessageBox("Bus not started.");
		return;
	}

	Instruction t;
	t.type = 3;
	t.station = clockwiseSpinCtrl->GetValue();
	bus->Dispatch(t);
}

void MainFrame::OnAppTimer(wxTimerEvent& event)
{
	time++;

	wxClientDC dc(this);
	dc.SetBrush(wxBrush(*wxBLACK));
	dc.DrawRectangle(wxRect(795, 4, 180, 18));
	dc.SetTextForeground(*wxYELLOW);
	dc.DrawText(
		_("App running: ") + wxString::Format(wxT("%d"), time) + _(" sec(s)"),
		wxPoint(795, 4)
	);
}

void MainFrame::OnBusTimer(wxTimerEvent& event)
{
	if (frames % 50 == 0) {
		Instruction t;
		t.type = 1;
		t.station = 0;
		bus->Dispatch(t);
	}

	// draw
	Draw();
}
