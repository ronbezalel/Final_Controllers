#include "EventEngine.h"



EventEngine::EventEngine()
{
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(handle, &_consoleMode);
	SetConsoleMode(handle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

void EventEngine::run(IControl &c)
{
	c.Show();
	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128];
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	// Save the current input mode, to be restored on exit. 
	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events. 
	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");

	// Loop to read and handle the next 100 input events. 
	while (true)
	{
		// Wait for the events. 
		if (!ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead)) // number of records read 
			ErrorExit("ReadConsoleInput");

		// Dispatch the events to the appropriate handler. 
		for (i = 0; i < cNumRead; i++) {
			c.HandleInput(irInBuf[i]);
		}
	}


	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
	return;
}

EventEngine::~EventEngine()
{
	SetConsoleMode(handle, _consoleMode);
}

void EventEngine::moveFocus(IControl &main, IControl *focused)
{
}

void EventEngine::ErrorExit(LPSTR lpszMessage) {
	fprintf(stderr, "%s\n", lpszMessage);
	// Restore input mode on exit.
	SetConsoleMode(hStdin, fdwSaveOldMode);
	ExitProcess(0);
}