#include "MainFrame.h"
#include <wx/button.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/msgdlg.h>
#include <wx/imagpng.h>
#include <wx/file.h>
#include <wx/filedlg.h>

const int FPS = 45;
const int VertexToLength[10] = { 0, 140, 280, 405, 545, 670, 810, 950, 1075, 1215 };

//const wxPoint Vertex[10] = {
//	wxPoint(220, 160), // l = 0
//	wxPoint(360, 160),
//	wxPoint(500, 160), // l = 280
//	wxPoint(610, 220), // l = 405
//	wxPoint(610, 360), // l = 545
//	wxPoint(500, 420), // l = 670
//	wxPoint(360, 420),
//	wxPoint(220, 420), // l = 950
//	wxPoint(112, 360), // l = 1075
//	wxPoint(112, 220)  // l = 1215
//};

const int StationToVertex[11][10] = {
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

	map = wxBitmap(wxT("Assets/MapFlat.bmp"), wxBITMAP_TYPE_BMP);
	flag = wxBitmap(wxT("Assets/Flag.png"), wxBITMAP_TYPE_PNG);
	panel = wxBitmap(wxT("Assets/Panel.bmp"), wxBITMAP_TYPE_BMP);
	panelStarted = wxBitmap(wxT("Assets/PanelStarted.bmp"), wxBITMAP_TYPE_BMP);
	for (int i = 0; i < 8; i++)
		vehicle[i] = wxBitmap(wxString::Format(wxT("Assets/Bus/%d.png"), i), wxBITMAP_TYPE_PNG);

	wxBitmap::Rescale(vehicle[0], wxSize(42, 34));
	wxBitmap::Rescale(vehicle[1], wxSize(46, 36));
	wxBitmap::Rescale(vehicle[2], wxSize(42, 34));
	wxBitmap::Rescale(vehicle[3], wxSize(46, 36));
	wxBitmap::Rescale(vehicle[4], wxSize(42, 34));
	wxBitmap::Rescale(vehicle[5], wxSize(46, 36));
	wxBitmap::Rescale(vehicle[6], wxSize(42, 34));
	wxBitmap::Rescale(vehicle[7], wxSize(46, 36));

	wxBitmap::Rescale(map, wxSize(800, 561));
	wxBitmap::Rescale(flag, wxSize(50, 100));
	wxBitmap::Rescale(panel, wxSize(250, 561));
	wxBitmap::Rescale(panelStarted, wxSize(250, 561));

	SetBackgroundColour(*wxWHITE);

	const int buttonX = 768;
	const wxSize buttonSize(100, 30);
	const int spinCtrlX = buttonX + 109;
	const wxSize spinCtrlSize(73, 30);

	targetButton = new wxButton(
		this,
		ButtonTarget,
		_("Target"),
		wxPoint(buttonX, 290),
		buttonSize
	);
	counterclockwiseButton = new wxButton(
		this,
		ButtonCounterclockwise,
		_("Counter"),
		wxPoint(buttonX, 326),
		buttonSize
	);
	clockwiseButton = new wxButton(
		this,
		ButtonClockwise,
		_("Clockwise"),
		wxPoint(buttonX, 362),
		buttonSize
	);
	startButton = new wxButton(
		this,
		ButtonStart,
		_("Start"),
		wxPoint(buttonX, 398),
		wxSize(180, 30)
	);
	endButton = new wxButton(
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

	targetButton->Disable();
	counterclockwiseButton->Disable();
	clockwiseButton->Disable();
	endButton->Disable();
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

wxPoint MainFrame::LengthToPoint(int l)
{
	if (l >= 0 && l < 280) {
		return wxPoint(220 + l, 160);
	} else if (l >= 280 && l < 405) {
		double n = (l - 280.0) / fabs(6.0 * 6 + 11 * 11);
		return wxPoint(500 + 110 * n, 160 + 60 * n);
	} else if (l >= 405 && l < 545) {
		return wxPoint(610, 220 + l - 405);
	} else if (l >= 545 && l < 670) {
		double n = (l - 545.0) / fabs(6.0 * 6 + 11 * 11);
		return wxPoint(610 - 110 * n, 360 + 60 * n);
	} else if (l >= 670 && l < 950) {
		return wxPoint(500 - (l - 670), 420);
	} else if (l >= 950 && l < 1075) {
		double n = (l - 950.0) / fabs(6.0 * 6 + 11 * 11);
		return wxPoint(220 - 110 * n, 420 - 60 * n);
	} else if (l >= 1075 && l < 1215) {
		return wxPoint(112, 360 - (l - 1075));
	} else if (l >= 1215 && l <= 1355) {
		double n = (l - 1215.0) / fabs(6.0 * 6 + 11 * 11);
		return wxPoint(112 + 110 * n, 220 - 60 * n);
	}
	return wxPoint(-1, -1);
}

void MainFrame::Draw()
{
	wxClientDC clientDC(this);
	wxBufferedDC dc(&clientDC, wxSize(1000, 600));

	// Draw map and panel
	dc.DrawBitmap(map, wxPoint(-10, 0));
	dc.DrawBitmap(panelStarted, wxPoint(734, 0));

	// Draw stations
	for (int i = 0; i < bus->config.total_station; i++) {
		int s = StationToVertex[bus->config.total_station][i];
		if ((s >= 0 && s <= 2) || (s >= 5 && s <= 7)) {
			// »­Õ¾µã±àºÅ
			dc.SetTextForeground(*wxRED);
			dc.DrawText(
				wxString::Format(wxT("%d"), i + 1),
				wxPoint(
					LengthToPoint(VertexToLength[s]).x - 30,
					LengthToPoint(VertexToLength[s]).y - 35
				)
			);
			// »­ÆìÖÄ
			dc.DrawBitmap(
				flag,
				wxPoint(
					LengthToPoint(VertexToLength[s]).x - 15,
					LengthToPoint(VertexToLength[s]).y - 110
				),
				true
			);
		} else if (s == 8 || s == 9) {
			dc.SetTextForeground(*wxRED);
			dc.DrawText(
				wxString::Format(wxT("%d"), i + 1),
				wxPoint(
					LengthToPoint(VertexToLength[s]).x - 25,
					LengthToPoint(VertexToLength[s]).y - 25
				)
			);
			dc.DrawBitmap(
				flag,
				wxPoint(
					LengthToPoint(VertexToLength[s]).x - 65,
					LengthToPoint(VertexToLength[s]).y - 80
				),
				true
			);
		} else if (s == 3 || s == 4) {
			dc.SetTextForeground(*wxRED);
			dc.DrawText(
				wxString::Format(wxT("%d"), i + 1),
				wxPoint(
					LengthToPoint(VertexToLength[s]).x + 25,
					LengthToPoint(VertexToLength[s]).y - 25
				)
			);
			dc.DrawBitmap(
				flag,
				wxPoint(
					LengthToPoint(VertexToLength[s]).x + 20,
					LengthToPoint(VertexToLength[s]).y - 80
				),
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

	// Draw state
	dc.SetTextForeground(*wxWHITE);
	dc.DrawText(
		wxString::Format(wxT("Time: %d"), bus->state.time),
		wxPoint(775, 80)
	);
	dc.DrawText(
		wxString::Format(wxT("Position: %d"), bus->state.position),
		wxPoint(775, 100)
	);
	wxString buf = wxEmptyString;
	for (int i = 1; i <= bus->config.total_station; i++)
		buf << bus->state.target[i];
	dc.DrawText(
		wxString::Format(wxT("Target: %s"), buf),
		wxPoint(775, 120)
	);
	buf = wxEmptyString;
	for (int i = 1; i <= bus->config.total_station; i++)
		buf << bus->state.clockwise_request[i];
	dc.DrawText(
		wxString::Format(wxT("Clockwise: %s"), buf),
		wxPoint(775, 140)
	);
	buf = wxEmptyString;
	for (int i = 1; i <= bus->config.total_station; i++)
		buf << bus->state.counterclockwise_request[i];
	dc.DrawText(
		wxString::Format(wxT("Counterclockwise: %s"), buf),
		wxPoint(775, 160)
	);

	// Draw vehicle
	int distance = bus->config.distance;
	int totalStation = bus->config.total_station;
	int station = bus->state.position / distance;
	int rem = bus->state.position - station * distance;
	int lenA = VertexToLength[StationToVertex[totalStation][station]];
	int intervalLen;
	if (station == totalStation - 1)
		intervalLen = VertexToLength[StationToVertex[totalStation][0]] + 1355 - lenA;
	else
		intervalLen = VertexToLength[StationToVertex[totalStation][station + 1]] - VertexToLength[StationToVertex[totalStation][station]];
	double len = lenA + 1.0 * intervalLen * rem / distance;
	if (bus->state.state == 1)
		len -= (1.0 * intervalLen / distance / FPS) * frames;
	else if (bus->state.state == 3)
		len += (1.0 * intervalLen / distance / FPS) * frames;
	int l = (int)len % 1355;
	static int picID = 0, fixX = -15, fixY = -25;
	if (l >= 0 && l < 280) {
		fixX = -15, fixY = -25;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 4;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 0;
	} else if (l >= 280 && l < 405) {
		fixX = -15, fixY = -25;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 3;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 7;
	} else if (l >= 405 && l < 545) {
		fixX = -20, fixY = -25;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 2;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 6;
	} else if (l >= 545 && l < 670) {
		fixX = -15, fixY = -25;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 1;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 5;
	} else if (l >= 670 && l < 950) {
		fixX = -20, fixY = -20;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 0;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 4;
	} else if (l >= 950 && l < 1075) {
		fixX = -25, fixY = -25;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 7;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 3;
	} else if (l >= 1075 && l < 1215) {
		fixX = -20, fixY = -25;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 6;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 2;
	} else if (l >= 1215 && l <= 1355) {
		fixX = -25, fixY = -20;
		if (bus->state.state == 1 || bus->state.last_state == 1)
			picID = 5;
		else if (bus->state.state == 3 || bus->state.last_state == 3)
			picID = 1;
	}
	dc.DrawBitmap(vehicle[picID], LengthToPoint(l) + wxPoint(fixX, fixY), true);
}

void MainFrame::OnPaint(wxPaintEvent& event)
{
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
		targetSpinCtrl->SetRange(1, bus->config.total_station);
		counterclockwiseSpinCtrl->SetRange(1, bus->config.total_station);
		clockwiseSpinCtrl->SetRange(1, bus->config.total_station);
		busTimer->Start(1500 / FPS);
		startButton->Disable();
		endButton->Enable();
		targetButton->Enable();
		counterclockwiseButton->Enable();
		clockwiseButton->Enable();
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
	targetSpinCtrl->SetRange(0, 0);
	counterclockwiseSpinCtrl->SetRange(0, 0);
	clockwiseSpinCtrl->SetRange(0, 0);
	counterclockwiseSpinCtrl->SetValue(1);
	clockwiseSpinCtrl->SetValue(1);
	targetButton->Disable();
	counterclockwiseButton->Disable();
	clockwiseButton->Disable();
	startButton->Enable();
	endButton->Disable();
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
	frames = (frames + 1) % FPS;
	if (frames == 0) {
		Instruction t;
		t.type = 1;
		t.station = 0;
		bus->Dispatch(t);
	}
	Draw();
}
