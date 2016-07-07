
#include <Windows.h>
#include <stdio.h>
#include "ComboBox.h"
#include "TextBox.h"
#include "CheckList.h"
#include "NumericBox.h"
#include "Button.h"
#include "RadioList.h"
#include "Label.h"
#include "Panel.h"
#include "MassageBox.h"
#include "EventEngine.h"

HANDLE hStdin;
DWORD fdwSaveOldMode;

struct ExListener : public MouseListener {
	void MousePressed(IControl &b, int x, int y, bool isLeft) {
		if (isLeft) {
			COORD crd = { x,y };
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleCursorPosition(handle, crd);
			DWORD dw = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			SetConsoleTextAttribute(handle, dw);
			cout << "bla bla!!!";
		}
	}
};


//void ErrorExit(LPSTR lpszMessage);

int main() {

	/* Label Main */

	DWORD dw = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	ExListener func;


	///////////////////////////////////////////////////////////////////
	EventEngine engine = EventEngine();



	//MassageBox box = MassageBox(10, 10);
	//box.SetText("bbbbbaavvccccc");
	//box.SetTitle("xxx");
	//box.SetForeground(ForegroundColor::White);
	//box.SetBackground(BackgroundColor::Blue);
	//box.SetBorder(BorderType::Double);
	//box.SetCoordinates(0, 0);
	//box.Show();
	//button accept one parameter width 
	//Sleep(2222);
	//box.Hide();
	//Button button = Button(10);
	//button.SetForeground(ForegroundColor::White);
	//button.SetBackground(BackgroundColor::Blue);
	//button.SetCoordinates(10, 10);
	//button.SetBorder(BorderType::None);
	//button.SetValue("xxx");
	//button.AddListener(&func);
	//button.SetBorder(BorderType::Single);

	Button button1 = Button(5);
	button1.SetForeground(ForegroundColor::White);
	button1.SetBackground(BackgroundColor::Blue);
	//button1.SetCoordinates(10, 10);
	button1.SetBorder(BorderType::None);
	button1.SetValue("123");
	button1.AddListener(&func);
	button1.SetBorder(BorderType::Single);

	//button1.Show();

	////////////////////////////////////////////////////////////////////////////////////////
	//NumericBox numeric = NumericBox(7, 0, 10);
	//numeric.SetForeground(ForegroundColor::White);
	//numeric.SetBackground(BackgroundColor::Blue);
	//numeric.SetBorder(BorderType::Double);
	//numeric.SetCoordinates(8, 8);
	//numeric.SetValue(1);
	//numeric.SetBorder(BorderType::Single);
	//numeric.Show();
	//numeric.SetMax(11);
	//numeric.SetMin(3);
	//Sleep(1111);
	//numeric.Hide();

	/* to test one of the controllers comment the "Label main" and un-commet the requested controller */

	Label label = Label(40, "Eyal & Ron & Yossi");
	label.SetForeground(ForegroundColor::Green);
	label.SetCoordinates(20, 0);
	//label.Show();
	//Sleep(2222);
	vector<string> list = {
		"line 1",
		"line 2",
		"line 3",
		"xx",
		"line 5"
	};
	ComboBox combo = ComboBox(15, list);
	combo.SetForeground(ForegroundColor::White);
	combo.SetBackground(BackgroundColor::Blue);
	combo.SetBorder(BorderType::None);
	//combo.SetCoordinates(0, 3);
	//combo.SetCoordinates(10, 10);
	//combo.Show();

	RadioList radio = RadioList(5, 15, list);
	radio.SetForeground(ForegroundColor::Red);
	radio.SetBackground(BackgroundColor::Black);
	radio.SetBorder(BorderType::Double);
	radio.SetCoordinates(0, 2);


	RadioList radio1 = RadioList(5, 15, list);
	radio1.SetForeground(ForegroundColor::Red);
	radio1.SetBackground(BackgroundColor::Black);
	radio1.SetBorder(BorderType::Double);
	//radio.SetCoordinates(0, 2);
	//Sleep(2222);
	//radio.Hide();

	/*CheckList check = CheckList(5, 15, list);
	check.SetForeground(ForegroundColor::Purple);
	check.SetBackground(BackgroundColor::Green);
	check.SetBorder(BorderType::Single);
	check.SetCoordinates(0, 18);
	check.Show();*/
	TextBox text = TextBox(5);
	text.SetCoordinates(20, 5);
	text.SetBackground(BackgroundColor::White);
	text.SetForeground(ForegroundColor::Blue);
	text.SetBorder(BorderType::None);


	//combo.Show();
	//radio.Show();
	//text.Show();
	//check.Show();
	//label.Show();
	//Sleep(1111);
	Panel p1 = Panel(20, 20);
	//p.SetCoordinates(0, 0);
	p1.SetBorder(BorderType::Double);
	p1.SetForeground(ForegroundColor::Purple);
	p1.SetBackground(BackgroundColor::Black);
	p1.AddControl(&button1, 2, 2);
	p1.AddControl(&radio, 4, 4);


	Panel p = Panel(50, 50);
	p.SetCoordinates(0, 0);
	p.SetBorder(BorderType::Single);
	p.SetForeground(ForegroundColor::White);
	p.SetBackground(BackgroundColor::Teal);
	//p.AddControl(&button1, 0, 0);
	//p.AddControl(&radio, 4, 4);

	//p.AddControl(&text, 4, 14);
	p.AddControl(&p1, 0, 0);
	p.AddControl(&combo, 30, 20);
	p.AddControl(&radio1, 30, 10);

	int x = 90;
	engine.run(p);
	return 0;
}
//Sleep(1111);
//Panel p1 = Panel(10, 5);
//p1.SetCoordinates(10, 20);
//p1.SetBorder(BorderType::Single);
//p1.SetForeground(ForegroundColor::Purple);
//p1.SetBackground(BackgroundColor::Green);
//p.Show();
//Sleep(1111);
//p.Hide();
//Sleep(1111);
//p.AddControl(&p1, 0, 10);
//p1.AddControl(&button1, 2, 2);
//p.Show();

