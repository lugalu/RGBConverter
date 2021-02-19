#include "Main.h"


int main() {
	//variables
	short int option;

	//check for files

	//code
	do {
		do {
			cout << "1- RGB 255 to 1\n2- RGB 1 to 255\n3- Hex to RGB\n4- exit\nType your option: ";
			cin >> option;
			//cout << string(50, '\n');
			cout << "\033[2J\033[1;1H";
		} while (option > 4 || option < 1);

		switch (option) {
		case 1:
			manager255To1();
			break;
		case 2:
			manager1To255();
			break;
		case 3:
			managerHex();
			break;
		case 4:
			cout << "Closing..." << endl;
			break;
		}
		cout << "\033[2J\033[1;1H";
	} while (option != 4);

	return 0;
}


void manager255To1() {
	double rgbIn[3], rgbOut[3];
	int i;
	
	for (i = 0; i < 3; i++) {
		rgbIn[i] = static_cast<int>(getValues(i, 255));
		rgbOut[i] = rgb255To1(rgbIn[i]);
	}

	cout.precision(4);
	cout << "RGB 255: " << rgbIn[0] << "," << rgbIn[1] << "," << rgbIn[2] << endl;
	cout << "RGB 1: " << rgbOut[0] << "," << rgbOut[1] << "," << rgbOut[2] << endl;
	cout << "press any key to continue" << endl;
	
}

void manager1To255() {
	double rgbIn[3], rgbOut[3];
	int i;

	for (i = 0; i < 3; i++) {
		rgbIn[i] = getValues(i, 1);
		rgbOut[i] =static_cast<int>(rgbIn[i]  * 255);
	}

	cout.precision(4);
	cout << "RGB 1: " << rgbIn[0] << "," << rgbIn[1] << "," << rgbIn[2] << endl;
	cout << "RGB 255: " << rgbOut[0] << "," << rgbOut[1] << "," << rgbOut[2] << endl;
	cout << "press any key to continue" << endl;
}

void managerHex() {
	string hexValue,splitHex[3];
	double rgb1[3];
	int rgb255[3];

	hexValue = getHex();

	for (int i = 0; i < 3; i ++) {
		splitHex[i].append(hexValue,i*2.0,2);													//split the hex string into 3 groups of 3;
		
		stringstream ss;																		//string buffer don't ask me
		ss << hex << splitHex[i];																//converting to "true" hex
		unsigned temp;																			//int with only positive values(binary duh)
		ss >> temp;																				//assing the value?
		bitset<8> binary(temp);																	//finally converts to binary;
		
		rgb255[i] = static_cast<int>(binary.to_ulong());
		rgb1[i] = rgb255To1(rgb255[i]);
	}

	cout.precision(4);
	cout << "translated hex input: " << hexValue << endl;
	cout << "RGB 1: " << rgb1[0] << "," << rgb1[1] << "," << rgb1[2] << endl;
	cout << "RGB 255: " << rgb255[0] << "," << rgb255[1] << "," << rgb255[2] << endl;
	cin >> cinBlocker;

}

string getHex() {
	string hex;

	do {
		cout << "type the hex value: "<<endl;
		cin >> hex;
	} while (hex.size()!=3 && hex.size()!=6);

	if (hex.size() == 3) {
		string temp;
		
		for (int i = 0; i < 3; i++) {
			for(int j = 0; j<2; j++)
			temp.append(hex,i,1);
		}
		hex = temp;
	
	}

	return hex;
}



/*
*	generic function to get rgb values	
*	@param 
*	index - index for the displayed color message.
*	end - determine the max value of the range (255 or 0)
*	@return 
*	returns the user value;
*/
double getValues(int index,int end) {
	string msg[] = { "Red","Green","Blue" };													//Easy to change
	double value;

	do {
		cout << "Range: 0-"<<end << endl;
		cout << "Type the "<<msg[index]<<" value" << endl;
		cin >> value;
	} while (value < 0 || value>end);															//Simple Checker
	
	cout << "\033[2J\033[1;1H";
	
	return value;
}

double rgb255To1(int value) {
	return static_cast<double>(value / 255);
}

