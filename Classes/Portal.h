#include "BWTP_Func.h"

class Bullet;

#pragma once
class Portal : public Sprite
{
public:
	Portal(Vec2 position, Vec2 faceTo, float scale, int tag, Layer* parent);
	~Portal();

	bool _isTeleported = false;

	void setOuputPortal(Portal* output);
	void leteport(Bullet* bullet);
	int getEnemyTag();

private:
	Portal* _outputPortal;
	Bullet* _bullet;
	Vec2 _bulletVelocity;

	int _tag = 0;
	int _bulletDirection;
	bool _isBulletIn = false;

	bool onContactBegin(PhysicsContact& contact);
	bool onContactSeparate(PhysicsContact& contact);
};

