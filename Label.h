#ifndef _LABEL_H_
#define _LABEL_H_

#include <Windows.h>
#include "IControl.h"
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;
class Label : public IControl {
private:
	//COORD cord = { 0,0 };
	//HANDLE handle;
	DWORD dw;
	CONSOLE_CURSOR_INFO cci;
	string labelText;
	int width;


public:
	Label(short width, short height, string text, bool isVisible);
	Label(int width, string value);// api ctor
	COORD GetCord();
	string GetValue();
	DWORD GetColor();
	void Hide();
	void SetColor(DWORD color);
	void SetCursorEnable(bool enable);
	void SetCoord(COORD newCoord);
	void SetValue(string value);
	void SwitchContent(string newText);
	void Show();
	void Print() const;
	void Hoover(bool isHover, DWORD newDw);
	int Length() const;
	void CleanLabel();
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);
	int GetWidth();
	~Label();
};


#endif // !_LABEL_H_