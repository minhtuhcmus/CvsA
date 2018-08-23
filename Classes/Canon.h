#include "BWTP_Func.h"

#pragma once
class Canon : public ui::Button
{
public:
	Canon(Vec2 position, float scale, int tag, Layer* parent);
	~Canon();

	void load(Node* bullet);
	int getEnemyTag();

private:
	Sprite* _shootingPoint;
	ui::Button* _bgButton;
	Node* _bullet = nullptr;

	int _tag = 0;
	float _timeDelay = 0.5f;
	bool _isExploded = false;

	bool onContactBegin(PhysicsContact& contact);
	void onTouch(Ref* sender, ui::Widget::TouchEventType type);
};

