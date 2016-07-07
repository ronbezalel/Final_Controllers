#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "IControl.h"
#include <vector>

typedef struct MouseListener {
	virtual void MousePressed(IControl &b, int x, int y, bool isLeft) = 0;
} MouseListener;

class Button : public IControl
{
private:
	vector<MouseListener*> listen;
	DWORD generalDw;
	string buttonText;
	int btnWidth;
	//private func
	string GetValue();
	void MousePressed(MOUSE_EVENT_RECORD mer);
	void MouseMoved(MOUSE_EVENT_RECORD mer);
	int CheckPosition(MOUSE_EVENT_RECORD mer);
	void SetCursorEnable(bool enable);
public:
	Button(int width);
	~Button();
	void Show();
	void Hide();

	void SetCoordinates(short x, short y);
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);

	virtual void KeyEventProc(KEY_EVENT_RECORD ker) {};
	virtual void MouseEventProc(MOUSE_EVENT_RECORD mer);
	virtual void HandleInput(INPUT_RECORD iRecord);

	void AddListener(MouseListener* listener);
	void SetValue(string value);

};

#endif // !_BUTTON_H_


