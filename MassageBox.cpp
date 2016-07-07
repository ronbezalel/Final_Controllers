#include "MassageBox.h"
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
MassageBox::MassageBox(int height, int width) : IControl(0, 0), boxWidth(width), showed(false) {
	boxHeight = height < 4 ? 4 : height;
	wrapperPanel = new Panel(boxWidth, boxHeight);
	SetWidth(width);
	SetHeight(height);
}
MassageBox::~MassageBox() {
	delete(wrapperPanel);
}
void MassageBox::SetText(string text) {
	if (text.size() < boxWidth) {
		boxValue = text;
	}
	else {

		vector<string> tmp;
		string delimiter = " ";

		size_t pos = 0;
		string token;
		while ((pos = text.find(delimiter)) != string::npos) {
			token = text.substr(0, pos);
			text.erase(0, pos + delimiter.length());
			tmp.push_back(token);
		}
		tmp.push_back(text);

		int count = 0;
		int lines = 1;
		for (int i = 0; i < tmp.size(); i++) {
			count += tmp[i].size() + 1;
			if (tmp[i].size() >= boxWidth) {
				boxValue = "";
				return;
			}
			if (count > boxWidth) {
				if (++lines >= boxHeight - 3)break;
				boxValue += "\n";
				count = tmp[i].size();
				boxValue += tmp[i] + " ";
			}
			else {
				boxValue += tmp[i] + " ";
			}
		}
	}
}

void MassageBox::SetTitle(string title) {
	if (title.size() < boxWidth) {
		boxTitle = title;
	}
}

void MassageBox::Show() {
	if (!showed) {
		wrapperPanel->AddControl(this, 0, 0);
		showed = true;
		wrapperPanel->Show();
		COORD endPrint = { coord.X + 1 , coord.Y + 1 };
		SetConsoleCursorPosition(handle, endPrint);
		cout << boxTitle;
		endPrint = { coord.X + 1 , coord.Y + 2 };
		SetConsoleCursorPosition(handle, endPrint);
		for (int z = 0; z < boxWidth; z++) {
			cout << '\xCD';
		}
		endPrint = { coord.X + 1 , coord.Y + 3 };
		SetConsoleCursorPosition(handle, endPrint);
		short lines = 1;
		for (short s = 0; s < boxValue.size(); s++) {
			if (boxValue[s] == '\n') {
				endPrint = { coord.X + 1 , coord.Y + 3 + lines++ };
				SetConsoleCursorPosition(handle, endPrint);
				continue;
			}
			cout << boxValue[s];
		}
		endPrint = { coord.X + 1 , coord.Y + (short)boxHeight };
		SetConsoleCursorPosition(handle, endPrint);
		cout << "OK";
		short w = (short)boxWidth - 5;
		endPrint = { coord.X + w , coord.Y + (short)boxHeight };
		SetConsoleCursorPosition(handle, endPrint);
		cout << "CANCEL";
	}
}
void MassageBox::Hide() {
	wrapperPanel->Hide();
}
void MassageBox::SetCoordinates(short x, short y) {
	coord = { x,y };
	wrapperPanel->SetCoordinates(x, y);
}
void MassageBox::SetForeground(ForegroundColor color) {
	wrapperPanel->SetForeground(color);
	switch (color)
	{
	case ForegroundColor::Red:
		boxDw = boxDw | (FOREGROUND_RED);
		break;
	case ForegroundColor::Blue:
		boxDw = boxDw | (FOREGROUND_BLUE);
		break;
	case ForegroundColor::Green:
		boxDw = boxDw | (FOREGROUND_GREEN);
		break;
	case ForegroundColor::Purple:
		boxDw = boxDw | (FOREGROUND_RED | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Teal:
		boxDw = boxDw | (FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case ForegroundColor::Yellow:
		boxDw = boxDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ForegroundColor::White:
		boxDw = boxDw | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case ForegroundColor::Black:
		boxDw = boxDw | !(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		break;
	}
}
void MassageBox::SetBackground(BackgroundColor color) {
	wrapperPanel->SetBackground(color);
	switch (color)
	{
	case BackgroundColor::Red:
		boxDw = boxDw | (BACKGROUND_RED);
		break;
	case BackgroundColor::Blue:
		boxDw = boxDw | (BACKGROUND_BLUE);
		break;
	case BackgroundColor::Green:
		boxDw = boxDw | (BACKGROUND_GREEN);
		break;
	case BackgroundColor::Purple:
		boxDw = boxDw | (BACKGROUND_RED | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Teal:
		boxDw = boxDw | (BACKGROUND_RED | BACKGROUND_GREEN);
		break;
	case BackgroundColor::Yellow:
		boxDw = boxDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case BackgroundColor::White:
		boxDw = boxDw | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	case BackgroundColor::Black:
		boxDw = boxDw | !(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		break;
	}
}
void MassageBox::SetBorder(BorderType _border) {
	border = _border;
	wrapperPanel->SetBorder(_border);
}
void MassageBox::MouseEventProc(MOUSE_EVENT_RECORD mer) {
	if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
		COORD tmpCord = { coord.X + 2, coord.Y };
		if (border != BorderType::None) {
			tmpCord = { tmpCord.X + 1, tmpCord.Y + 1 };
		}
		int res = checkPosition(mer);
		if (res == 1) {
			selectedOption = "OK";
			Hide();
		}
		if (res == 0) {
			selectedOption = "CANCEL";
			Hide();
		}
		else {
			return;
		}
	}
}

int MassageBox::checkPosition(MOUSE_EVENT_RECORD mer) {
	if (mer.dwMousePosition.X >= coord.X + 1 && mer.dwMousePosition.X <= coord.X + 2 && mer.dwMousePosition.Y == coord.Y + boxHeight) {
		return 1;
	}
	if (mer.dwMousePosition.X >= coord.X + boxWidth - 5 && mer.dwMousePosition.X <= coord.X + boxWidth && mer.dwMousePosition.Y == coord.Y + boxHeight) {
		return 0;
	}
	return -1;
}

void MassageBox::HandleInput(INPUT_RECORD iRecord) {
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
#endif