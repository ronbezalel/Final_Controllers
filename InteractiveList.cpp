#include "InteractiveList.h"


#include "CheckList.h"



InteractiveList::InteractiveList(string* textList, int listSize, short width, short height, DWORD color) :
	IControl(0, 0), currentRow(0), currentPosition(0), rowMaxLength(0)
{
	int row = 0;
	coord = { 0 , 0 };
	currentPosition = 0;
	for (int i = 0; i < listSize; i++) {
		list.push_back(Label(width, row++, "[ ] " + textList[i], false));
		if (rowMaxLength < textList[i].size() + 4) rowMaxLength = textList[i].size() + 4;
		list[i].SetColor(color);
	}
	list[0].Hoover(true, color);
	chosen = new bool[listSize];
	for (int j = 0; j < listSize; j++) {
		chosen[j] = false;
	}
}

InteractiveList::InteractiveList(int height, int width, vector<string> options) :
	IControl(0, 0), currentRow(0), currentPosition(0), rowMaxLength(0), hoverEnable(false), maxRowNumber(height), firstShow(true)
{

	//generalDw = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	if (options.size() <= height) {
		int row = 0;
		currentPosition = 0;
		for (int i = 0; i < options.size(); i++) {
			Label l = Label(width, row++, "[ ] " + options[i], false);
			l.SetCoordinates(0, row);
			list.push_back(l);
			if (rowMaxLength < options[i].size() + 4) rowMaxLength = options[i].size() + 4;
			list[i].SetColor(generalDw);
		}
		//list[0].Hoover(true);
		chosen = new bool[height];
		for (int j = 0; j < height; j++) {
			chosen[j] = false;
		}
	}
	SetWidth(width);
	SetHeight(height);
}

