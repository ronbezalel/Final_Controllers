#ifndef _INTERACTIVELIST_H_
#define _INTERACTIVELIST_H_

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "Label.h"
#include "IControl.h"
#include <vector>

class InteractiveList : public IControl
{
protected:
	vector<Label> list;
	bool* chosen;
	short currentRow;
	short currentPosition;
	int rowMaxLength;
	DWORD generalDw;
	bool hoverEnable;
	int maxRowNumber;
	bool firstShow;
	bool keyFocus;


	void MoveUp();
	void MoveDown();
	virtual void Mark();
	void MousePressed(MOUSE_EVENT_RECORD moer);
	void MouseMoved(MOUSE_EVENT_RECORD mer);
	int CheckPosition(MOUSE_EVENT_RECORD mer);
	void MousePressedNoMark(MOUSE_EVENT_RECORD moer);

public:
	InteractiveList(string* textList, int listSize, short width, short height, DWORD color);
	InteractiveList(int height, int width, vector<string> options);
	string GetValue();
	void KeyEventProc(KEY_EVENT_RECORD ker);
	void MouseEventProc(MOUSE_EVENT_RECORD mer);
	void HandleInput(INPUT_RECORD iRecord);
	bool* GetChosenRows();
	~InteractiveList();
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);
	void Hide();
	void SetCoordinates(short x, short y);
	bool AddSelectedItem(string item);
	bool RemoveSelectedItem(string item);
	void Show();
	TabStatus Tab();
};

#endif
