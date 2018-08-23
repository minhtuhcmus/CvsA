#include "BWTP_Func.h"

USING_NS_CC;

#pragma once
class EnemyExplosion : public Node
{
public:
	EnemyExplosion(string enemyName, Vec2 position, float scale, Node* parent);
	~EnemyExplosion();

private:
	string _enemyName;

	int _explosionForce = EXPLOSION_FORCE;

	void remove();
	void explode();
};

