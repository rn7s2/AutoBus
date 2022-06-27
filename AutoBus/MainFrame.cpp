#include "MainFrame.h"
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/msgdlg.h>
#include <wx/imagpng.h>
#include <wx/file.h>
#include <wx/filedlg.h>

const wxPoint Vertex[10] = {
	wxPoint(220, 160),
	wxPoint(360, 160),
	wxPoint(500, 160),
	wxPoint(610, 220),
	wxPoint(610, 360),
	wxPoint(500, 420),
	wxPoint(360, 420),
	wxPoint(220, 420),
	wxPoint(112, 360),
	wxPoint(112, 220)
};

const int Station[11][10] = {
	{},
	{0},
	{0, 5},
	{1, 4, 8},
	{0, 2, 5, 7},
	{1, 3, 5, 7, 9},
	{1, 3, 4, 6, 8, 9},
	{1, 3, 4, 5, 7, 8, 9},
	{2, 3, 4, 5, 7, 8, 9, 0},
	{0, 1, 2, 3, 4, 5, 7, 8, 9},
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
};

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
	wxImage::AddHandler(new wxPNGHandler);

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
	busTimer->Stop();
	if (bus != nullptr) {
		busTimer->Stop();
		delete bus, bus = nullptr;
	}
}

void MainFrame::Draw()
{
	wxBitmap map(wxT("Assets/MapFlat.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap flag(wxT("Assets/Flag.png"), wxBITMAP_TYPE_PNG);
	wxBitmap panel(wxT("Assets/PanelStarted.bmp"), wxBITMAP_TYPE_BMP);

	// Rescale bitmaps
	wxBitmap::Rescale(map, wxSize(800, 561));
	wxBitmap::Rescale(flag, wxSize(50, 100));
	wxBitmap::Rescale(panel, wxSize(250, 561));

	wxClientDC clientDC(this);
	wxBufferedDC dc(&clientDC, wxSize(1000, 600));

	// Draw map and panel
	dc.DrawBitmap(map, wxPoint(-10, 0));
	dc.DrawBitmap(panel, wxPoint(734, 0));

	// Draw stations
	for (int i = 0; i < bus->config.total_station; i++) {
		int s = Station[bus->config.total_station][i];
		if ((s >= 0 && s <= 2) || (s >= 5 && s <= 7)) {
			dc.DrawBitmap(
				flag,
				wxPoint(Vertex[s].x - 15, Vertex[s].y - 110),
				true
			);
		} else if (s == 8 || s == 9) {
			dc.DrawBitmap(
				flag,
				wxPoint(Vertex[s].x - 65, Vertex[s].y - 80),
				true
			);
		} else if (s == 3 || s == 4) {
			dc.DrawBitmap(
				flag,
				wxPoint(Vertex[s].x + 20, Vertex[s].y - 80),
				true
			);
		}
	}

	// Draw time
	dc.SetBrush(wxBrush(*wxBLACK));
	dc.DrawRectangle(wxRect(795, 4, 180, 18));
	dc.SetTextForeground(*wxYELLOW);
	dc.DrawText(
		_("App running: ") + wxString::Format(wxT("%d"), time) + _(" sec(s)"),
		wxPoint(795, 4)
	);

	dc.SetTextForeground(*wxWHITE);
	dc.DrawText(
		wxString::Format(wxT("Time: %d"), bus->state.time),
		wxPoint(795, 100)
	);
	dc.DrawText(
		wxString::Format(wxT("Position: %d"), bus->state.position),
		wxPoint(795, 120)
	);
	wxString buf = wxEmptyString;
	for (int i = 1; i <= bus->config.total_station; i++)
		buf << bus->state.target[i];
	dc.DrawText(
		wxString::Format(wxT("Target: %s"), buf),
		wxPoint(795, 140)
	);
	buf = wxEmptyString;
	for (int i = 1; i <= bus->config.total_station; i++)
		buf << bus->state.clockwise_request[i];
	dc.DrawText(
		wxString::Format(wxT("Clockwise: %s"), buf),
		wxPoint(795, 160)
	);
	buf = wxEmptyString;
	for (int i = 1; i <= bus->config.total_station; i++)
		buf << bus->state.counterclockwise_request[i];
	dc.DrawText(
		wxString::Format(wxT("Counterclockwise: %s"), buf),
		wxPoint(795, 180)
	);
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
	wxBitmap map(wxT("Assets/MapFlat.bmp"), wxBITMAP_TYPE_BMP);
	wxBitmap flag(wxT("Assets/Flag.png"), wxBITMAP_TYPE_PNG);
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

	wxFileDialog d(
		this,
		_("Select config file..."),
		wxEmptyString,
		wxT("dict.dic"),
		_("AutoBus Configuration File (*.dic) | *.dic"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	if (d.ShowModal() == wxID_OK) {
		bus = new AutoBus(d.GetPath().ToStdString());
		busTimer->Start(100);
	}
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
	frames = (frames + 1) % 10;
	if (frames == 0) {
		Instruction t;
		t.type = 1;
		t.station = 0;
		bus->Dispatch(t);
	}

	// draw
	Draw();
}
