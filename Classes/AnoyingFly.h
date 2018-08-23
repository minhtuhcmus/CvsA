#include "BWTP_Func.h"

class Enemies;

#pragma once
class AnoyingFly : public Sprite
{
public:
	AnoyingFly(Vec2 position, float scale, int tag, Enemies* enemiesHandler, Layer* parent);
	~AnoyingFly();

	void destroy();
	int getEnemyTag();
	void setMoveToPos(Vec2 position);

private:
	Node* _scene;
	Enemies* _enemiesHandler;
	Vec2 _a_Pos;
	Vec2 _b_Pos;

	int _tag = 0;
	float _speed = 100.0f;

	bool onContactBegin(PhysicsContact& contact);
};

