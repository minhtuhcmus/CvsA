#include "BWTP_Func.h"

class Enemies;

#pragma once
class ChickenEater : public Sprite
{
public:
	ChickenEater(Vec2 position, float scale, int tag, Enemies* enemiesHandler, Layer* parent);
	~ChickenEater();

	void destroy();
	int getEnemyTag();

private:
	Node* _scene;
	Enemies* _enemiesHandler;

	int _tag = 0;

	bool onContactBegin(PhysicsContact& contact);
};

