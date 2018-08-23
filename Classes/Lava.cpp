#include "Lava.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "Bullet.h"
#include "EnemyExplosion.h"

Lava::Lava(Vec2 position, float scale,int tag, Layer* parent)
{
	this->autorelease();

	// Start :: Init
	_tag = tag;
	//this->initWithFile("items/Lava.png");
	this->setPosition(position);
	this->setName("Lava" + to_string(_tag));
	parent->addChild(this, 0);
	// End :: Init

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(Lava::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);
}


Lava::~Lava()
{
	//delete this;
}

bool Lava::onContactBegin(PhysicsContact& contact) {
	auto objectB = (Lava*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (Lava*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Bullet* other = nullptr;
	Lava* lava = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "Lava" + to_string(objectA->getEnemyTag())) {
		lava = objectA;
		other = (Bullet*)objectB;
	}
	else if (objectB->getName() == "Lava" + to_string(objectB->getEnemyTag())) {
		lava = objectB;
		other = (Bullet*)objectA;
	}

	if (other == nullptr || lava == nullptr) return true;

	//MessageBox(to_string(other->getTag()).c_str(), "a");

	switch (other->getTag())
	{
	case BULLET:
		lava->explode(other);
		break;
	default:
		break;
	}

	return true;
}

// Call when Lava hit Bullet
void Lava::explode(Bullet* bullet) {
	if (bullet == nullptr) return;
	if (bullet->getName() != "RoastedChicken") {
	// Check if other is bullet
	if (bullet->getTag() == BULLET) {
		// Spawn FX
		FX* bloodHitFX = new FX(
			bullet->getPosition(),
			Vec2(0.5, 0.5),
			Director::getInstance()->getRunningScene(),
			"TNT_Explosion",
			0.05
		);
	}

	_fx_Pos = bullet->getPosition();
	_fx_Velocity = bullet->getPhysicsBody()->getVelocity();

	// Spawn roasted chicken
		_currentBullet = bullet->getName();
		CallFunc* roasted = CallFunc::create(CC_CALLBACK_0(Lava::createRoastedChicken, this));
		this->runAction(roasted);
	}
}

// Spawn roasted chicken
void Lava::createRoastedChicken() {
	// Play Chicken's Dead sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("roastedChicken.wav");
	experimental::AudioEngine::play2d("roastedChicken.mp3");
	// Spawn Roasted Chicken Sprite
	Sprite* roastedChicken = Sprite::create("items/Roasted_Chicken.png");
	roastedChicken->setTag(BULLET);
	roastedChicken->setName("RoastedChicken");
	

	roastedChicken->setPosition(_fx_Pos);
	this->getParent()->addChild(roastedChicken);

	// Add physicbody and force
	PhysicsBody* roastedChickenBody = PhysicsBody::createBox(roastedChicken->getContentSize() / 2);
	roastedChickenBody->setCategoryBitmask(BULLET);
	roastedChickenBody->setCollisionBitmask(WALL + ROCK + PAN + LAVA);
	roastedChickenBody->setContactTestBitmask(ENEMIES + TNT_DEFINE);
	roastedChicken->addComponent(roastedChickenBody);
	roastedChicken->getPhysicsBody()->setVelocity(_fx_Velocity);

	// Remove the bullet
	Bullet* bullet = (Bullet*)this->getParent()->getChildByName(_currentBullet);
	if (bullet != nullptr) {
		bullet->_trailRenderer->removeFromParentAndCleanup(1);
		bullet->removeFromParentAndCleanup(1);
	}
}


int Lava::getEnemyTag() {
	return this->_tag;
}
