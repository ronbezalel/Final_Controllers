#include "ComboBox.h"



ComboBox::ComboBox(string* textList, int size, short width, short height, DWORD color) :
	IControl(0, 0)
{
	listSize = size;
	chosenText = new Label(width, height, "+ chose 1 option:", false);
	DWORD dw = BACKGROUND_BLUE && color;
	chosenText->SetColor(dw);
	optionsList = new RadioList(textList, listSize, width, height + 2, color);
}

ComboBox::ComboBox(int width, vector<string> options) :
	IControl(0, 0), firstShow(true)
{

	listSize = options.size();
	string defaultText = "+ ";
	int x = width > defaultText.size() ? width : defaultText.size();
	cbWidth = x;
	chosenText = new Label(x, defaultText);
	optionsList = new InteractiveList(listSize, width, options);
	optionsList->SetBorder(BorderType::None);
	optionsList->SetCoordinates(coord.X, coord.Y + 1);
	cbHeight = options.size() + 1;
}



void ComboBox::Show() {
	if (!firstShow) coord = { coord.X - 1, coord.Y };
	else firstShow = false;
	SetConsoleCursorPosition(handle, coord);
	SetConsoleTextAttribute(handle, chosenText->GetColor());
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
		chosenText->Show();
		return;
	}
	short length = (short)chosenText->GetWidth();
	for (short j = coord.Y; j < coord.Y + listSize + 3; j++) {
		for (short i = coord.X; i < coord.X + length + 2; i++) {
			COORD tmpCoord = { i,j };
			SetConsoleCursorPosition(handle, tmpCoord);
			SetConsoleTextAttribute(handle, chosenText->GetColor());
			if (i == coord.X) {
				if (j == coord.Y) {
					cout << luCorner;
				}
				else if (j == coord.Y + listSize + 2) {
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
				else if (j == coord.Y + listSize + 2) {
					cout << rdCorner;
				}
				else
				{
					cout << vertical;
				}
				continue;
			}
			else {
				if (j == coord.Y || j == coord.Y + listSize + 2) cout << horizontal;
				else cout << ' ';
			}
		}
	}
	SetCoordinates(coord.X + 1, coord.Y + 1);
	SetConsoleCursorPosition(handle, coord);
	chosenText->Print();
	SetConsoleCursorPosition(handle, coord);
}

void ComboBox::KeyEventProc(KEY_EVENT_RECORD ker) {
	if (displayList) {
		if (ker.wVirtualKeyCode == VK_ESCAPE) {
			CloseList();
			return;
		}
		string prevChose = optionsList->GetValue();
		optionsList->KeyEventProc(ker);
		string chose = optionsList->GetValue();
		if (prevChose != chose) {
			CloseList();
		}
		chosenText->CleanLabel();
		if (chose != "no choose") {
			chose[0] = '-';
			chose[1] = ' ';
			chose[2] = ' ';
		}
		else {
			chose = "- ";
		}
		chosenText->SwitchContent(chose);
	}
	else {
		if (ker.wVirtualKeyCode == VK_RETURN && displayList) {
			OpenList();
		}
	}

}
void ComboBox::MouseEventProc(MOUSE_EVENT_RECORD mer) {
	if (displayList) {
		if (checkPosition(mer) && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			CloseList();
			return;
		}
		string prevChose = optionsList->GetValue();
		optionsList->MouseEventProc(mer);
		string chose = optionsList->GetValue();
		if (prevChose != chose) {
			CloseList();
		}
		chosenText->CleanLabel();
		if (chose != "no choose") {
			chose[0] = '+';
			chose[1] = ' ';
			chose[2] = ' ';
		}
		else {
			chose = "+ ";
		}
		chosenText->SwitchContent(chose);
	}
	else {
		if (checkPosition(mer) && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			OpenList();
		}
	}
}

//void ComboBox::EraseList() {
//	optionsList->Hide();
//	//HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
//	//DWORD dw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
//	//SetConsoleTextAttribute(handle, dw);
//	//COORD crd = { chosenText->GetCord().X , chosenText->GetCord().Y + 2 };
//	//SetConsoleCursorPosition(handle, crd);
//	//int maxLength = optionsList->LongestOptios();
//	//for (short i = crd.Y; i < crd.Y + listSize; i++) {
//	//	for (short j = crd.X; j < crd.X + maxLength; j++) {
//	//		COORD tmpCrd = { j , i };
//	//		SetConsoleCursorPosition(handle, tmpCrd);
//	//		cout << " ";
//	//	}
//	//}
//}

void ComboBox::HandleInput(INPUT_RECORD iRecord) {
	//if (!isClicked) return;
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

bool ComboBox::checkPosition(MOUSE_EVENT_RECORD mer) {
	if (mer.dwMousePosition.X == chosenText->GetCord().X &&
		mer.dwMousePosition.Y == chosenText->GetCord().Y) {
		return true;
	}
	return false;
}

void ComboBox::CloseList() {
	displayList = false;
	optionsList->Hide();
	string plus = optionsList->GetValue();
	if (plus != "no choose") {
		plus[0] = '+';
		plus[1] = ' ';
		plus[2] = ' ';
	}
	else {
		plus = "+ " + plus;
	}
	chosenText->CleanLabel();
	chosenText->SwitchContent(plus);
	if (border != BorderType::None) {
		for (int j = coord.Y + 1; j < coord.Y + 1 + listSize; j++) {
			for (int i = coord.X; i < coord.X + chosenText->GetWidth(); i++) {
				COORD tmpCoord = { i, j };
				SetConsoleCursorPosition(handle, tmpCoord);
				SetConsoleTextAttribute(handle, chosenText->GetColor());
				cout << ' ';
			}
		}
	}
	return;
}

void ComboBox::OpenList() {
	displayList = true;
	optionsList->Show();
	optionsList->SetCoordinates(coord.X, coord.Y + 1);
}

string ComboBox::GetValue() {
	return optionsList->GetValue();
}

ComboBox::~ComboBox() {
	delete(chosenText);
	delete(optionsList);
}

void ComboBox::SetForeground(ForegroundColor color) {
	optionsList->SetForeground(color);
	chosenText->SetForeground(color);
}

void ComboBox::SetBackground(BackgroundColor color) {
	optionsList->SetBackground(color);
	chosenText->SetBackground(color);
}

void ComboBox::SetBorder(BorderType _border) {
	border = _border;
}

void ComboBox::Hide() {
	chosenText->Hide();
	optionsList->Hide();
	short length = (short)chosenText->GetWidth();
	DWORD dw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	if (border != BorderType::None) {
		for (short j = coord.Y - 1; j < coord.Y + listSize + 3; j++) {
			for (short i = coord.X - 1; i < coord.X + length + 2; i++) {
				COORD tmpCoord = { i,j };
				SetConsoleCursorPosition(handle, tmpCoord);
				SetConsoleTextAttribute(handle, dw);
				cout << ' ';
			}
		}
		SetCoordinates(coord.X, coord.Y - 1);
	}
}

void ComboBox::SetCoordinates(short x, short y) {
	coord = { x , y };
	chosenText->SetCoordinates(x, y);
	optionsList->SetCoordinates(x, y + 1);
}
