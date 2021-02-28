#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
wxEND_EVENT_TABLE()


cMain::cMain() : wxFrame(nullptr, wxID_ANY, "RGB Converter", wxPoint(50,50), wxSize(DEFAULT_WIDTH,DEFAULT_HEIGHT)) {
	this->SetBackgroundColour(wxColour(65,65,65,1));
	this->SetMinSize(this->GetSize());
	this->SetMaxSize(this->GetSize());

	sizer = new wxBoxSizer(wxVERTICAL);

	panel = new wxPanel(this, wxID_ANY, wxPoint((DEFAULT_WIDTH / 2) - 50, 100), wxSize(100, 100));
	panel->SetBackgroundColour(*wxBLACK);
	sizer->Add(panel, 1, wxEXPAND);
	
	wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
	wxTextValidator hexValidator(wxFILTER_INCLUDE_CHAR_LIST);

	validator.SetIncludes(generateList(".0123456789"));
	hexValidator.SetIncludes(generateList("ABCDEFabcdef0123456789"));
	
	rgb255TextField = new wxTextCtrl*[RGB_SLOTS];																	
	rgb1TextField = new wxTextCtrl * [RGB_SLOTS];																	
	cmykTextField = new wxTextCtrl*[CMYK_SLOTS];
	
	hexTextField = new wxTextCtrl(this, 10007, "000000",
		wxPoint(defaultXPosition - (2 * rgbSizeX) - offset, 250),
		wxSize(2*rgbSizeX+offset,rgbSizeY),0,hexValidator);
	hexTextField->SetMaxLength(6);
	hexTextField->Bind(wxEVT_COMMAND_TEXT_UPDATED, &cMain::onTextChanged, this);

	initializeStaticTexts();

	for (int i = 0; i < 3; i++) {
		
		rgb255TextField[i] = new wxTextCtrl(this,
			9997 + i,
			"0",
			wxPoint(defaultXPosition -2 * rgbSizeX - offset,
				300 + i * rgbSizeY + i * 5),
			wxSize(rgbSizeX, rgbSizeY),
			0,
			validator);																														//Declaration of each button with it's position being ruled by some math
		rgb255TextField[i]->SetMaxLength(3);
		rgb255TextField[i]->Bind(wxEVT_COMMAND_TEXT_UPDATED,&cMain::onTextChanged,this);

		rgb1TextField[i] = new wxTextCtrl(this,
			10000 + i,
			"0.0",
			wxPoint(defaultXPosition - rgbSizeX,
				300 + i * rgbSizeY + i * 5),
			wxSize(rgbSizeX, rgbSizeY),
			0,
			validator);

		rgb1TextField[i]->Bind(wxEVT_COMMAND_TEXT_UPDATED, &cMain::onTextChanged, this);
	}

	int xPos;
	int yPos;

	for (int i = 0; i < 4; i++) {
		

		if (i % 2 == 0 || i== 0) {
			 xPos = defaultXPosition - 2*rgbSizeX - offset;
			 yPos = 400+ i * rgbSizeY;
		}else {
			xPos = defaultXPosition - rgbSizeX;
		}



		cmykTextField[i] = new wxTextCtrl(this,
			10003 + i,
			"0",
			wxPoint(xPos,yPos),
			wxSize(rgbSizeX, rgbSizeY),
			0,
			validator);
		cmykTextField[i]->SetMaxLength(5);
		cmykTextField[i]->Bind(wxEVT_COMMAND_TEXT_UPDATED, &cMain::onTextChanged, this);
	}
	

}

cMain::~cMain(){
	delete[]rgb255TextField;
	delete[]rgb1TextField;
	delete[]cmykTextField;
	delete[]panel;
	delete[]sizer;
}

