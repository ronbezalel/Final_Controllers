#include "CheckList.h"



CheckList::CheckList(int height, int width, vector<string> options) :
	InteractiveList(height, width, options)
{
}


CheckList::~CheckList()
{
}

void CheckList::Mark() {
	if (chosen[currentRow]) {
		string newText = list[currentRow].GetValue();
		newText[1] = ' ';
		int width = list[currentRow].GetWidth();
		COORD newCoord = list[currentRow].GetCord();
		DWORD color = list[currentRow].GetColor();
		list[currentRow] = Label(width, 0, newText, false);
		list[currentRow].SetCoordinates(newCoord.X, newCoord.Y);
		list[currentRow].SetColor(color);
		chosen[currentRow] = false;
	}
	else {
		string newText = list[currentRow].GetValue();
		newText[1] = 'X';
		COORD newCoord = list[currentRow].GetCord();
		DWORD color = list[currentRow].GetColor();
		int width = list[currentRow].GetWidth();
		list[currentRow] = Label(width, 0, newText, false);
		list[currentRow].SetColor(color);
		list[currentRow].SetCoordinates(newCoord.X, newCoord.Y);
		chosen[currentRow] = true;
	}
	list[currentRow].Print();
	return;
}

string CheckList::GetValue() {
	bool* rows = GetChosenRows();
	string res = "";
	for (int i = 0; i < list.size(); i++) {
		if (rows[i]) {
			string tmp = list[i].GetValue() + ',';
			for (int j = 4; j < tmp.size(); j++) {
				res.push_back(tmp[j]);
			}
		}
	}
	return res;
}

void CheckList::SelectIndex(size_t index) {
	if (index > list.size() || index < 1) {
		return;
	}
	chosen[index - 1] = true;
	string newText = list[index - 1].GetValue();
	newText[1] = 'X';
	list[index - 1].SetValue(newText);
	if (hoverEnable) {
		list[index - 1].Print();
	}
}
void CheckList::DeselectIndex(size_t index) {
	if (index > list.size() || index < 1) {
		return;
	}
	chosen[index - 1] = false;
	string newText = list[index - 1].GetValue();
	newText[1] = ' ';
	list[index - 1].SetValue(newText);
	if (hoverEnable) {
		list[index - 1].Print();
	}
}

vector<size_t> CheckList::GetSelectedIndices() {
	vector<size_t> res;
	for (int i = 0; i < list.size(); i++) {
		if (chosen[i]) {
			res.push_back(i + 1);
		}
	}
	return res;
}
