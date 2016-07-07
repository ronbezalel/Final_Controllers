#include "Panel.h"

Panel::Panel(int width, int height) : IControl(0, 0), panelHeight(height), panelWidth(width), currentController(0) {
	border = BorderType::None;
	SetWidth(width);
	SetHeight(height);
}
Panel::~Panel() {

}
void Panel::AddControl(IControl* control, int left, int top) {
	if (coord.X + panelWidth > coord.X + left && coord.Y + panelHeight > coord.Y + top) {
		if (border != BorderType::None) {
			control->SetCoordinates(coord.X + 1 + (short)left, coord.Y + 1 + (short)top);
		}
		else {
			control->SetCoordinates(coord.X + (short)left, coord.Y + (short)top);
		}
		controllers.push_back(control);
		ControllerSureface tempCs = ControllerSureface();
		tempCs.xstart = control->GetCoord().X;
		tempCs.ystart = control->GetCoord().Y;
		tempCs.xend = control->GetCoord().X + control->GetWidth();
		tempCs.yend = control->GetCoord().Y + control->GetWidth();
		cs.push_back(tempCs);
		return;
	}
}

void Panel::Show() {
	SetConsoleCursorPosition(handle, coord);
	COORD tmpCoord = { coord.X,coord.Y };
	SetConsoleTextAttribute(handle, panelDw);
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
		luCorner = '\0';
		horizontal = '\0';
		ruCorner = '\0';
		vertical = '\0';
		rdCorner = '\0';
		ldCorner = '\0';
	}
	short length = (short)panelWidth;
	for (short j = coord.Y; j < coord.Y + panelHeight + 2; j++) {
		for (short i = coord.X; i < coord.X + length + 2; i++) {
			COORD tmpCoord = { i,j };
			SetConsoleCursorPosition(handle, tmpCoord);
			SetConsoleTextAttribute(handle, panelDw);
			if (i == coord.X) {
				if (j == coord.Y) {
					cout << luCorner;
				}
				else if (j == coord.Y + panelHeight + 1) {
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
				else if (j == coord.Y + panelHeight + 1) {
					cout << rdCorner;
				}
				else
				{
					cout << vertical;
				}
				continue;
			}
			else {
				if (j == coord.Y || j == coord.Y + panelHeight + 1) cout << horizontal;
				else cout << ' ';
			}
		}
	}
	COORD endPrint = { coord.X + 1 , coord.Y + 1 };
	SetConsoleCursorPosition(handle, endPrint);
	for (int i = 0; i < controllers.size(); i++) {
		controllers[i]->Show();
	}
}

void Panel::Hide() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD blackDw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	SetConsoleTextAttribute(handle, blackDw);
	short length = (short)panelWidth;
	COORD crd = { coord.X - 1 , coord.Y - 1 };
	SetConsoleCursorPosition(handle, crd);
	for (short j = crd.Y; j < crd.Y + panelHeight + 3; j++) {
		for (short i = crd.X; i < crd.X + length + 3; i++) {
			COORD tmpCrd = { i , j };
			SetConsoleCursorPosition(handle, tmpCrd);
			cout << " ";
		}
	}
}

void Panel::SetCoordinates(short x, short y) {
	coord = { x,y };
}
void Panel::SetBorder(BorderType _border) {
	border = _border;
}

void Panel::SetForeground(ForegroundColor color) {
	switch (color)
	{
	case ForegroundColor::Red:
		panelDw = panelDw | (FOREGROUND_RED);
		break;
	case ForegroundColor::Blue:
		panelDw = panelDw | (FOREGROUND_BLUE);
		break;
	case ForegroundColor::Green:
		panelDw = panelDw | (FOREGROUND_GREEN);
		break;
	case ForegroundColor::Purple:
		panelDw = panelDw | (FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Teal:
		panelDw = panelDw | (FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ForegroundColor::Yellow:
		panelDw = panelDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ForegroundColor::White:
		panelDw = panelDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Black:
		panelDw = panelDw | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		break;
	}
}

void Panel::SetBackground(BackgroundColor color) {
	switch (color)
	{
	case BackgroundColor::Red:
		panelDw = panelDw | (BACKGROUND_RED);
		break;
	case BackgroundColor::Blue:
		panelDw = panelDw | (BACKGROUND_BLUE);
		break;
	case BackgroundColor::Green:
		panelDw = panelDw | (BACKGROUND_GREEN);
		break;
	case BackgroundColor::Purple:
		panelDw = panelDw | (BACKGROUND_RED | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Teal:
		panelDw = panelDw | (BACKGROUND_RED | BACKGROUND_GREEN);
		break;
	case BackgroundColor::Yellow:
		panelDw = panelDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case BackgroundColor::White:
		panelDw = panelDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Black:
		panelDw = panelDw | !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		break;
	}
}

TabStatus Panel::Tab() {
	TabStatus res = controllers[currentController]->Tab();
	bool stop = false;
	switch (res)
	{
	case NEXT:
		currentController++;
		if (currentController == controllers.size()) {
			currentController = 0;
			return NEXT;
		}
		while (!stop) {
			res = controllers[currentController]->Tab();
			if (res == WAIT) {
				stop = true;
			}
			else if (res == STAY) {
				stop = true;
				currentController++;
				return currentController == controllers.size() ? NEXT : WAIT;
			}
			else currentController++;
			if (currentController == controllers.size()) {
				currentController = 0;
				return NEXT;
			}
		}
		break;
	case STAY:
		currentController++;
		return currentController == controllers.size() ? NEXT : WAIT;
		break;
	case WAIT:
		return WAIT;
		break;
	}
}

void Panel::HandleInput(INPUT_RECORD iRecord) {
	switch (iRecord.EventType)
	{
	case KEY_EVENT: // keyboard input 
		if (iRecord.Event.KeyEvent.wVirtualKeyCode == VK_TAB && iRecord.Event.KeyEvent.bKeyDown) {
			TabStatus res = Tab();
			if (res == NEXT) {
				currentController = 0;
			}
		}
		controllers[currentController]->HandleInput(iRecord);
		break;

	case MOUSE_EVENT: // mouse input 
		currentController = checkPositionController(iRecord.Event.MouseEvent);
		for (int i = 0; i < controllers.size(); i++) {
			controllers[i]->HandleInput(iRecord);
		}
		break;

	default:
		break;
	}
}

int Panel::checkPositionController(MOUSE_EVENT_RECORD mer) {
	short xIndex = mer.dwMousePosition.X;
	short yIndex = mer.dwMousePosition.Y;

	for (int i = 0; i < controllers.size(); i++) {
		if (xIndex >= cs[i].xstart && xIndex <= cs[i].xend && yIndex >= cs[i].ystart && yIndex <= cs[i].yend)
			return i;
	}
	return currentController;
}