#include "Player.h"
#include "Gun.h"
#include "Pan.h"
#include "PreloadedSprite.h"

Player::Player(Vec2 position,int ammo, float scale, Layer* scene)
{
	this->autorelease();

	scene->addChild(this);
	this->setName("Player");
	//this->setAnchorPoint(Vec2(0.5, 0));
	this->setPosition(position);
	this->setScale(scale);

	//_body = Sprite::createWithSpriteFrame(getSprite("Body.png"));
	//_body->setPosition(Vec2(-_body->getContentSize().width * 0.4, 0));
	//_body->setAnchorPoint(Vec2(0.5, 0));
	//this->addChild(_body, 1);

	//_head = Sprite::createWithSpriteFrame(getSprite("Head.png"));
	//_head->setAnchorPoint(Vec2(0.45, 0.0));
	//_head->setPosition(Vec2(-_body->getContentSize().width * 0.3, _body->getContentSize().height * 0.9));
	//this->addChild(_head, 2);

	_gun = new Gun(ammo, this, scene);
	this->addChild(_gun, 3);

	//child = root->FirstChildElement();
	//_gun = Sprite::createWithSpriteFrame(loadAnim(child, "Gun").at(0));
	//_gun->setPosition(Vec2(_body->getContentSize().width * 0.35, _body->getContentSize().height * 0.7));
	//_gun->setAnchorPoint(Vec2(0, 0.5));
	//this->addChild(_gun, 3);

	//_shootingPoint = Sprite::create("bullet/ShootingPoint.png");
	//_shootingPoint->setAnchorPoint(Vec2(0.5, 0.5));
	//_shootingPoint->setPosition(_gun->getContentSize().width, _gun->getContentSize().height * 0.8);
	//_gun->addChild(_shootingPoint);
}


Player::~Player()
{
	//delete this;
}

void Player::setBodyPartsRotateion(float degree) {
	//_head->setRotation(degree);
	_gun->setRotation(degree);
}

void Player::shoot() {
	_gun->Shoot();
}

int Player::getAmmo() {
	return _gun->getAmmo();
}

void Player::reloadAmmo(int ammo) {
	_gun->reloadAmmo(ammo);
}

void Player::setOpacityForAll(float a) {
	_body->setOpacity(a);
	_head->setOpacity(a);
	_gun->setOpacity(a);
}