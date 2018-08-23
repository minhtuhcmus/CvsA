#include "Ammo.h"

Ammo::Ammo(int ammo)
{
	this->autorelease();

	_ammo = ammo;
	this->setScale(0.5);
	this->setPosition(Director::getInstance()->getVisibleSize()/20);
	this->setAnchorPoint(Vec2(0, 0));

	for (int i = 0; i < _ammo; i++) {
		_bulletList.pushBack(Sprite::create("player/bullet/Chicken.png"));
		_bulletList.at(i)->setPositionX(i * _bulletList.at(i)->getContentSize().width);
		this->addChild(_bulletList.at(i));
	}
}


Ammo::~Ammo()
{
	//delete this;
}

void Ammo::popBullet() {
	_ammo--;
	_bulletList.back()->removeFromParentAndCleanup(1);
	_bulletList.popBack();
}

int Ammo::getAmmo() {
	return _ammo;
}

void Ammo::reloadAmmo(int ammo) {
	_ammo = ammo;

	for (int i = 0; i < _ammo; i++) {
		_bulletList.pushBack(Sprite::create("player/bullet/Chicken.png"));
		_bulletList.at(i)->setPositionX(i * _bulletList.at(i)->getContentSize().width);
		this->addChild(_bulletList.at(i));
	}
}

Vector<Node*>* Ammo::getListOfBullets() {
	return &_bulletList;
}