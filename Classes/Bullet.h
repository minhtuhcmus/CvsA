#include "BWTP_Func.h"

class Ammo;

USING_NS_CC;

#pragma once
class Bullet : public Sprite
{
public:
	Bullet(float direction, float rotate, Vec2 position, Node* scene);
	~Bullet();

	void setAmmo(Ammo* ammo);
	void setDisableHitCounter();
	int getHit();
	void remove();
	MotionStreak* _trailRenderer;

private:
	Ammo* _ammo;
	Node* _currentHitTarget;
	Node* _scene;
	Sprite* _bulletCountDisplay_BG;
	Label* _bulletCountDisplay;

	float _scale = 0.7;
	int _hitCounter = BULLET_HITS;
	bool _isRemoved = false;

	bool onContactBegin(PhysicsContact& contact);
	void onContactSeparate(PhysicsContact& contact);

	void update(float) override;
	void rotate();
};

