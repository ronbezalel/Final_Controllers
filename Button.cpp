#include "Button.h"



Button::Button(int width) : IControl(0, 0)
{
	btnWidth = width;
	SetWidth(width);
	SetHeight(3);
	CONSOLE_CURSOR_INFO cci = { 100, FALSE };
	SetConsoleCursorInfo(handle, &cci);
}


Button::~Button()
{
}

void Button::Show() {
	SetConsoleCursorPosition(handle, coord);
	string toCompare = "+";
	COORD tmpCoord = { coord.X,coord.Y };
	SetConsoleTextAttribute(handle, generalDw);
	char luCorner, ruCorner, ldCorner, rdCorner, vertical, horizontal;
	if (border == BorderType::Single) {
		luCorner = '\xDA';
		horizontal = '\xC4';
		ruCorner = '\xBF';
		vertical = '\xB3';
		rdCorner = '\xD9';
		ldCorner = '\xC0';
	}
	else if (border == BorderType::Double) {
		luCorner = '\xC9';
		horizontal = '\xCD';
		ruCorner = '\xBB';
		vertical = '\xBA';
		rdCorner = '\xBC';
		ldCorner = '\xC8';
	}
	else if (border == BorderType::None) {
		luCorner = '\0';
		horizontal = '\0';
		ruCorner = '\0';
		vertical = '\0';
		rdCorner = '\0';
		ldCorner = '\0';
	}

	for (short i = coord.X; i < coord.X + btnWidth + 2; i++) {
		COORD tmpCoord = { i,coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		DWORD tmpDw = border != BorderType::None ? generalDw : blackDw;
		SetConsoleTextAttribute(handle, tmpDw);

		if (i == coord.X) {
			tmpCoord.Y = tmpCoord.Y;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << luCorner;
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << vertical;
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << ldCorner;
			continue;
		}
		if (i == coord.X + btnWidth + 1) {
			tmpCoord.Y = tmpCoord.Y;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << ruCorner;
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << vertical;
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << rdCorner;
			continue;
		}
		cout << horizontal;
		tmpCoord.Y = tmpCoord.Y + 2;
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << horizontal;
	}
	tmpCoord.Y = (coord.Y + 1);
	tmpCoord.X = ((btnWidth / 2) + coord.X + (buttonText.size() / 2) - 1);
	SetConsoleCursorPosition(handle, tmpCoord);
	SetConsoleTextAttribute(handle, generalDw);
	cout << buttonText;

	COORD endPrint = { coord.X + 1 , coord.Y + 1 };
	SetConsoleCursorPosition(handle, endPrint);
	//cout << buttonText;
	return;
}

void Button::Hide() {
	DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(handle, blackDw);
	COORD crd = { coord.X , coord.Y };
	SetConsoleCursorPosition(handle, crd);
	short length = (short)btnWidth;
	for (short i = crd.Y; i < crd.Y + 3; i++) {
		for (short j = crd.X; j < (coord.X + length) + 2; j++) {
			COORD tmpCrd = { j , i };
			SetConsoleCursorPosition(handle, tmpCrd);
			cout << " ";
		}
	}
}

string Button::GetValue() {
	return "";
}

void Button::SetCoordinates(short x, short y) {
	coord = { x,y };
}

void Button::SetForeground(ForegroundColor color) {
	switch (color)
	{
	case ForegroundColor::Red:
		generalDw = generalDw | (FOREGROUND_RED);
		break;
	case ForegroundColor::Blue:
		generalDw = generalDw | (FOREGROUND_BLUE);
		break;
	case ForegroundColor::Green:
		generalDw = generalDw | (FOREGROUND_GREEN);
		break;
	case ForegroundColor::Purple:
		generalDw = generalDw | (FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Teal:
		generalDw = generalDw | (FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ForegroundColor::Yellow:
		generalDw = generalDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ForegroundColor::White:
		generalDw = generalDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Black:
		generalDw = generalDw | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		break;
	}
}

void Button::SetBackground(BackgroundColor color) {
	switch (color)
	{
	case BackgroundColor::Red:
		generalDw = generalDw | (BACKGROUND_RED);
		break;
	case BackgroundColor::Blue:
		generalDw = generalDw | (BACKGROUND_BLUE);
		break;
	case BackgroundColor::Green:
		generalDw = generalDw | (BACKGROUND_GREEN);
		break;
	case BackgroundColor::Purple:
		generalDw = generalDw | (BACKGROUND_RED | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Teal:
		generalDw = generalDw | (BACKGROUND_RED | BACKGROUND_GREEN);
		break;
	case BackgroundColor::Yellow:
		generalDw = generalDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case BackgroundColor::White:
		generalDw = generalDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Black:
		generalDw = generalDw | !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		break;
	}
}

void Button::SetBorder(BorderType _border) {
	border = _border;
}

void Button::AddListener(MouseListener* listener) {
	listen.push_back(listener);
}

void Button::HandleInput(INPUT_RECORD iRecord) {
	//if (!isClicked) return;
	SetConsoleTextAttribute(handle, generalDw);
	switch (iRecord.EventType)
	{
	case MOUSE_EVENT: // mouse input 
		MouseEventProc(iRecord.Event.MouseEvent);
		break;

	default:
		//errorInput();
		break;
	}
}

void Button::MouseEventProc(MOUSE_EVENT_RECORD mer) {
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	switch (mer.dwEventFlags) {

	case 0:
		//Right button press
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			MousePressed(mer);
		}
		break;
	case MOUSE_MOVED:
		MouseMoved(mer);
		break;
	}
}

void Button::MousePressed(MOUSE_EVENT_RECORD mer) {
	int res = CheckPosition(mer);
	if (res != -1) {
		for (int i = 0; i < listen.size(); i++) {
			int x = coord.X + btnWidth / 2;
			int y = coord.Y / 2;
			listen[i]->MousePressed(*this, coord.X + 1, coord.Y + 3 + i, true);
		}

	}
}

void Button::MouseMoved(MOUSE_EVENT_RECORD mer) {
	int res = CheckPosition(mer);
	if (res != -1) {
		//before Refactoring  
	}

}

int Button::CheckPosition(MOUSE_EVENT_RECORD mer) {
	int YAxis = mer.dwMousePosition.Y;
	int XAxis = mer.dwMousePosition.X;

	if ((XAxis >= coord.X && XAxis <= coord.X + btnWidth) && (YAxis >= coord.Y && YAxis <= coord.Y + 1)) {
		return YAxis;
	}
	return -1;
}

void Button::SetCursorEnable(bool enable) {
	CONSOLE_CURSOR_INFO newCci = { 0, enable };
	SetConsoleCursorInfo(handle, &newCci);
}

void Button::SetValue(string value) {
	int x = value.size();
	if (x > btnWidth) {
		buttonText = "+";
	}
	else {
		buttonText = value;
	}
}