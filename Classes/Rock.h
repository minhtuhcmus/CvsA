#include "BWTP_Func.h"

#pragma once
class Rock : public Sprite
{
public:
	Rock(Vec2 position, float scale, int tag, Layer* parent);
	~Rock();

	void explode();
	int getEnemyTag();

private:
	int _tag = 0;
	bool _isHit = false;
	PhysicsBody* _rockBody;

	bool onContactBegin(PhysicsContact& contact);
};

