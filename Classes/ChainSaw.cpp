#include "ChainSaw.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "Bullet.h"
#include "EnemyExplosion.h"

ChainSaw::ChainSaw(Vec2 position, float scale,int tag, Layer* parent)
{
	this->autorelease();

	// Start :: Init
	_tag = tag;
	_scene = parent;

	this->initWithFile("items/ChainSaw.png");
	this->setPosition(position);
	this->setName("ChainSaw" + to_string(_tag));
	parent->addChild(this);

	ScaleTo* scaleTo1 = ScaleTo::create(1.0f, 1.05f);
	ScaleTo* scaleTo2 = ScaleTo::create(1.0f, 1.0f);
	RotateBy* rotate = RotateBy::create(0.1f, 20.0f);

	this->runAction(RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, nullptr)));
	this->runAction(RepeatForever::create(rotate));
	// End :: Init

	PhysicsBody* chainsawBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
	chainsawBody->setGroup(CHAINSAW);
	chainsawBody->setCategoryBitmask(CHAINSAW);
	chainsawBody->setCollisionBitmask(0);
	chainsawBody->setContactTestBitmask(BULLET);
	chainsawBody->setDynamic(false);

	this->addComponent(chainsawBody);

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(ChainSaw::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);
}


ChainSaw::~ChainSaw()
{
	//delete this;
}

bool ChainSaw::onContactBegin(PhysicsContact& contact) {
	auto objectB = (Bullet*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (Bullet*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	// If ChainSaw hit bullet
	if (objectA->getName() == "Bullet") {
		if (objectB->getName() == "ChainSaw" + to_string(_tag)) {
			explode(objectA);
		}
	}
	else if (objectB->getName() == "Bullet") {
		if (objectA->getName() == "ChainSaw" + to_string(_tag)) {
		explode(objectB);
		}
	}

	return true;
}

// Call when ChainSaw hit Bullet
void ChainSaw::explode(Bullet* bullet) {
	// Check if other is bullet
	if (bullet->getName() == "Bullet") {
		// Spawn FX
		FX* bloodHitFX = new FX(
			bullet->getPosition(),
			Vec2(0.5, 0.5),
			Director::getInstance()->getRunningScene(),
			"Bullet_Wall_Contact",
			0.05
		);
	}

	// Spawn splited chicken
	CallFunc* plit = CallFunc::create(CC_CALLBACK_0(ChainSaw::createSplitedChicken, this));
	this->runAction(plit);
}

// Spawn splited chicken
void ChainSaw::createSplitedChicken() {
	// Play Chicken's Dead sound
	experimental::AudioEngine::play2d("roastedChicken.mp3");
	// Spawn splited Chicken Sprite
	Bullet* bullet = (Bullet*)this->getParent()->getChildByName("Bullet");
	int direction = (bullet->getPhysicsBody()->getVelocity().x > 0) ? 1 : -1;
	float rotate = atanf(bullet->getPhysicsBody()->getVelocity().y / bullet->getPhysicsBody()->getVelocity().x) * 180.0f / PI;
	for (int i = 0; i < 2; i++) {
		Bullet* newBullet = new Bullet(direction * 0.5f, -1 * (rotate - 45.0f + 90 * i), bullet->getPosition(), _scene);
		newBullet->setName("splitedChicken" + to_string(i));
		newBullet->setDisableHitCounter();
	}

	Bullet* newBullet = new Bullet(direction * 0.5f, -1 * rotate, bullet->getPosition(), _scene);
	newBullet->setName("splitedChicken" + to_string(2));

	// Remove the bullet
	bullet->_trailRenderer->removeFromParentAndCleanup(1);
	bullet->removeFromParentAndCleanup(1);
	this->removeFromParentAndCleanup(1);
}


int ChainSaw::getEnemyTag() {
	return this->_tag;
}
