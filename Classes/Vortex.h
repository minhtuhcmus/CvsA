#include "BWTP_Func.h"

#pragma once
class Vortex : public ui::Button
{
public:
	Vortex(Vec2 position, Node* parent);
	~Vortex();

private:
	Node* _scene;

	bool _isClicked = false;

	void onTouch(Ref* sender, ui::Widget::TouchEventType type);
};

