#include "Portal.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "SimpleAudioEngine.h"
#include "Bullet.h"

Portal::Portal(Vec2 position, Vec2 faceTo, float scale,int tag, Layer* parent)
{
	this->autorelease();

	// Start :: Init and set idle animation
	_tag = tag;
	this->initWithSpriteFrame(getSprite("Portal_Blue_01.png"));
	this->setTag(PORTAL);
	this->setPosition(position);
	this->setScaleX(0.5f);
	this->setName("Portal" + to_string(_tag));
	parent->addChild(this);

	Animation* animation = Animation::createWithSpriteFrames(getAnimation("Portal_Blue_%02d.png"), 0.1f);
	Animate* animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));

	Sprite* smallPortal = Sprite::createWithSpriteFrame(getSprite("Portal_Blue_01.png"));
	smallPortal->setScale(0.65f);
	smallPortal->setPosition(Vec2(this->getContentSize().width*0.1, this->getContentSize().height*0.5));
	Animation* animation2 = Animation::createWithSpriteFrames(getAnimation("Portal_Blue_%02d.png"), 0.1f);
	Animate* animate2 = Animate::create(animation2);
	smallPortal->runAction(RepeatForever::create(animate2));
	this->addChild(smallPortal, -1);

	Vec2 distance = faceTo - position;
	float rotate = -1 * atanf(distance.y / distance.x) * 180 / PI;
	if (distance.x < 0) {
		rotate += 180.0f;
	}
	this->setRotation(rotate);
	// End :: Init and set idle animation

	// Start :: Init Physicbody
	PhysicsBody* PortalBody = PhysicsBody::createBox(Size(this->getContentSize().width/2, this->getContentSize().height));
	PortalBody->setGroup(PORTAL);
	PortalBody->setCategoryBitmask(PORTAL);
	PortalBody->setCollisionBitmask(0);
	PortalBody->setContactTestBitmask(BULLET);
	PortalBody->setDynamic(false);

	this->addComponent(PortalBody);

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(Portal::onContactBegin, this);
	colissionListener->onContactSeparate = CC_CALLBACK_1(Portal::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);

	// End :: Init Physicbody
}


Portal::~Portal()
{
	//delete this;
}

bool Portal::onContactBegin(PhysicsContact& contact) {
	auto objectB = (Portal*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (Portal*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Portal* other = nullptr;
	Portal* Portal = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "Portal" + to_string(objectA->getEnemyTag())) {
		Portal = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "Portal" + to_string(objectB->getEnemyTag())) {
		Portal = objectB;
		other = objectA;
	}

	if (other == nullptr || Portal == nullptr) return true;

	//MessageBox(to_string(other->getTag()).c_str(), "a");

	switch (other->getTag())
	{
	case BULLET:
	case ROCK:
		_isBulletIn = true;
		Portal->leteport((Bullet*)other);
		break;
	default:
		break;
	}
	
	return true;
}

bool Portal::onContactSeparate(PhysicsContact& contact) {
	auto objectB = (Portal*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (Portal*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Portal* other = nullptr;
	Portal* Portal = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "Portal" + to_string(objectA->getEnemyTag())) {
		Portal = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "Portal" + to_string(objectB->getEnemyTag())) {
		Portal = objectB;
		other = objectA;
	}

	if (other == nullptr || Portal == nullptr) return true;

	//MessageBox(to_string(other->getTag()).c_str(), "a");

	switch (other->getTag())
	{
	case BULLET: {
		break;
	}
	default:
		break;
	}

	return true;
}

void Portal::leteport(Bullet* bullet) {
	if (_isTeleported == true) return;
	_isTeleported = true;
	_outputPortal->_isTeleported = true;

	// Play Portal explode sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom.wav");
	experimental::AudioEngine::play2d("teleport.mp3");
	// If Portal's not exploded yet
	_bulletVelocity = bullet->getPhysicsBody()->getVelocity();
	_bulletDirection = (_bulletVelocity.x > 0) ? 1 : -1;
	Vec2 distanceFromBulletToThis = bullet->getPosition() - this->getPosition();

	bullet->getPhysicsBody()->setVelocity(Vec2::ZERO);
	bullet->setVisible(false);
	_bullet = bullet;

	_bulletVelocity.x = _bulletVelocity.x * cos(-_outputPortal->getRotation() * PI / 180) - _bulletVelocity.y * sin(-_outputPortal->getRotation() * PI / 180);
	_bulletVelocity.y = _bulletVelocity.x * sin(-_outputPortal->getRotation() * PI / 180) + _bulletVelocity.y * cos(-_outputPortal->getRotation() * PI / 180);

	MoveTo* moveTo = MoveTo::create(0.0f, _outputPortal->getPosition() + Vec2(0.0f,distanceFromBulletToThis.y));
	ScaleTo* delay = ScaleTo::create(1.0f, bullet->getScaleX(), bullet->getScaleY());
	CallFunc* teleportBullet = CallFunc::create([&] {
		_bullet->setVisible(true);
		_bullet->getPhysicsBody()->setVelocity(_bulletDirection * _bulletVelocity);
	});

	bullet->runAction(Sequence::create(delay, teleportBullet, moveTo, nullptr));

	CallFunc* setTeleport = CallFunc::create([&] {
		_isTeleported = false;
		_outputPortal->_isTeleported = false;
	});
	ScaleTo* delayPortal = ScaleTo::create(1.0f, 1.0f);
	ScaleTo* scale = ScaleTo::create(0.2f, 0.5f, 1.0f);
	_outputPortal->runAction(Sequence::create(delayPortal, scale, setTeleport, nullptr));
}

int Portal::getEnemyTag() {
	return this->_tag;
}

void Portal::setOuputPortal(Portal* output) {
	_outputPortal = output;
}

