#include "BWTP_Func.h"

class Gun;

USING_NS_CC;
using namespace tinyxml2;

#pragma once
class Player : public Sprite
{
public:
	Player(Vec2 position,int ammo, float scale, Layer* scene);
	~Player();

	Gun* _gun;

	void setBodyPartsRotateion(float degree);
	void shoot();
	void setOpacityForAll(float a);
	int getAmmo();
	void reloadAmmo(int ammo);

private:
	Sprite* _body;
	Sprite* _neck_and_arm;
	Sprite* _head;
	Sprite* _shootingPoint;
};

