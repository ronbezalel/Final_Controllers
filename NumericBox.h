#ifndef _NUMERICBOX_H_
#define _NUMERICBOX_H_

#include <Windows.h>
#include <string>
#include "IControl.h"

using namespace std;

class NumericBox : public IControl
{
private:
	int width;
	DWORD numericDW;
	int BoxValue;
	int maxValue;
	int minValue;

	void MouseEventProc(MOUSE_EVENT_RECORD mer);
	int checkPosition(MOUSE_EVENT_RECORD mer);
public:
	NumericBox(int width, int min, int max);
	~NumericBox();
	void SetValue(int value);
	void Show();
	void Hide();
	int GetValue();
	void SetCoordinates(short x, short y);
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);
	void HandleInput(INPUT_RECORD iRecord);
	void SetMin(int minimum);
	void SetMax(int maximum);

};

#endif