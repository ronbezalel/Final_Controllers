#pragma once
#include "InteractiveList.h"

class RadioList : public InteractiveList
{
public:
	RadioList(string* textList, int listSize, short width, short hieght, DWORD color);
	RadioList(int height, int width, vector<string> options);
	~RadioList();
	size_t GetSelectedIndex();
	void SetSelectIndex(size_t index);
	int LongestOptios();






};
