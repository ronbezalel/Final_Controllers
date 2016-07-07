#pragma once
#ifndef _CHECKLIST_H_
#define _CHECKLIST_H_

#include "InteractiveList.h"

class CheckList : public InteractiveList
{
private:
	void Mark();
public:
	CheckList(int height, int width, vector<string> options);
	~CheckList();
	string GetValue();
	void SelectIndex(size_t index);
	void DeselectIndex(size_t index);
	vector<size_t> GetSelectedIndices();
};

#endif