#include "BWTP_Func.h"

#pragma once
class TNT : public Sprite
{
public:
	TNT(Vec2 position, float scale, int tag, Layer* parent);
	~TNT();

	void explode();
	int getEnemyTag();

private:
	int _tag = 0;
	bool _isExploded = false;

	bool onContactBegin(PhysicsContact& contact);
};

