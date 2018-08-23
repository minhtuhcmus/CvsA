#include "TNT.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "SimpleAudioEngine.h"

TNT::TNT(Vec2 position, float scale,int tag, Layer* parent)
{
	this->autorelease();

	// Start :: Init and set idle animation
	_tag = tag;
	this->initWithFile("items/TNT.png");
	this->setTag(TNT_DEFINE);
	this->setPosition(position);
	this->setName("TNT" + to_string(_tag));
	parent->addChild(this);
	this->setAnchorPoint(Vec2(0.5, 0));

	ScaleTo* scaleTo1 = ScaleTo::create(0.5, 0.55);
	ScaleTo* scaleTo2 = ScaleTo::create(0.5, 0.5);

	this->runAction(RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, nullptr)));
	// End :: Init and set idle animation

	// Start :: Init Physicbody
	PhysicsBody* tntBody = PhysicsBody::createBox(this->getContentSize());
	tntBody->setCategoryBitmask(TNT_DEFINE);
	tntBody->setCollisionBitmask(0);
	tntBody->setContactTestBitmask(BULLET + TNT_EXPLODE);
	tntBody->setDynamic(false);

	this->addComponent(tntBody);

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(TNT::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);

	// End :: Init Physicbody
}


TNT::~TNT()
{
	//delete this;
}

bool TNT::onContactBegin(PhysicsContact& contact) {
	auto objectB = (TNT*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (TNT*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	TNT* other = nullptr;
	TNT* tnt = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "TNT" + to_string(objectA->getEnemyTag())) {
		tnt = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "TNT" + to_string(objectB->getEnemyTag())) {
		tnt = objectB;
		other = objectA;
	}

	if (other == nullptr || tnt == nullptr) return true;

	//MessageBox(to_string(other->getTag()).c_str(), "a");

	switch (other->getTag())
	{
	case BULLET:
	case ROCK:
		tnt->explode();
		break;
	case TNT_DEFINE:
		tnt->explode();
		other->explode();
		break;
	default:
		break;
	}
	
	return true;
}

void TNT::explode() {
	// Play TNT explode sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom.wav");
	experimental::AudioEngine::play2d("boom.mp3");
	// If TNT's not exploded yet
	if (_isExploded == false) {
		// Set exploded
		_isExploded = true;

		this->setTag(TNT_EXPLODE);

		// Spawn FX
		FX* explosionFX = new FX(
			this->getPosition(),
			Vec2(0.5, 0.5),
			Director::getInstance()->getRunningScene(),
			"TNT_Explosion",
			0.05
		);
		explosionFX->setScale(this->getScale() * 6);

		// Set physic contact
		this->getPhysicsBody()->setCategoryBitmask(TNT_EXPLODE);
		this->getPhysicsBody()->setCollisionBitmask(ROCK);
		this->getPhysicsBody()->setContactTestBitmask(ENEMIES + ROCK + TNT_DEFINE);

		// Set visible and top idling
		this->stopAllActions();
		this->setOpacity(0);
		this->setAnchorPoint(Vec2(0.5f, 0.5f));

		// Scale the explosion and remove
		ScaleTo* explosion = ScaleTo::create(0.1f, 6);
		ScaleTo* delay = ScaleTo::create(0.5f, 6);
		CallFunc* remove = CallFunc::create([&] {this->removeFromParentAndCleanup(1); });
		this->runAction(Sequence::create(explosion, delay, remove, nullptr));
	}
}

int TNT::getEnemyTag() {
	return this->_tag;
}
