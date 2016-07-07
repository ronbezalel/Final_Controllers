#include "TextBox.h"



TextBox::TextBox(int boxLength, short width, short height) :
	IControl(0, 0), textIndex(0), focus(false), enableWrite(false)
{
	boxSize = boxLength;
	text = vector<char>(boxLength);
	CONSOLE_CURSOR_INFO newCci = { 100, true };
	SetConsoleCursorInfo(handle, &newCci);
	SetBorder(BorderType::Single);
	for (int i = 0; i < width; i++) {
		text[i] = ' ';
	}
	SetWidth(width);
	SetHeight(height);
}

TextBox::TextBox(int width) :
	IControl(0, 0), textIndex(0), focus(false), enableWrite(false)
{
	boxSize = width;
	text = vector<char>(width);
	CONSOLE_CURSOR_INFO newCci = { 100, true };
	SetConsoleCursorInfo(handle, &newCci);
	SetBorder(BorderType::Single);
	for (int i = 0; i < width; i++) {
		text[i] = ' ';
	}
}

void TextBox::Print() {
	DWORD dw = textDw;
	for (short i = coord.X; i < coord.X + boxSize + 2; i++) {
		COORD tmpCoord = { i,coord.Y };
		SetConsoleCursorPosition(handle, tmpCoord);
		SetConsoleTextAttribute(handle, dw);
		if (i == coord.X) {
			tmpCoord.Y = tmpCoord.Y;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << '\xDA';
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << '\xB3';
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << '\xC0';
			continue;
		}
		if (i == coord.X + boxSize + 1) {
			tmpCoord.Y = tmpCoord.Y;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << '\xBF';
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << '\xB3';
			tmpCoord.Y = tmpCoord.Y + 1;
			SetConsoleCursorPosition(handle, tmpCoord);
			cout << '\xD9';
			continue;
		}
		cout << '\xC4';
		tmpCoord.Y = tmpCoord.Y + 2;
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << '\xC4';
	}
	curserPosition = coord.X;
	COORD endPrint = { curserPosition, coord.Y };
	SetConsoleCursorPosition(handle, endPrint);
}

TextBox::~TextBox()
{
}

void TextBox::HandleInput(INPUT_RECORD iRecord) {
	SetConsoleTextAttribute(handle, textDw);
	CONSOLE_CURSOR_INFO newCci = { 100, true };
	SetConsoleCursorInfo(handle, &newCci);
	if (!enableWrite) return;
	switch (iRecord.EventType)
	{
	case KEY_EVENT: // keyboard input 
		KeyEventProc(iRecord.Event.KeyEvent);
		break;

	case MOUSE_EVENT: // mouse input 
		MouseEventProc(iRecord.Event.MouseEvent);
		break;

	default:
		//errorInput();
		break;
	}
}

void TextBox::MouseEventProc(MOUSE_EVENT_RECORD mer) {
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
	}
}

void TextBox::MousePressed(MOUSE_EVENT_RECORD mer) {
	int res = CheckPosition(mer);
	if (res != -1) {
		res = LastLetterIndex(res);
		curserPosition = res;
		textIndex = res - coord.X - 1;
		COORD reset = { res , coord.Y + 1 };
		SetConsoleCursorPosition(handle, reset);
		focus = true;
	}
}

int TextBox::LastLetterIndex(int position) {
	int currentIndex = position - coord.X - 1;
	int lastLetter = 0;
	for (int i = 0; i < text.size(); i++) {
		if (text[i] != ' ') lastLetter = i;
	}
	if (currentIndex > lastLetter) {
		return coord.X + lastLetter + 2;
	}
	else {
		return position;
	}
}

int TextBox::CheckPosition(MOUSE_EVENT_RECORD mer) {
	int YAxis = mer.dwMousePosition.Y;
	int XAxis = mer.dwMousePosition.X;
	if (XAxis <= coord.X + boxSize && XAxis >= coord.X + 1 &&
		YAxis == coord.Y + 1) {
		return XAxis;
	}
	return -1;
}

void TextBox::KeyEventProc(KEY_EVENT_RECORD ker) {
	if (ker.bKeyDown) {
		switch (ker.wVirtualKeyCode)
		{
		case VK_LEFT:
		case VK_NUMPAD4:
			MoveLeft();
			break;
		case VK_NUMPAD6:
		case VK_RIGHT:
			MoveRight();
			break;
		case VK_BACK:
			BackSpace();
			break;
		case VK_DELETE:
			Delete();
			break;
		default:
			AddCharecter(ker.uChar.AsciiChar);
			break;
		}
	}
}
void TextBox::MoveLeft() {
	if (curserPosition > coord.X + 1) {
		curserPosition--;
		COORD left = { curserPosition , coord.Y + 1 };
		SetConsoleCursorPosition(handle, left);
		if (focus) {
			textIndex--;
		}
		else focus = true;
	}
}

void TextBox::MoveRight() {
	if (curserPosition < coord.X + boxSize + 1) {
		curserPosition++;
		COORD right = { curserPosition , coord.Y + 1 };
		SetConsoleCursorPosition(handle, right);
		if (focus) {
			textIndex++;
		}
		else {
			focus = true;
			textIndex++;
		}
	}
}

void TextBox::Delete() {
	if (focus) {
		int offset = 1;
		for (int i = textIndex; i < boxSize; i++) {
			if (i == boxSize - 1) {
				text[i] = ' ';
				cout << text[i];
				break;
			}
			text[i] = text[i + 1];
			cout << text[i];
			COORD moveRight = { curserPosition + offset++ , coord.Y + 1 };
			SetConsoleCursorPosition(handle, moveRight);
		}
		COORD reset = { curserPosition , coord.Y + 1 };
		SetConsoleCursorPosition(handle, reset);
	}
}

