#include "BWTP_Func.h"

#pragma once
class Pan : public Sprite
{
public:
	Pan(Vec2 position, int tag, Node* parent);
	~Pan();

private:
	Vec2 _touchPos;
	Sprite* _point = nullptr;

	bool _isClicked = false;
	bool _isSelected = false;
	float _currentRotation;
	float _rotateAngel = 22.5f;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void touchOnPan(Ref* sender, ui::Widget::TouchEventType type);
};

