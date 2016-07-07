#include "RadioList.h"
#include "Label.h"
#include "IControl.h"
#include "InteractiveList.h"

class ComboBox : public IControl
{
private:
	Label* chosenText;
	InteractiveList* optionsList;
	bool displayList = false;
	int listSize;
	bool firstShow;
	void KeyEventProc(KEY_EVENT_RECORD ker);
	void MouseEventProc(MOUSE_EVENT_RECORD mer);
	bool checkPosition(MOUSE_EVENT_RECORD mer);
	void CloseList();
	void OpenList();

	int cbWidth;
	int cbHeight;




public:
	ComboBox(string* textList, int size, short width, short height, DWORD color);
	ComboBox(int width, vector<string> options);
	~ComboBox();
	void Show();
	void HandleInput(INPUT_RECORD iRecord);
	string GetValue();
	void SetForeground(ForegroundColor color);
	void SetBackground(BackgroundColor color);
	void SetBorder(BorderType _border);
	void Hide();
	void SetCoordinates(short x, short y);

	int GetWidth() { return cbWidth; };
	int GetHeigth() { return cbHeight; };
	void SetWidth(int width) { cbHeight = width; };
	void SetHeight(int height) {};

};