void TextBox::BackSpace() {
	if (focus) {
		if (textIndex == 0) {
			return;
		}
		for (int i = textIndex - 1; i < boxSize; i++) {
			if (i == boxSize - 1) {
				text[i] = ' ';
				cout << text[i];
				break;
			}
			text[i] = text[i + 1];
		}
		string tmpText(text.begin(), text.end());
		COORD reset = { coord.X + 1 , coord.Y + 1 };
		SetConsoleCursorPosition(handle, reset);
		cout << tmpText;
		COORD newPosition = { --curserPosition , coord.Y + 1 };
		SetConsoleCursorPosition(handle, newPosition);
		--textIndex;
	}
}

void TextBox::AddCharecter(char c) {
	if (focus) {
		if (textIndex < boxSize) {
			bool res = MiddleWrite();
			if (res) {
				if (text[boxSize - 1] == ' ') {
					vector<char> tmpState = text;
					for (int i = textIndex + 1; i < boxSize; i++) {
						text[i] = tmpState[i - 1];
						COORD reset = { coord.X + i + 1 , coord.Y + 1 };
						SetConsoleCursorPosition(handle, reset);
						cout << text[i];
					}
					COORD reset = { coord.X + textIndex + 1 , coord.Y + 1 };
					SetConsoleCursorPosition(handle, reset);
					text[textIndex] = c;
					cout << c;
					MoveRight();
					return;
				}
				else return;
			}

			text[textIndex] = c;
			cout << c;
			MoveRight();
		}
	}
}

bool TextBox::MiddleWrite() {
	bool left = false;
	bool right = false;
	for (int i = 0; i < textIndex; i++) {
		if (text[i] != ' ') {
			left = true;
			break;
		}
	}
	for (int i = textIndex; i < text.size(); i++) {
		if (text[i] != ' ') {
			right = true;
			break;
		}
	}
	return right && left;
}

void TextBox::SetText(string textToEnter) {
	if (textToEnter.size() < text.size()) {
		for (int i = 0; i < textToEnter.size(); i++) {
			text[i] = textToEnter[i];
		}
	}
}

string TextBox::GetValue() {
	string str(text.begin(), text.end());
	return str;
}

void TextBox::Show() {
	enableWrite = true;
	DWORD dw = textDw;
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
	else {
		for (int i = coord.X + 1; i < coord.X + boxSize + 1; i++) {
			COORD tmpCoord = { i, coord.Y + 1 };
			SetConsoleCursorPosition(handle, tmpCoord);
			SetConsoleTextAttribute(handle, dw);
			cout << text[i - (coord.X + 1)];
		}
		curserPosition = coord.X + 1;
		COORD endPrint = { curserPosition, coord.Y + 1 };
		SetConsoleCursorPosition(handle, endPrint);
		return;
	}

	for (short i = coord.X; i < coord.X + boxSize + 2; i++) {
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
		if (i == coord.X + boxSize + 1) {
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
		tmpCoord.Y = tmpCoord.Y + 1;
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << text[i - (coord.X + 1)];
		tmpCoord.Y = tmpCoord.Y + 1;
		SetConsoleCursorPosition(handle, tmpCoord);
		cout << horizontal;
	}
	curserPosition = coord.X + 1;
	COORD endPrint = { curserPosition, coord.Y + 1 };
	SetConsoleCursorPosition(handle, endPrint);
}

void TextBox::SetForeground(ForegroundColor color) {

	switch (color)
	{
	case ForegroundColor::Red:
		textDw = textDw | (FOREGROUND_RED);
		break;
	case ForegroundColor::Blue:
		textDw = textDw | (FOREGROUND_BLUE);
		break;
	case ForegroundColor::Green:
		textDw = textDw | (FOREGROUND_GREEN);
		break;
	case ForegroundColor::Purple:
		textDw = textDw | (FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Teal:
		textDw = textDw | (FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ForegroundColor::Yellow:
		textDw = textDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ForegroundColor::White:
		textDw = textDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Black:
		textDw = textDw | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		break;
	}
}
void TextBox::SetBackground(BackgroundColor color) {
	switch (color)
	{
	case BackgroundColor::Red:
		textDw = textDw | (BACKGROUND_RED);
		break;
	case BackgroundColor::Blue:
		textDw = textDw | (BACKGROUND_BLUE);
		break;
	case BackgroundColor::Green:
		textDw = textDw | (BACKGROUND_GREEN);
		break;
	case BackgroundColor::Purple:
		textDw = textDw | (BACKGROUND_RED | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Teal:
		textDw = textDw | (BACKGROUND_RED | BACKGROUND_GREEN);
		break;
	case BackgroundColor::Yellow:
		textDw = textDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case BackgroundColor::White:
		textDw = textDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Black:
		textDw = textDw | !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		break;
	}
}

void TextBox::SetBorder(BorderType _border) {
	border = _border;
	if (border == BorderType::None) {
		coord = { coord.X - 1, coord.Y - 1 };
	}
}

void TextBox::Hide() {
	CONSOLE_CURSOR_INFO newCci = { 100, false };
	SetConsoleCursorInfo(handle, &newCci);
	enableWrite = false;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(handle, blackDw);
	COORD crd = { coord.X , coord.Y };
	SetConsoleCursorPosition(handle, crd);
	short length = (short)boxSize;
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

TabStatus TextBox::Tab() {
	//COORD tmp = coord;
	//if (border != BorderType::None) {
	//	tmp.X = coord.X + 1;
	//	tmp.Y = coord.Y + 1;
	//}
	MOUSE_EVENT_RECORD mer;
	mer.dwMousePosition.X = coord.X + 1;
	mer.dwMousePosition.Y = coord.Y + 1;
	MousePressed(mer);
	return STAY;
}