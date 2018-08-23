#include "Alien.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "Enemies.h"
#include "SimpleAudioEngine.h"

Alien::Alien(Vec2 position, float scale,int tag, Enemies* enemiesHandler, Layer* parent)
{
	this->autorelease();

	//// Start :: Init and set idle animation
	_tag = tag;
	_enemiesHandler = enemiesHandler;
	_enemiesHandler->addEnemy();

	this->initWithSpriteFrame(getAnimation("Alien_%02d.png").at(0));

	this->setScale(scale, scale);
	this->setPosition(position);
	this->setName("Alien" + to_string(_tag));
	this->setTag(ENEMIES);
	parent->addChild(this);
	this->setAnchorPoint(Vec2(0.5, 0));

	Animation* animation = Animation::createWithSpriteFrames(getAnimation("Alien_%02d.png"), 0.2f);
	Animate* animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
	//// End :: Init and set idle animation

	//// Start :: init physicbody and hitbox
	PhysicsBody* alienBody = PhysicsBody::createBox(
		this->getContentSize()
	);
	alienBody->setCategoryBitmask(ENEMIES);
	alienBody->setCollisionBitmask(0);
	alienBody->setContactTestBitmask(BULLET + TNT_EXPLODE + ROCK);
	alienBody->setDynamic(false);
	this->addComponent(alienBody);

	// Flip X if Alien stand on the rightside of player
	auto player = parent->getChildByName("Player");
	if (this->getPositionX() - player->getPositionX() > 0) {
		this->setScaleX(-1 * this->getScaleX());
	}
	else {
		this->setScaleX(1 * this->getScaleX());
	}

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(Alien::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);
	//// End :: init physicbody and hitbox

	_scene = parent;
}


Alien::~Alien()
{
	//delete this;
}

void Alien::destroy() {
	// Play alien dead sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("alien.wav");
	experimental::AudioEngine::play2d("alien.mp3");
	// Spawn FX
	FX* bloodHitFX = new FX(
		this->getPosition(),
		Vec2(0.5, 0.5),
		Director::getInstance()->getRunningScene(),
		"Blood_Hit",
		0.03
	);

	bloodHitFX->runAction(TintTo::create(0.0f, Color3B::WHITE));

	// Spawn aliens explode FX
	EnemyExplosion* explode = new EnemyExplosion("Alien_Body_Parts", this->getPosition(), 1.0f, _scene);

	// Remove Alien
	this->removeFromParentAndCleanup(1);
}

bool Alien::onContactBegin(PhysicsContact& contact) {
	// Get head or body of a alien
	auto objectB = (Alien*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (Alien*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Alien* other = nullptr;
	Alien* alien = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "Alien" + to_string(objectA->getEnemyTag())) {
		alien = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "Alien" + to_string(objectB->getEnemyTag())) {
		alien = objectB;
		other = objectA;
	}

	if (other == nullptr || alien == nullptr) return true;

	switch (other->getTag())
	{
	case BULLET:
	case ROCK:
	case TNT_EXPLODE:
		_enemiesHandler->removeEnemy(alien->getName());
		alien->destroy();
		break;
	default:
		break;
	}

	return true;
}

int Alien::getEnemyTag() {
	if (this->getTag() == ENEMIES) {
		return _tag;
	}
	
	return -1;
}
