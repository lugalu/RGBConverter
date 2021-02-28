#pragma once

#include "wx/wx.h"
#include "wx/frame.h"
#include "wx/string.h"
#include "wx/clrpicker.h"
#include "wx/colordlg.h"
#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <iomanip>


#define DEFAULT_WIDTH 300
#define DEFAULT_HEIGHT 600
#define RGB_SLOTS 3
#define CMYK_SLOTS 4




class cMain : public wxFrame{
	public:
		cMain();
		~cMain();
private:
	int rgbSizeX = 60;
	int rgbSizeY = 20;
	int offset = 5;
	int defaultXPosition = (DEFAULT_WIDTH / 2) + rgbSizeX / 2 + 5*offset;

	//static text
	wxStaticText* rStaticText = nullptr;
	wxStaticText* gStaticText = nullptr;
	wxStaticText* bStaticText = nullptr;
	wxStaticText* rgb255Static = nullptr;
	wxStaticText* rgb1Static = nullptr;
	wxStaticText* hexStatic = nullptr;

	wxStaticText* cStaticText = nullptr;
	wxStaticText* mStaticText = nullptr;
	wxStaticText* kStaticText = nullptr;
	wxStaticText* yStaticText = nullptr;

	//text fields
	wxTextCtrl** rgb255TextField;
	wxTextCtrl** rgb1TextField;
	wxTextCtrl** cmykTextField;
	wxTextCtrl* hexTextField = nullptr;
	
	//color panel
	wxPanel* panel = nullptr;
	wxBoxSizer* sizer = nullptr;

	//events
	
	void onTextChanged(wxCommandEvent &event);


	//functions
	wxArrayString cMain::generateList(std::string text);
	void cMain::rgb255ToCmyk(int r, int g, int b);
	void cMain::cmykToRgb255(double c, double m, double y, double k);
	double rgb255To1(int value);
	int rgb1To255(double value);	
	std::string intToHex(int value);
	void cMain::initializeStaticTexts();

	wxDECLARE_EVENT_TABLE();
};