void cMain::onTextChanged(wxCommandEvent& event) {
	
	int id = event.GetId() - 10000;
	
	double value = NULL;
	
	if (id<0) {			//add 3 to id. rgb255
		id += 3;
		value = wxAtoi(rgb255TextField[id]->GetValue());
		
		if (value > 255) {
		
			value = 255;
			rgb255TextField[id]->ChangeValue(wxString::Format(wxT("%d"), static_cast<int>(value)));
		
		}else if(value<0 || value == NULL) {
			
			value = 0;
			rgb255TextField[id]->ChangeValue(wxString::Format(wxT("%d"), static_cast<int>(value)));
		}

		rgb1TextField[id]->ChangeValue(wxString::Format(wxT("%f"), rgb255To1(value)));

		hexTextField->ChangeValue(wxString::Format(wxT("%s%s%s"),
			intToHex(wxAtoi(rgb255TextField[0]->GetValue())),
			intToHex(wxAtoi(rgb255TextField[1]->GetValue())),
			intToHex(wxAtoi(rgb255TextField[2]->GetValue()))));

		rgb255ToCmyk(wxAtoi(rgb255TextField[0]->GetValue()), wxAtoi(rgb255TextField[1]->GetValue()), wxAtoi(rgb255TextField[2]->GetValue()));

		
	}else if(id<3) {	//id keep being the same. rgb1
				
		rgb1TextField[id]->GetValue().ToDouble(&value);
		

		if (value > 1.0) {
			value = 1.0;
			rgb1TextField[id]->ChangeValue(wxString::Format(wxT("%f"), value));
		}
		else if (value < 0.0) {
			value = 0.0;
			rgb1TextField[id]->ChangeValue(wxString::Format(wxT("%f"), value));
		}

		rgb255TextField[id]->ChangeValue(wxString::Format(wxT("%d"), rgb1To255(value)));

		
			hexTextField->ChangeValue(wxString::Format(wxT("%s%s%s"),
				intToHex(wxAtoi(rgb255TextField[0]->GetValue())),
				intToHex(wxAtoi(rgb255TextField[1]->GetValue())),
				intToHex(wxAtoi(rgb255TextField[2]->GetValue()))));

			rgb255ToCmyk(wxAtoi(rgb255TextField[0]->GetValue()), wxAtoi(rgb255TextField[1]->GetValue()), wxAtoi(rgb255TextField[2]->GetValue()));
		
			

	}else if (id < 7) {				//subtract 3 fom id. cmyk value
		double cmykVal[4];

		for (int i = 0; i < 4;i++) {
			cmykTextField[i]->GetValue().ToDouble(&value);
			if (value > 100) {
				value = 100;
				cmykTextField[i]->ChangeValue(wxString::Format(wxT("%f"), value));
			}
			else if (value < 0) {
				value = 0;
				cmykTextField[i]->ChangeValue(wxString::Format(wxT("%f"), value));
			}
			cmykVal[i] = value;		
		}

		cmykToRgb255(cmykVal[0], cmykVal[1], cmykVal[2], cmykVal[3]);
		
	}else {								//hex
		std::string complete = hexTextField->GetValue().ToStdString();
		std::string split[3];

		if (complete.length() < 3) { event.Skip(); return; }

		if (complete.length() == 3) {
			std::string temp="";
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 2; j++)
					temp.append(complete, i, 1);
			}
			complete = temp;
		}

		
		for (int i = 0; i < 3; i++) {
			split[i].append(complete, i * 2.0, 2);													//split the hex string into 3 groups of 3;

			std::stringstream ss;																	//string buffer don't ask me
			ss << std::hex << split[i];																//converting to "true" hex
			unsigned temp;																			//int with only positive values(binary duh)
			ss >> temp;																				//assing the value?
			std::bitset<8> binary(temp);

			rgb255TextField[i]->ChangeValue(wxString::Format(wxT("%d"),static_cast<int>(binary.to_ulong())));
			rgb1TextField[i]->ChangeValue(wxString::Format(wxT("%f"), rgb255To1(static_cast<int>(binary.to_ulong()))));	
		}

		rgb255ToCmyk(wxAtoi(rgb255TextField[0]->GetValue()), wxAtoi(rgb255TextField[1]->GetValue()), wxAtoi(rgb255TextField[2]->GetValue()));

	}

	panel->SetBackgroundColour(wxColour(wxAtoi(rgb255TextField[0]->GetValue()), wxAtoi(rgb255TextField[1]->GetValue()), wxAtoi(rgb255TextField[2]->GetValue())));
	panel->Refresh();
	event.Skip();

}

wxArrayString cMain::generateList(std::string text) {
	wxArrayString list;

	wxString valid_chars(wxString::Format(wxT("%s"), text));
	size_t len = valid_chars.Length();
	for (size_t i = 0; i < len; i++) { list.Add(wxString(valid_chars.GetChar(i))); }

	return list;
}

double cMain::rgb255To1(int value) {
	return static_cast<double>(value / 255.0);
}

int cMain::rgb1To255(double value) {
	return static_cast<int>(value * 255);
}

std::string cMain::intToHex(int value) {
	std::stringstream hex;
	hex << std::hex << std::setw(2) << std::setfill('0') << value;
	return hex.str();
}

void cMain::rgb255ToCmyk(int r,int g, int b) {


	double R = r / 255.0;
	double G = g / 255.0;
	double B = b / 255.0;
	
	double black = 1.0 - std::max(std::max(R,G),B);

	double cyan = (1.0 - R - black) / (1.0 - black);
	double magenta = (1.0 - G - black) / (1.0 - black);
	double yellow = (1.0 - B - black) / (1.0 - black);

	
	

	
	cmykTextField[0]->ChangeValue(wxString::Format(wxT("%.2f"), 100*cyan));
	cmykTextField[1]->ChangeValue(wxString::Format(wxT("%.2f"),  100*magenta));
	cmykTextField[2]->ChangeValue(wxString::Format(wxT("%.2f"),  100*yellow));
	cmykTextField[3]->ChangeValue(wxString::Format(wxT("%.2f"),  100*black));
}

