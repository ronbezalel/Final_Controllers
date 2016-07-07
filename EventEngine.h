#pragma once

#include "IControl.h"
#include <Windows.h>

class EventEngine
{
private:
	void moveFocus(IControl &main, IControl *focused);
	HANDLE handle;
	DWORD _consoleMode;
	void ErrorExit(LPSTR lpszMessage);
	HANDLE hStdin;
	DWORD fdwSaveOldMode;

public:
	EventEngine();
	void run(IControl &c);
	virtual ~EventEngine();


	//Graphics _graphics;
	//HANDLE _console;
	//DWORD _consoleMode;
};
