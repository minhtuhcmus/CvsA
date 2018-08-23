#include "BWTP_Func.h"

#pragma once
class GameplayTutorial : public ui::Button
{
public:
	GameplayTutorial(Vec2 positionFrom, Vec2 positionTo, bool isDeleteOnBegin, ui::Button* nextObjTutorial, Node* parent);
	~GameplayTutorial();

private:
	ui::Button* _next;

	bool _isDeleteOnBegin = false;

	void next(Ref* sender, ui::Widget::TouchEventType type);
};

