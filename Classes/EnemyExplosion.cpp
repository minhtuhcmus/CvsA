#include "EnemyExplosion.h"
#include <time.h> 

EnemyExplosion::EnemyExplosion(string enemyName, Vec2 position, float scale, Node* parent)
{
	this->autorelease();

	_enemyName = enemyName;

	this->setName("Enemy_Parts");

	// Remove FX after a while
	CallFunc* remove = CallFunc::create(CC_CALLBACK_0(EnemyExplosion::remove, this));
	ScaleTo* delay = ScaleTo::create(6.0f, scale);
	this->runAction(Sequence::create(delay, remove, NULL));

	// Spawn the FX
	CallFunc* explode = CallFunc::create(CC_CALLBACK_0(EnemyExplosion::explode, this));
	this->runAction(explode);

	this->setScale(scale);
	this->setPosition(position);
	parent->addChild(this);
}


EnemyExplosion::~EnemyExplosion()
{
	//delete this;
}

void EnemyExplosion::remove() {
	this->removeFromParentAndCleanup(1);
}

void EnemyExplosion::explode() {
	// Create random x:y for forcing
	srand(time(NULL));
	Vec2 randValue = Vec2(random(-10, 10), random(-10,10));

	// Read sprites
	string str = _enemyName + "_%02d.png";
	Vector<SpriteFrame*> bodyParts_Frames = getAnimation(str.c_str());

	// Delay 5s before blinking
	ScaleTo* delay = ScaleTo::create(5.0f, 1);
	for (int i = 0; i < bodyParts_Frames.size(); i++) {
		Sprite* part = Sprite::createWithSpriteFrame(bodyParts_Frames.at(i));
		part->runAction(Sequence::create(
			delay,
			FadeOut::create(0.2f),
			FadeIn::create(0.2f),
			FadeOut::create(0.2f),
			FadeIn::create(0.2f),
			FadeOut::create(0.2f),
			FadeIn::create(0.2f),
			FadeOut::create(0.2f),
			FadeIn::create(0.2f),
			FadeOut::create(0.2f),
			FadeIn::create(0.2f),
			NULL
		));
		this->addChild(part);

		// Add physicbody
		PhysicsBody* partBody = PhysicsBody::createCircle(part->getContentSize().width / 2);
		partBody->setCategoryBitmask(ENEMIES_PARTS);
		partBody->setCollisionBitmask(WALL);
		part->addComponent(partBody);
		part->getPhysicsBody()->setLinearDamping(1.0f);
		part->getPhysicsBody()->applyImpulse(Vec2(randValue.x * _explosionForce, randValue.y * _explosionForce));
	}
}