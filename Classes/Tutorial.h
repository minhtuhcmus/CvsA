#include "BWTP_Func.h"

#pragma once
class Tutorial : public Node
{
public:
	Tutorial(string levelName, Node* parent);
	~Tutorial();

	void setEnable();

private:
	ui::Button* _firstTutorial;
};

