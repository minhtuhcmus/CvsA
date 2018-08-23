#include "BWTP_Func.h"

class Bullet;
class Ammo;

USING_NS_CC;

#pragma once
class Gun : public Sprite
{
public:
	Gun(int ammo, Node* player, Layer* scene);
	~Gun();

	Sprite* _shootingPoint;
	Sprite* _shootingPoint2;

	void Shoot();
	int getAmmo();
	void reloadAmmo(int ammo);
	//void setRotation(float rotation);

private:
	Bullet* _bullet;
	Bullet* _fakeBullet = nullptr;
	Ammo* _ammo;
	Layer* _scene;

	bool _isShot = false;
	float _currentRotation;
	float _timeToCount;
	float _timeToSetRotation = 0.2f;

	void checkOutOfBullets();
	void update(float dt) override;
};

