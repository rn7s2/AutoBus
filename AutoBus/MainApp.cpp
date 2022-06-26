#include "MainApp.h"
#include "MainFrame.h"
#include <wx/splash.h>

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit()
{
	wxBitmap bitmap;
	if (bitmap.LoadFile(wxT("Assets/Splash.bmp"), wxBITMAP_TYPE_BMP)) {
		wxSplashScreen* splash = new wxSplashScreen(
			bitmap,
			wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
			2500,
			nullptr,
			wxID_ANY,
			wxDefaultPosition,
			wxDefaultSize,
			wxBORDER_SIMPLE | wxSTAY_ON_TOP
		);
	}

	MainFrame* frame = new MainFrame(_("AutoBus (GUI) - Red Alert 2"));
	return frame->Show(true);

	wxYield();
}
