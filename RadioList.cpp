#include "RadioList.h"



RadioList::RadioList(string* textList, int listSize, short width, short hieght, DWORD color) :
	InteractiveList(textList, listSize, width, hieght, color) {


}

RadioList::RadioList(int height, int width, vector<string> options) :
	InteractiveList(height, width, options) {

}


int RadioList::LongestOptios() {
	int size = 0;
	for (int i = 0; i < list.size(); i++) {
		if (size <= list[i].Length()) size = list[i].Length();
	}
	return size;
}




size_t RadioList::GetSelectedIndex() {
	for (int i = 0; i < list.size(); i++) {
		if (chosen[i]) {
			return i + 1;
		}
	}
}
void RadioList::SetSelectIndex(size_t index) {
	if (index > list.size() || index < 1) {
		return;
	}
	for (int i = 0; i < list.size(); i++) {
		if (chosen[i] && (i + 1) != index) {
			chosen[i] = false;
			string newText = list[i].GetValue();
			newText[1] = ' ';
			list[i].SetValue(newText);
			if (hoverEnable) {
				list[i].Print();
			}
		}
	}
	chosen[index - 1] = true;
	string newText = list[index - 1].GetValue();
	newText[1] = 'X';
	list[index - 1].SetValue(newText);
	if (hoverEnable) {
		list[index - 1].Print();
	}
}

RadioList::~RadioList() {

}