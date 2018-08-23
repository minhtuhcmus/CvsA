#include "Gun.h"
#include "FX.h"
#include "Bullet.h"
#include "Ammo.h"

Gun::Gun(int ammo, Node* player, Layer* scene)
{
	this->autorelease();

	_timeToCount = _timeToSetRotation;

	this->initWithSpriteFrame(getSprite("Canon_Gun_01.png"));
	this->setPosition(player->getContentSize()/2);
	this->setAnchorPoint(Vec2(0.3, 0.5));

	_shootingPoint = Sprite::create("player/bullet/ShootingPoint.png");
	_shootingPoint->setOpacity(0);
	_shootingPoint->setPosition(Vec2(this->getContentSize().width * 0.8, this->getContentSize().height * 0.65));
	this->addChild(_shootingPoint);


	_shootingPoint2 = Sprite::create("player/bullet/ShootingPoint.png");
	_shootingPoint2->setOpacity(0);
	_shootingPoint2->setPosition(_shootingPoint->getPosition() + Vec2(20000.0f, 0.0f));
	this->addChild(_shootingPoint2);

	_ammo = new Ammo(ammo);
	scene->addChild(_ammo);

	_scene = scene;
	this->scheduleUpdate();

	
}

void Gun::update(float dt) {
	float rotation = this->getRotation();
	
	_timeToCount -= dt;
	if (rotation != _currentRotation) {
		if (_timeToCount < 0) {
			_currentRotation = rotation;
			_timeToCount = _timeToSetRotation;
		}
	}
	else {
		_timeToCount = _timeToSetRotation;
	}
}

Gun::~Gun()
{
	//delete this;
}

void Gun::Shoot() {
	if (_scene->getChildByName("Bullet") == nullptr) {
		_isShot = false;
	}
	if (!_isShot && _ammo->getAmmo()) {
		_ammo->popBullet();

		checkOutOfBullets();

		if (_scene->getChildByName("Tutorial")) {
			_scene->getChildByName("Tutorial")->removeFromParentAndCleanup(1);
		}

		if (_scene->getChildByName("Hint")) {
			_scene->getChildByName("Hint")->removeFromParentAndCleanup(1);
		}

		if (_scene->getChildByName("ObjectTutorial")) {
			_scene->getChildByName("ObjectTutorial")->removeFromParentAndCleanup(1);
		}

		Animation* animation = Animation::createWithSpriteFrames(getAnimation("Canon_Gun_%02d.png"), 0.03f);
		Animate* shootAnim = Animate::create(animation);
		CallFunc* shooting = CallFunc::create([&] {
			int directtion = (this->getParent()->getScaleX() > 0) ? 1 : -1;
			float rotation = (directtion > 0) ? _currentRotation : -_currentRotation;
			_bullet = new Bullet(directtion, rotation, _shootingPoint->convertToWorldSpace(Vec2::ZERO), _scene);
			_bullet->setAmmo(_ammo);
			_isShot = true;
		});

		this->runAction(Sequence::create(shootAnim, shooting, nullptr));
	}
}

void Gun::checkOutOfBullets() {
	//if (_ammo->getAmmo() < 1) {
	//	_bulletDisplayment->removeFromParentAndCleanup(1);
	//}
}

int Gun::getAmmo() {
	return _ammo->getAmmo();
}

void Gun::reloadAmmo(int ammo) {
	_ammo->reloadAmmo(ammo);
}

//void Gun::setRotation(float rotation) {
//	_currentRotation = rotation;
//}