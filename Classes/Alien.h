#include "BWTP_Func.h"

class Enemies;


#pragma once
class Alien : public Sprite
{
public:
	Alien(Vec2 position, float scale, int tag, Enemies* enemiesHandler, Layer* parent);
	~Alien();

	void destroy();
	int getEnemyTag();

private:
	Node* _scene;
	Enemies* _enemiesHandler;

	int _tag = 0;

	bool onContactBegin(PhysicsContact& contact);
};

