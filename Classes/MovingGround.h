#include "BWTP_Func.h"

#pragma once
class MovingGround : public Sprite
{
public:
	MovingGround(Vec2 position, float scale, int tag, Layer* parent);
	~MovingGround();

	void setMoveToPos(Vec2 position);
	void setFaceTo(Vec2 position);

private:
	Node* _scene;
	Vec2 _a_Pos;
	Vec2 _b_Pos;

	int _tag = 0;
	float _speed = 200.0f;
	bool _isVertical = false;
	bool _isHorizontal = false;

	bool onContactBegin(PhysicsContact& contact);
};

