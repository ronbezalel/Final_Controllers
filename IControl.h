#ifndef _ICONTROL_
#define _ICONTROL_

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

enum class BorderType { Single, Double, None };
enum class ForegroundColor { Red, Blue, Green, Purple, Teal, Yellow, White, Black };
enum class BackgroundColor { Red, Blue, Green, Purple, Teal, Yellow, White, Black };

enum TabStatus { NEXT, STAY, WAIT };

class IControl
{
protected:
	HANDLE handle;
	COORD coord;
	BorderType border;
	bool isActive;
	int cWidth;
	int cHeight;

public:
	IControl(short width, short height);
	virtual ~IControl();
	virtual void Show() {};
	virtual void Hide() = 0;
	virtual void SetCoordinates(short x, short y);
	virtual void SetForeground(ForegroundColor color) = 0;
	virtual void SetBackground(BackgroundColor color) = 0;
	virtual void SetBorder(BorderType _border) = 0;
	virtual TabStatus Tab();

	virtual void KeyEventProc(KEY_EVENT_RECORD ker) {};
	virtual void MouseEventProc(MOUSE_EVENT_RECORD mer) {};
	virtual void HandleInput(INPUT_RECORD iRecord) {};

	int GetWidth() { return cWidth; };
	int GetHeigth() { return cHeight; };
	void SetWidth(int width) { cWidth = width; };
	void SetHeight(int height) { cHeight = height; };
	COORD GetCoord() { return coord; };
};

#endif