void InteractiveList::Show() {
	if (!firstShow) coord = { coord.X - 1, coord.Y };
	else firstShow = false;
	SetConsoleCursorPosition(handle, coord);
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
	else
	{
		for (std::vector<Label>::const_iterator i = list.begin(); i != list.end(); i++) {
			i->Print();
		}
		SetConsoleCursorPosition(handle, coord);
		hoverEnable = true;
		return;
	}
	short length = (short)rowMaxLength;
	for (short j = coord.Y; j < coord.Y + list.size() + 2; j++) {
		for (short i = coord.X; i < coord.X + length + 2; i++) {
			COORD tmpCoord = { i,j };
			SetConsoleCursorPosition(handle, tmpCoord);
			SetConsoleTextAttribute(handle, generalDw);
			if (i == coord.X) {
				if (j == coord.Y) {
					cout << luCorner;
				}
				else if (j == coord.Y + list.size() + 1) {
					cout << ldCorner;
				}
				else
				{
					cout << vertical;
				}
				continue;
			}
			if (i == coord.X + length + 1) {
				if (j == coord.Y) {
					cout << ruCorner;
				}
				else if (j == coord.Y + list.size() + 1) {
					cout << rdCorner;
				}
				else
				{
					cout << vertical;
				}
				continue;
			}
			else {
				if (j == coord.Y || j == coord.Y + list.size() + 1) cout << horizontal;
				else cout << ' ';
			}
		}
	}
	coord = { coord.X + 1 , coord.Y + 1 };
	SetConsoleCursorPosition(handle, coord);
	for (short i = 0; i < list.size(); i++) {
		COORD newCoord = { coord.X , coord.Y + i };
		list[i].SetCoord(newCoord);
	}
	for (std::vector<Label>::const_iterator i = list.begin(); i != list.end(); i++) {
		i->Print();
	}
	SetConsoleCursorPosition(handle, coord);
	hoverEnable = true;
	return;
}
bool* InteractiveList::GetChosenRows() {
	return chosen;
}
void InteractiveList::HandleInput(INPUT_RECORD iRecord) {
	//if (!isClicked) return;
	SetConsoleTextAttribute(handle, generalDw);
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
void InteractiveList::MouseEventProc(MOUSE_EVENT_RECORD mer) {
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
void InteractiveList::MousePressed(MOUSE_EVENT_RECORD mer) {
	int res = CheckPosition(mer);
	if (res != -1) {
		if (hoverEnable) {
			list[currentRow].Hoover(false, generalDw);
		}
		currentRow = res - coord.Y;
		currentPosition = res;
		COORD newPosition = { coord.X , currentPosition };
		SetConsoleCursorPosition(handle, newPosition);
		if (hoverEnable) {
			list[currentRow].Hoover(true, generalDw);
		}
		Mark();
	}
}

void InteractiveList::MousePressedNoMark(MOUSE_EVENT_RECORD mer) {
	int res = CheckPosition(mer);
	if (res != -1) {
		if (hoverEnable) {
			list[currentRow].Hoover(false, generalDw);
		}
		currentRow = res - coord.Y;
		currentPosition = res;
		COORD newPosition = { coord.X , currentPosition };
		SetConsoleCursorPosition(handle, newPosition);
		if (hoverEnable) {
			list[currentRow].Hoover(true, generalDw);
		}
	}
}

void InteractiveList::MouseMoved(MOUSE_EVENT_RECORD mer) {
	int res = CheckPosition(mer);
	if (res != -1) {
		keyFocus = true;
		if (hoverEnable) {
			list[currentRow].Hoover(false, generalDw);
		}
		currentRow = res - coord.Y;
		currentPosition = res;
		COORD newPosition = { coord.X , currentPosition };
		SetConsoleCursorPosition(handle, newPosition);
		if (hoverEnable) {
			list[currentRow].Hoover(true, generalDw);
		}
	}
	else {
		keyFocus = false;
		return;
	}
}
int InteractiveList::CheckPosition(MOUSE_EVENT_RECORD mer) {
	int YAxis = mer.dwMousePosition.Y;
	int XAxis = mer.dwMousePosition.X;
	if (XAxis <= coord.X + rowMaxLength && XAxis >= coord.X &&
		YAxis < coord.Y + list.size() && YAxis >= coord.Y) {
		return YAxis;
	}
	return -1;
}
void InteractiveList::KeyEventProc(KEY_EVENT_RECORD ker) {
	if (ker.bKeyDown) {
		switch (ker.wVirtualKeyCode)
		{
		case VK_UP:
		case VK_NUMPAD8:
			MoveUp();
			break;
		case VK_NUMPAD2:
		case VK_DOWN:
			MoveDown();
			break;
		case VK_RETURN:
		case VK_SPACE:
			Mark();
			break;
		default:
			break;
		}
	}
}
void InteractiveList::MoveUp() {
	if (keyFocus) {
		if (currentPosition > coord.Y) {
			if (hoverEnable) {
				list[currentRow].Hoover(false, generalDw);
			}
			currentRow--;
			currentPosition--;
			COORD newPosition = { coord.X , currentPosition };
			SetConsoleCursorPosition(handle, newPosition);
			if (hoverEnable) {
				list[currentRow].Hoover(true, generalDw);
			}
		}
		else {
			if (hoverEnable) {
				list[currentRow].Hoover(false, generalDw);
			}
			currentRow += list.size() - 1;
			currentPosition += list.size() - 1;
			COORD newPosition = { coord.X , currentPosition };
			SetConsoleCursorPosition(handle, newPosition);
			if (hoverEnable) {
				list[currentRow].Hoover(true, generalDw);
			}
		}
	}
}
void InteractiveList::MoveDown() {
	if (keyFocus) {
		if (currentPosition < coord.Y + list.size() - 1) {
			if (hoverEnable) {
				list[currentRow].Hoover(false, generalDw);
			}
			list[currentRow].Hoover(false, generalDw);
			currentRow++;
			currentPosition++;
			COORD newPosition = { coord.X , currentPosition };
			SetConsoleCursorPosition(handle, newPosition);
			if (hoverEnable) {
				list[currentRow].Hoover(true, generalDw);
			}
		}
		else {
			if (hoverEnable) {
				list[currentRow].Hoover(false, generalDw);
			}
			currentRow -= list.size() - 1;
			currentPosition -= list.size() - 1;
			COORD newPosition = { coord.X , currentPosition };
			SetConsoleCursorPosition(handle, newPosition);
			if (hoverEnable) {
				list[currentRow].Hoover(true, generalDw);
			}
		}
	}
}
void InteractiveList::Mark() {
	if (chosen[currentRow]) {
		string newText = list[currentRow].GetValue();
		newText[1] = ' ';
		int width = list[currentRow].GetWidth();
		COORD newCoord = list[currentRow].GetCord();
		DWORD color = list[currentRow].GetColor();
		list[currentRow] = Label(width, 0, newText, false);
		list[currentRow].SetCoordinates(newCoord.X, newCoord.Y);
		list[currentRow].SetColor(color);
		chosen[currentRow] = false;
	}
	else {
		int oldChoice = -1;
		for (int i = 0; i < list.size(); i++) {
			if (chosen[i]) {
				oldChoice = i;
				chosen[i] = false;
				break;
			}
		}
		if (oldChoice != -1) {
			string unmark = list[oldChoice].GetValue();
			unmark[1] = ' ';
			int width = list[oldChoice].GetWidth();
			COORD unmarkCoord = list[oldChoice].GetCord();
			DWORD unmarkcolor = list[oldChoice].GetColor();
			list[oldChoice] = Label(width, 0, unmark, false);
			list[oldChoice].SetCoordinates(unmarkCoord.X, unmarkCoord.Y);
			list[oldChoice].SetColor(unmarkcolor);
			list[oldChoice].Print();
		}
		//new chosen raw
		string newText = list[currentRow].GetValue();
		newText[1] = 'X';
		int width = list[currentRow].GetWidth();
		COORD newCoord = list[currentRow].GetCord();
		DWORD color = list[currentRow].GetColor();
		list[currentRow] = Label(width, 0, newText, false);
		list[currentRow].SetCoordinates(newCoord.X, newCoord.Y);
		list[currentRow].SetColor(color);
		chosen[currentRow] = true;
	}
	list[currentRow].Print();
	return;
}

string InteractiveList::GetValue() {
	int index = 0;
	bool flage = false;
	for (index; index < list.size(); index++) {
		if (chosen[index]) {
			flage = true;
			break;
		}
	}
	if (!flage) return "no choose";
	return list[index].GetValue();
}

InteractiveList::~InteractiveList() {
	free(chosen);
}

void InteractiveList::SetForeground(ForegroundColor color)
{
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
	for (int i = 0; i < list.size(); i++) {
		list[i].SetColor(generalDw);
	}

}
void InteractiveList::SetBackground(BackgroundColor color)
{
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
	for (int i = 0; i < list.size(); i++) {
		list[i].SetColor(generalDw);
	}
}
void InteractiveList::SetBorder(BorderType _border) {
	border = _border;
}

void InteractiveList::Hide() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(handle, blackDw);
	short length = (short)rowMaxLength;
	if (border == BorderType::None) {
		COORD crd = coord;
		SetConsoleCursorPosition(handle, crd);
		for (short j = crd.Y; j < crd.Y + list.size(); j++) {
			for (short i = crd.X; i < crd.X + length; i++) {
				COORD tmpCrd = { i,j };
				SetConsoleCursorPosition(handle, tmpCrd);
				cout << " ";
			}
		}
	}
	else {
		COORD crd = { coord.X - 1 , coord.Y - 1 };
		SetConsoleCursorPosition(handle, crd);
		for (short j = crd.Y; j < crd.Y + list.size() + 2; j++) {
			for (short i = crd.X; i < crd.X + length + 2; i++) {
				COORD tmpCrd = { i , j };
				SetConsoleCursorPosition(handle, tmpCrd);
				cout << " ";
			}
		}
	}
	hoverEnable = false;
}

void InteractiveList::SetCoordinates(short x, short y) {
	coord = { x,y };
	for (short i = 0; i < list.size(); i++) {
		list[i].SetCoordinates(x, y + i);
	}
}

bool InteractiveList::AddSelectedItem(string item) {
	if (list.size() < maxRowNumber) {
		int width = list[0].GetWidth();
		Label l = Label(width, 0, "[ ] " + item, false);
		l.SetColor(generalDw);
		if (rowMaxLength < item.size() + 4) rowMaxLength = item.size() + 4;
		list.push_back(l);
		for (int i = 0; i < list.size(); i++) {
			list[i].SetCoordinates(coord.X, coord.Y + i);
		}
		return true;
	}
	return false;
}
bool InteractiveList::RemoveSelectedItem(string item) {
	for (int i = 0; i < list.size(); i++) {
		string s = list[i].GetValue();
		if (s == "[ ] " + item || s == "[X] " + item) {
			list.erase(list.begin() + i);
			return true;
		}
	}
	return false;
}

TabStatus InteractiveList::Tab() {
	if (keyFocus) {
		if (currentRow == list.size() - 1) {
			keyFocus = false;
			return NEXT;
		}
		MoveDown();
		return WAIT;
	}
	else {
		MOUSE_EVENT_RECORD mer;
		mer.dwMousePosition.X = list[0].GetCord().X;
		mer.dwMousePosition.Y = list[0].GetCord().Y;
		MousePressedNoMark(mer);
		MousePressedNoMark(mer);
		keyFocus = true;
		//if (hoverEnable) {
		//	list[0].Hoover(false, generalDw);
	}
	///
	//currentRow = 0;
	//SetConsoleCursorPosition(handle, coord);
	return list.size() == 1 ? STAY : WAIT;
}