void cMain::cmykToRgb255(double c, double m, double y, double k) {

	int red = 255 * (1.0 - c/100.0) * (1.0 - k/100.0);
	int green = 255 * (1.0 - m/100.0) *(1.0 - k/100.0);
	int blue = 255 * (1.0 - y/100.0) * (1.0 - k/100.0);

	rgb255TextField[0]->ChangeValue(wxString::Format(wxT("%d"), red));
	rgb255TextField[1]->ChangeValue(wxString::Format(wxT("%d"), green));
	rgb255TextField[2]->ChangeValue(wxString::Format(wxT("%d"), blue));

	rgb1TextField[0]->ChangeValue(wxString::Format(wxT("%f"), rgb255To1(red)));;
	rgb1TextField[1]->ChangeValue(wxString::Format(wxT("%f"), rgb255To1(green)));;
	rgb1TextField[2]->ChangeValue(wxString::Format(wxT("%f"), rgb255To1(blue)));;

	hexTextField->ChangeValue(wxString::Format(wxT("%s%s%s"), intToHex(red), intToHex(green), intToHex(blue)));

}




void cMain::initializeStaticTexts() {

	hexStatic = new wxStaticText(this, wxID_ANY, "HEX", wxPoint(defaultXPosition - (2.5 * rgbSizeX) - offset, 250), wxSize(22, 20), 0, "");
	hexStatic->SetForegroundColour(wxColour(200, 200, 200));
	hexStatic->GetFont().SetPointSize(18);

	rStaticText = new wxStaticText(this,wxID_ANY,"R",wxPoint(defaultXPosition - (2.3 * rgbSizeX),300),wxSize(10,20));
	rStaticText->SetForegroundColour(wxColour(200, 200, 200));
	rStaticText->GetFont().SetPointSize(18);

	gStaticText = new wxStaticText(this, wxID_ANY, "G", wxPoint(defaultXPosition - (2.3 * rgbSizeX), 300+rgbSizeY+5), wxSize(10, 20));
	gStaticText->SetForegroundColour(wxColour(200, 200, 200));
	gStaticText->GetFont().SetPointSize(18);

	bStaticText = new wxStaticText(this, wxID_ANY, "B", wxPoint(defaultXPosition - (2.3 * rgbSizeX), 300 + (2* rgbSizeY) + 10), wxSize(10, 20));
	bStaticText->SetForegroundColour(wxColour(200, 200, 200));
	bStaticText->GetFont().SetPointSize(18);


	rgb255Static = new wxStaticText(this, wxID_ANY, "255", wxPoint(defaultXPosition - (1.8 * rgbSizeX), 300 -  rgbSizeY), wxSize(22, 20));
	rgb255Static->SetForegroundColour(wxColour(200, 200, 200));
	rgb255Static->GetFont().SetPointSize(18);

	rgb1Static = new wxStaticText(this, wxID_ANY, "1", wxPoint(defaultXPosition - rgbSizeX/2 - 5, 300 - rgbSizeY), wxSize(10, 20));
	rgb1Static->SetForegroundColour(wxColour(200, 200, 200));
	rgb1Static->GetFont().SetPointSize(18);

	cStaticText = new wxStaticText(this, wxID_ANY, "C", wxPoint(defaultXPosition - (1.7 * rgbSizeX), 400 - rgbSizeY), wxSize(10, 20));
	cStaticText->SetForegroundColour(wxColour(200, 200, 200));
	cStaticText->GetFont().SetPointSize(18);

	mStaticText = new wxStaticText(this, wxID_ANY, "M", wxPoint(defaultXPosition - (rgbSizeX / 2), 400 - rgbSizeY), wxSize(10, 20));
	mStaticText->SetForegroundColour(wxColour(200, 200, 200));
	mStaticText->GetFont().SetPointSize(18);

	yStaticText = new wxStaticText(this, wxID_ANY, "Y", wxPoint(defaultXPosition - (1.7*rgbSizeX), 400 + 1.1 * rgbSizeY), wxSize(10, 15));
	yStaticText->SetForegroundColour(wxColour(200, 200, 200));
	yStaticText->GetFont().SetPointSize(18);

	kStaticText = new wxStaticText(this, wxID_ANY, "K", wxPoint(defaultXPosition - (rgbSizeX / 2), 400 + 1.1 * rgbSizeY), wxSize(10, 15));
	kStaticText->SetForegroundColour(wxColour(200, 200, 200));
	kStaticText->GetFont().SetPointSize(18);
}


