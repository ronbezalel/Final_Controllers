#pragma once
#ifndef _PANEL_H_
#define _PANEL_H_


#include <Windows.h>
#include "IControl.h"
#include "ControllerSureface.h"
#include <vector>

class Panel : public IControl
{
private:
	vector<ControllerSureface> cs;
	int panelHeight;
	int panelWidth;
	vector<IControl*>  controllers;
	DWORD panelDw;
	int currentController;
	int checkPositionController(MOUSE_EVENT_RECORD mer);


public:
	Panel(int width, int height);
	~Panel();
	void AddControl(IControl* control, int left, int top);
	void Show();
	void Hide();
	void SetCoordinates(short x, short y);
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);
	void HandleInput(INPUT_RECORD iRecord);
	TabStatus Tab();
};
#endif


