#include "BWTP_Func.h"

#pragma once
class GunButton : public ui::Button
{
public:
	GunButton(Node* parent);
	~GunButton();

private:
	bool _isClicked = false;
	void onClick(Ref* sender, ui::Widget::TouchEventType type);
};

