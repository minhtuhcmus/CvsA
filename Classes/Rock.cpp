#include "Rock.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"

Rock::Rock(Vec2 position, float scale,int tag, Layer* parrent)
{
	this->autorelease();

	// Start :: Init
	_tag = tag;
	this->initWithFile("items/Rock.png");
	this->setTag(ROCK);
	this->setPosition(position);
	this->setName("Rock" + to_string(_tag));
	parrent->addChild(this);
	// End :: Init

	// Start :: Init Physicbody
	_rockBody = PhysicsBody::createCircle(this->getContentSize().width/2, PhysicsMaterial(1.0f,1.0f,1.0f));
	_rockBody->setGroup(ROCK);
	_rockBody->setCategoryBitmask(ROCK);
	_rockBody->setCollisionBitmask(WALL + BULLET + ROCK + FAKE_BULLET);
	_rockBody->setContactTestBitmask(ENEMIES + TNT_EXPLODE + BULLET);
	_rockBody->setDynamic(false);

	this->addComponent(_rockBody);

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(Rock::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);

	// End :: Init Physicbody
}


Rock::~Rock()
{
	//delete this;
}

bool Rock::onContactBegin(PhysicsContact& contact) {
	if (_isHit) return true;
	auto objectB = contact.getShapeB()->getBody()->getNode();
	auto objectA = contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	int otherTag = 0;
	CallFunc* explode = CallFunc::create(CC_CALLBACK_0(Rock::explode, this));
	// Check the right rock
	if (objectA->getName() == "Rock" + to_string(_tag)) { 
		otherTag = objectB->getTag();
	}
	else if (objectB->getName() == "Rock" + to_string(_tag)) {
		otherTag = objectA->getTag();
	}

	if (otherTag) {
		switch (otherTag)
		{
		case TNT_EXPLODE: // If Rock hit TNT
			this->runAction(explode);
			_isHit = true;
			break;
		case BULLET: // If Rock hit Bullet
			this->runAction(Sequence::create(ScaleTo::create(0.2f, 0.9f), ScaleTo::create(0.2f, 1.0f), nullptr));
			break;
		default:
			break;
		}
	}

	return true;
}

// When TNT hit Rock => Rock explode and roll
void Rock::explode() {
	// Remove static Physicbody
	this->removeAllComponents();

	// Add new dynamic Physicbody
	_rockBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
	_rockBody->setGroup(ROCK);
	_rockBody->setCategoryBitmask(ROCK);
	_rockBody->setCollisionBitmask(TNT_DEFINE + WALL);
	_rockBody->setContactTestBitmask(ENEMIES);
	this->addComponent(_rockBody);
}

int Rock::getEnemyTag() {
	return this->_tag;
}
