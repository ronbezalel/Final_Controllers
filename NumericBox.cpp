#include "NumericBox.h"



NumericBox::NumericBox(int width, int min, int max) :
	IControl(0, 0), minValue(min), maxValue(max), width(width)
{
	BoxValue = 0;
	SetWidth(width);
	SetHeight(3);
}


NumericBox::~NumericBox()
{

}

void NumericBox::SetValue(int value) {
	if (value < minValue || value > maxValue) {
		return;
	}

	BoxValue = value;
}


void NumericBox::Show() {
	SetConsoleCursorPosition(handle, coord);
	SetConsoleTextAttribute(handle, numericDW);
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
	else
	{
		for (int i = coord.X; i < coord.X + (short)width; i++) {
			cout << ' ';
		}
		COORD tmpCoord = { coord.X,coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << "+ ";
		tmpCoord = { coord.X + (((short)width / 2) - 1),coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << BoxValue;
		tmpCoord = { coord.X + (short)width - 2 ,coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << " -";
		CONSOLE_CURSOR_INFO newCci = { 100, false };
		SetConsoleCursorInfo(handle, &newCci);
		return;
	}
	short length = (short)width;
	for (short i = coord.X; i < coord.X + length + 2; i++) {
		COORD tmpCoord = { i,coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		SetConsoleTextAttribute(handle, numericDW);
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
		if (i == coord.X + length + 1) {
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
	COORD endPrint = { coord.X + 1 , coord.Y + 1 };
	SetConsoleCursorPosition(handle, endPrint);
	for (int i = coord.X + 1; i < coord.X + (short)width; i++) {
		cout << ' ';
	}
	COORD tmpCoord = { coord.X + 1,coord.Y + 1 };
	SetConsoleCursorPosition(handle, tmpCoord);
	cout << "+ ";
	tmpCoord = { coord.X + ((short)width / 2),coord.Y + 1 };
	SetConsoleCursorPosition(handle, tmpCoord);
	cout << BoxValue;
	tmpCoord = { coord.X + (short)width - 1,coord.Y + 1 };
	SetConsoleCursorPosition(handle, tmpCoord);
	cout << " -";
	CONSOLE_CURSOR_INFO newCci = { 100, false };
	SetConsoleCursorInfo(handle, &newCci);
	return;

}
void NumericBox::Hide() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(handle, blackDw);
	COORD crd = { coord.X , coord.Y };
	SetConsoleCursorPosition(handle, crd);
	short length = (short)width;
	if (border == BorderType::None) {
		for (short j = crd.X; j < (coord.X + length); j++) {
			COORD tmpCrd = { j , coord.Y };
			SetConsoleCursorPosition(handle, tmpCrd);
			cout << " ";
		}
	}
	else {
		for (short i = crd.Y; i < crd.Y + 3; i++) {
			for (short j = crd.X; j < (coord.X + length) + 2; j++) {
				COORD tmpCrd = { j , i };
				SetConsoleCursorPosition(handle, tmpCrd);
				cout << " ";
			}
		}
	}
}
int NumericBox::GetValue() {
	return BoxValue;
}
void NumericBox::SetCoordinates(short x, short y) {
	coord = { x,y };
}
void NumericBox::SetForeground(ForegroundColor color) {

	switch (color)
	{
	case ForegroundColor::Red:
		numericDW = numericDW | (FOREGROUND_RED);
		break;
	case ForegroundColor::Blue:
		numericDW = numericDW | (FOREGROUND_BLUE);
		break;
	case ForegroundColor::Green:
		numericDW = numericDW | (FOREGROUND_GREEN);
		break;
	case ForegroundColor::Purple:
		numericDW = numericDW | (FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Teal:
		numericDW = numericDW | (FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ForegroundColor::Yellow:
		numericDW = numericDW | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ForegroundColor::White:
		numericDW = numericDW | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Black:
		numericDW = numericDW | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		break;
	}
}
void NumericBox::SetBackground(BackgroundColor color) {
	switch (color)
	{
	case BackgroundColor::Red:
		numericDW = numericDW | (BACKGROUND_RED);
		break;
	case BackgroundColor::Blue:
		numericDW = numericDW | (BACKGROUND_BLUE);
		break;
	case BackgroundColor::Green:
		numericDW = numericDW | (BACKGROUND_GREEN);
		break;
	case BackgroundColor::Purple:
		numericDW = numericDW | (BACKGROUND_RED | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Teal:
		numericDW = numericDW | (BACKGROUND_RED | BACKGROUND_GREEN);
		break;
	case BackgroundColor::Yellow:
		numericDW = numericDW | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case BackgroundColor::White:
		numericDW = numericDW | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Black:
		numericDW = numericDW | !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		break;
	}
}
void NumericBox::SetBorder(BorderType _border) {
	border = _border;
}

void NumericBox::MouseEventProc(MOUSE_EVENT_RECORD mer) {
	if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
		COORD tmpCord = { coord.X + 2, coord.Y };
		if (border != BorderType::None) {
			tmpCord = { tmpCord.X + 1, tmpCord.Y + 1 };
		}
		int res = checkPosition(mer);
		if (res == 1) {
			if (BoxValue + 1 <= maxValue) {
				BoxValue++;
				SetConsoleCursorPosition(handle, coord);
				SetConsoleTextAttribute(handle, numericDW);
				Show();
			}
		}
		if (res == 0) {
			if (BoxValue - 1 >= minValue) {
				BoxValue--;
				SetConsoleCursorPosition(handle, coord);
				SetConsoleTextAttribute(handle, numericDW);
				Show();
			}
		}
		else {
			return;
		}
	}
}

int NumericBox::checkPosition(MOUSE_EVENT_RECORD mer) {
	COORD tmpCord = coord;
	if (border != BorderType::None) {
		tmpCord = { coord.X + 1, coord.Y + 1 };
	}
	if (mer.dwMousePosition.X == tmpCord.X && mer.dwMousePosition.Y == tmpCord.Y) {
		return 1;
	}
	if (mer.dwMousePosition.X == tmpCord.X + (short)width - 1 && mer.dwMousePosition.Y == tmpCord.Y) {
		return 0;
	}
	return -1;
}

void NumericBox::HandleInput(INPUT_RECORD iRecord) {
	//if (!isClicked) return;
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

void NumericBox::SetMin(int minimum) {
	minValue = minimum;
}
void NumericBox::SetMax(int maximum) {
	maxValue = maximum;
}