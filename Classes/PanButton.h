#include "BWTP_Func.h"

#pragma once
class PanButton : public ui::Button
{
public:
	PanButton(Node* parent);
	~PanButton();

private:
	bool _isClicked = false;
	void onClick(Ref* sender, ui::Widget::TouchEventType type);
};

