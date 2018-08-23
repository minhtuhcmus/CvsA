#include "ChickenEater.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "Enemies.h"
#include "SimpleAudioEngine.h"

ChickenEater::ChickenEater(Vec2 position, float scale,int tag, Enemies* enemiesHandler, Layer* parent)
{
	this->autorelease();

	//// Start :: Init
	_tag = tag;
	_enemiesHandler = enemiesHandler;
	_enemiesHandler->addEnemy();

	this->initWithFile("enemies/ChickenEater.png");

	this->setScale(scale, scale);
	this->setPosition(position);
	this->setName("ChickenEater" + to_string(_tag));
	this->setTag(ENEMIES);
	parent->addChild(this);
	this->setAnchorPoint(Vec2(0.5, 0));

	//// End :: Init

	//// Start :: init physicbody and hitbox
	PhysicsBody* chickeneaterBody = PhysicsBody::createBox(
		this->getContentSize() * 0.7f
	);
	chickeneaterBody->setCategoryBitmask(ENEMIES);
	chickeneaterBody->setCollisionBitmask(0);
	chickeneaterBody->setContactTestBitmask(BULLET + ROCK + TNT_DEFINE);
	chickeneaterBody->setDynamic(false);
	this->addComponent(chickeneaterBody);

	auto player = parent->getChildByName("Player");

	// Flip X if ChickenEater stand on the rightside of player
	if (this->getPositionX() - player->getPositionX() > 0) {
		this->setScaleX(-1 * this->getScaleX());
	}
	else {
		this->setScaleX(1 * this->getScaleX());
	}

	// Set idle animation
	Sequence* idle = Sequence::create(ScaleTo::create(0.5f, this->getScaleX(), 1.1f), ScaleTo::create(0.5f, this->getScaleX(), 1.0f), nullptr);
	this->runAction(RepeatForever::create(idle));

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(ChickenEater::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);
	//// End :: init physicbody and hitbox

	_scene = parent;
}


ChickenEater::~ChickenEater()
{
	//delete this;
}

void ChickenEater::destroy() {
	// Play ChickenEater dead sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ChickenEater.wav");
	experimental::AudioEngine::play2d("alien.mp3");
	// Spawn FX
	FX* bloodHitFX = new FX(
		this->getPosition(),
		Vec2(0.5, 0.5),
		Director::getInstance()->getRunningScene(),
		"Blood_Hit",
		0.03
	);

	bloodHitFX->runAction(TintTo::create(0.0f, Color3B::RED));

	// Spawn ChickenEaters explode FX
	EnemyExplosion* explode = new EnemyExplosion("CE_Body_Parts", this->getPosition(), 1.0f, _scene);

	// Remove ChickenEater
	this->removeFromParentAndCleanup(1);
}

bool ChickenEater::onContactBegin(PhysicsContact& contact) {
	// Get head or body of a alien
	auto objectB = (ChickenEater*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (ChickenEater*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Node* other = nullptr;
	ChickenEater* alien = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "ChickenEater" + to_string(objectA->getEnemyTag())) {
		alien = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "ChickenEater" + to_string(objectB->getEnemyTag())) {
		alien = objectB;
		other = objectA;
	}

	if (other == nullptr) return true;

	switch (other->getTag())
	{
	case BULLET: {
		other->removeFromParentAndCleanup(1);
		alien->stopAllActions();
		Sequence* eat = Sequence::create(ScaleTo::create(0.0f, alien->getScaleX(), 1.3f), ScaleTo::create(0.05f, alien->getScaleX(), 0.8f), ScaleTo::create(0.5f, alien->getScaleX(), 1.0f), nullptr);
		alien->runAction(eat);
		break;
	}
	case ROCK: {
		_enemiesHandler->removeEnemy(alien->getName());
		alien->destroy();
		break;
	}
	default:
		break;
	}
	return true;
}

int ChickenEater::getEnemyTag() {
	if (this->getTag() == ENEMIES) {
		return _tag;
	}
	
	return -1;
}
