#include "Label.h";

Label::Label(short width, short height, string value, bool isVisible) :
	IControl(0, 0), width(width)
{
	if (value.size() > width) {
		labelText = "Input out of max range";
	}
	else {
		//cord = { width, hieght };
		labelText = value;
		//handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetCursorEnable(isVisible);
	}
}
Label::Label(int width, string value) :
	IControl(0, 0), width(width)
{
	if (value.size() > width) {
		labelText = "Input out of max range";
	}
	else {
		labelText = value;
		SetCursorEnable(false);
	}
	SetWidth(width);
	SetHeight(3);
}
void Label::SetCursorEnable(bool enable) {
	CONSOLE_CURSOR_INFO newCci = { 100, enable };
	SetConsoleCursorInfo(handle, &newCci);
}
void Label::SetColor(DWORD color) {
	dw = color;
}
void Label::SwitchContent(string newText) {
	labelText = newText;
	SetConsoleCursorPosition(handle, coord);
	Show();
}
COORD Label::GetCord() {
	return coord;
}
string Label::GetValue() {
	return labelText;
}
DWORD Label::GetColor() {
	return dw;
}
void Label::Hoover(bool isHover, DWORD newDw) {
	if (isHover) {
		dw = (BACKGROUND_BLUE | BACKGROUND_INTENSITY) | dw;
		SetColor(dw);
		Show();
		return;
	}
	else {
		dw = newDw;
		SetColor(dw);
		Show();
	}
	return;
}
void Label::Show() {
	SetConsoleCursorPosition(handle, coord);
	SetConsoleTextAttribute(handle, dw);
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
		cout << labelText;
		return;
	}
	short length = (short)Length();
	for (short i = coord.X; i < coord.X + length + 2; i++) {
		COORD tmpCoord = { i,coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		SetConsoleTextAttribute(handle, dw);
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
	cout << labelText;
	return;
}

void Label::Print() const {
	SetConsoleCursorPosition(handle, coord);
	SetConsoleTextAttribute(handle, dw);
	cout << labelText;
}

int Label::Length() const {
	int i = 0;
	while (true) {
		if (labelText[i] == '\0')break;
		i++;
	}
	return i;
}
void Label::CleanLabel() {
	int i = 0;
	bool flage = true;
	while (flage)
	{
		if (labelText[i] == '\0') break;
		labelText[i] = ' ';
		i++;
	}
	Show();
}
Label::~Label() {

}

void Label::SetCoord(COORD newCoord) {
	coord = newCoord;
}

void Label::SetForeground(ForegroundColor color) {

	switch (color)
	{
	case ForegroundColor::Red:
		dw = dw | (FOREGROUND_RED);
		break;
	case ForegroundColor::Blue:
		dw = dw | (FOREGROUND_BLUE);
		break;
	case ForegroundColor::Green:
		dw = dw | (FOREGROUND_GREEN);
		break;
	case ForegroundColor::Purple:
		dw = dw | (FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Teal:
		dw = dw | (FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ForegroundColor::Yellow:
		dw = dw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ForegroundColor::White:
		dw = dw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Black:
		dw = dw | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		break;
	}
}
void Label::SetBackground(BackgroundColor color) {
	switch (color)
	{
	case BackgroundColor::Red:
		dw = dw | (BACKGROUND_RED);
		break;
	case BackgroundColor::Blue:
		dw = dw | (BACKGROUND_BLUE);
		break;
	case BackgroundColor::Green:
		dw = dw | (BACKGROUND_GREEN);
		break;
	case BackgroundColor::Purple:
		dw = dw | (BACKGROUND_RED | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Teal:
		dw = dw | (BACKGROUND_RED | BACKGROUND_GREEN);
		break;
	case BackgroundColor::Yellow:
		dw = dw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case BackgroundColor::White:
		dw = dw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Black:
		dw = dw | !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		break;
	}
}

void Label::Hide() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(handle, blackDw);
	COORD crd = { coord.X , coord.Y };
	SetConsoleCursorPosition(handle, crd);
	short length = (short)Length();
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
void Label::SetBorder(BorderType _border) {
	border = _border;
}

void Label::SetValue(string value) {
	labelText = value;
}

int Label::GetWidth() {
	return width;
}