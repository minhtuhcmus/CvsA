#include "BWTP_Func.h"

#pragma once
class ObjectTutorial : public ui::Button
{
public:
	ObjectTutorial(Vec2 position, float rotation, string path, string message, ui::Button* nextObjTutorial, Node* parent);
	~ObjectTutorial();

	void addToParent(Node* parent);

private:
	ui::Button* _next;

	void next(Ref* sender, ui::Widget::TouchEventType type);
};

