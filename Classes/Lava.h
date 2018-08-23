#include "BWTP_Func.h"

class Bullet;

#pragma once
class Lava : public Sprite
{
public:
	Lava(Vec2 position, float scale, int tag, Layer* parent);
	~Lava();

	void explode(Bullet* bullet);
	int getEnemyTag();

private:
	string _currentBullet;
	Vec2 _fx_Pos;
	Vec2 _fx_Velocity;

	int _tag = 0;

	bool onContactBegin(PhysicsContact& contact);
	void createRoastedChicken();
};

