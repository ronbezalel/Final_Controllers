#pragma once
#ifndef _MASSAGEBOX_H_
#define _MASSAGEBOX_H_
#include "IControl.h"
#include "Panel.h"
#include <vector>

class MassageBox : IControl
{
private:
	int boxWidth;
	int boxHeight;
	string boxTitle;
	string boxValue;
	DWORD boxDw;
	Panel* wrapperPanel;
	bool showed;
	int checkPosition(MOUSE_EVENT_RECORD mer);
	string selectedOption;

public:
	MassageBox(int height, int width);
	~MassageBox();
	void SetText(string text);
	void SetTitle(string title);
	void Show();
	void Hide();
	void SetCoordinates(short x, short y);
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);
	void MouseEventProc(MOUSE_EVENT_RECORD mer);
	void HandleInput(INPUT_RECORD iRecord);
};

#endif