//	DWORD cNumRead, fdwMode, i;
//	INPUT_RECORD irInBuf[128];
//
//	// Get the standard input handle. 
//	hStdin = GetStdHandle(STD_INPUT_HANDLE);
//	if (hStdin == INVALID_HANDLE_VALUE)
//		ErrorExit("GetStdHandle");
//
//	// Save the current input mode, to be restored on exit. 
//	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
//		ErrorExit("GetConsoleMode");
//
//	// Enable the window and mouse input events. 
//	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
//	if (!SetConsoleMode(hStdin, fdwMode))
//		ErrorExit("SetConsoleMode");
//
//	// Loop to read and handle the next 100 input events. 
//	while (true)
//	{
//		// Wait for the events. 
//		if (!ReadConsoleInput(
//			hStdin,      // input buffer handle 
//			irInBuf,     // buffer to read into 
//			128,         // size of read buffer 
//			&cNumRead)) // number of records read 
//			ErrorExit("ReadConsoleInput");
//
//		// Dispatch the events to the appropriate handler. 
//		for (i = 0; i < cNumRead; i++) {
//			//Send the input record to the textbox handler
//			radio.HandleInput(irInBuf[i]);
//			//check.HandleInput(irInBuf[i]);
//			text.HandleInput(irInBuf[i]);
//			combo.HandleInput(irInBuf[i]);
//			//numeric.HandleInput(irInBuf[i]);
//			//button.HandleInput(irInBuf[i]);
//			//button1.HandleInput(irInBuf[i]);
//			//box.HandleInput(irInBuf[i]);
//		}
//	}
//
//
//	// Restore input mode on exit.
//	SetConsoleMode(hStdin, fdwSaveOldMode);
//	return 0;
//}
//
//void ErrorExit(LPSTR lpszMessage) {
//	fprintf(stderr, "%s\n", lpszMessage);
//	// Restore input mode on exit.
//	SetConsoleMode(hStdin, fdwSaveOldMode);
//	ExitProcess(0);
//}