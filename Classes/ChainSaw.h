#include "BWTP_Func.h"

class Bullet;

#pragma once
class ChainSaw : public Sprite
{
public:
	ChainSaw(Vec2 position, float scale, int tag, Layer* parent);
	~ChainSaw();

	void explode(Bullet* bullet);
	int getEnemyTag();

private:
	Node* _scene;

	int _tag = 0;

	bool onContactBegin(PhysicsContact& contact);
	void createSplitedChicken();
};

