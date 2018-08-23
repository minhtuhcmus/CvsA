#include "AnoyingFly.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "Enemies.h"
#include "SimpleAudioEngine.h"

AnoyingFly::AnoyingFly(Vec2 position, float scale,int tag, Enemies* enemiesHandler, Layer* parent)
{
	this->autorelease();

	_scene = parent;
	_a_Pos = position;

	//// Start :: Init and set idle animation
	_tag = tag;
	_enemiesHandler = enemiesHandler;
	_enemiesHandler->addEnemy();

	this->initWithSpriteFrame(getAnimation("AnoyingFly_%02d.png").at(0));

	this->setScale(scale, scale);
	this->setPosition(_a_Pos);
	this->setName("AnoyingFly" + to_string(_tag));
	this->setTag(ENEMIES);
	parent->addChild(this);

	Animation* animation = Animation::createWithSpriteFrames(getAnimation("AnoyingFly_%02d.png"), 0.2f);
	Animate* animate = Animate::create(animation);
	this->runAction(RepeatForever::create(animate));
	//// End :: Init and set idle animation

	//// Start :: init physicbody and hitbox
	PhysicsBody* AnoyingFlyBody = PhysicsBody::createBox(
		this->getContentSize()
	);
	AnoyingFlyBody->setCategoryBitmask(ENEMIES);
	AnoyingFlyBody->setCollisionBitmask(0);
	AnoyingFlyBody->setContactTestBitmask(BULLET + TNT_EXPLODE + ROCK);
	AnoyingFlyBody->setDynamic(false);
	this->addComponent(AnoyingFlyBody);

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(AnoyingFly::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);
	//// End :: init physicbody and hitbox
}


AnoyingFly::~AnoyingFly()
{
	//delete this;
}

void AnoyingFly::destroy() {
	// Play AnoyingFly dead sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("AnoyingFly.wav");
	experimental::AudioEngine::play2d("AnoyingFly.mp3");
	// Spawn FX
	FX* bloodHitFX = new FX(
		this->getPosition(),
		Vec2(0.5, 0.5),
		Director::getInstance()->getRunningScene(),
		"Blood_Hit",
		0.03
	);

	bloodHitFX->runAction(TintTo::create(0.0f, Color3B::MAGENTA));

	// Spawn AnoyingFlys explode FX
	EnemyExplosion* explode = new EnemyExplosion("AF_Body_Parts", this->getPosition(), 1.0f, _scene);

	// Remove AnoyingFly
	this->removeFromParentAndCleanup(1);
}

bool AnoyingFly::onContactBegin(PhysicsContact& contact) {
	// Get head or body of a AnoyingFly
	auto objectB = (AnoyingFly*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (AnoyingFly*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	AnoyingFly* other = nullptr;
	AnoyingFly* anoyingFly = nullptr;

	// Check if AnoyingFly hit
	if (objectA->getName() == "AnoyingFly" + to_string(objectA->getEnemyTag())) {
		anoyingFly = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "AnoyingFly" + to_string(objectB->getEnemyTag())) {
		anoyingFly = objectB;
		other = objectA;
	}

	if (other == nullptr || anoyingFly == nullptr) return true;

	switch (other->getTag())
	{
	case BULLET:
	case ROCK:
	case TNT_EXPLODE:
		_enemiesHandler->removeEnemy(anoyingFly->getName());
		anoyingFly->destroy();
		break;
	default:
		break;
	}

	return true;
}

int AnoyingFly::getEnemyTag() {
	if (this->getTag() == ENEMIES) {
		return _tag;
	}
	
	return -1;
}

void AnoyingFly::setMoveToPos(Vec2 position) {
	_b_Pos = position;
	Vec2 distance = _b_Pos - _a_Pos;
	float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
	float timeToMove = dist / _speed;

	float rotation = (distance.x > 0)? 1.0f : -1.0f;

	MoveTo* moveA_B = MoveTo::create(timeToMove, _b_Pos);
	MoveTo* moveB_A = MoveTo::create(timeToMove, _a_Pos);
	ScaleTo* rotateLeft = ScaleTo::create(0.0f, rotation, 1.0f);
	ScaleTo* rotateRight = ScaleTo::create(0.0f, -rotation, 1.0f);

	Sequence* action = Sequence::create(rotateRight, moveA_B, rotateLeft, moveB_A, nullptr);
	this->runAction(RepeatForever::create(action));
}
