#include "Bullet.h"
#include "AudioEngine.h"
#include "FX.h"
#include "Ammo.h"
#include <math.h>
#include <SimpleAudioEngine.h>


Bullet::Bullet(float direction, float rotate, Vec2 position, Node* scene)
{
	this->autorelease();

	// Start :: Init Sprite
	this->initWithFile("player/bullet/Chicken.png");
	this->setName("Bullet");
	this->setTag(BULLET);
	this->setScale(direction * _scale);
	this->setRotation(rotate);
	this->setPosition(position);
	// End :: Init Sprite

	// Start :: Init Physicbody
	PhysicsBody* bulletBody = PhysicsBody::createCircle(
		this->getContentSize().width * 0.1f,
		PhysicsMaterial(0.0f, 1.0f, 0.0f)
	);
	bulletBody->setCategoryBitmask(BULLET);
	bulletBody->setCollisionBitmask(WALL + ROCK + PAN);
	bulletBody->setContactTestBitmask(WALL + ENEMIES + TNT_DEFINE + PAN + ROCK + LAVA + CANON + CHAINSAW + PORTAL);
	bulletBody->setGravityEnable(false);
	bulletBody->setRotationEnable(false);
	this->addComponent(bulletBody);

	// Add force to bullet
	float rotationRadians = rotate * (PI / 180);
	float vX = direction * cos(-rotationRadians) * BULLET_VELOCITY;
	float vY = direction * sin(-rotationRadians) * BULLET_VELOCITY;
	this->getPhysicsBody()->setVelocity(Vec2(vX, vY));

	auto conlissionListener = EventListenerPhysicsContact::create();
	conlissionListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);
	conlissionListener->onContactSeparate = CC_CALLBACK_1(Bullet::onContactSeparate, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(conlissionListener, this);
	// End :: Init Physicbody

	// Start :: Create bullet's trail
	_trailRenderer = MotionStreak::create(1,0,10 * _scale, Color3B(255,255,255) , "player/bullet/Trail.png");
	scene->addChild(_trailRenderer);
	scene->addChild(this);
	// End:: Create bullet's trail

	// Start :: Display hit counter
	if (scene->getChildByName("Hitcounter") != nullptr) {
		scene->getChildByName("Hitcounter")->removeFromParentAndCleanup(1);
		scene->getChildByName("Hitcounter_BG")->removeFromParentAndCleanup(1);
	}
	_bulletCountDisplay_BG = Sprite::create("player/bullet/Chicken.png");
	_bulletCountDisplay_BG->setName("Hitcounter_BG");
	_bulletCountDisplay_BG->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.20, Director::getInstance()->getVisibleSize().height * 0.9));
	_bulletCountDisplay_BG->setAnchorPoint(Vec2(0.5f, 0.4f));
	_bulletCountDisplay_BG->setColor(Color3B::BLACK);
	scene->addChild(_bulletCountDisplay_BG);
	_bulletCountDisplay = Label::createWithTTF(to_string(_hitCounter), "fonts/Marker Felt.ttf", 25);
	_bulletCountDisplay->setName("Hitcounter");
	_bulletCountDisplay->setPosition(_bulletCountDisplay_BG->getPosition());
	scene->addChild(_bulletCountDisplay);

	// End :: Display hit counter

	this->scheduleUpdate();

	_scene = scene;
}

Bullet::~Bullet()
{
}

void Bullet::update(float delta) {
	_trailRenderer->setPosition(this->getPosition());
}

void Bullet::setAmmo(Ammo* ammo) {
	_ammo = ammo;
}

bool Bullet::onContactBegin(PhysicsContact& contact) {
	auto objectB = contact.getShapeB()->getBody()->getNode();
	auto objectA = contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Node* other = nullptr;
	Bullet* bullet = nullptr;

	// Find bullet and other
	if (objectA->getTag() == BULLET) {
		bullet = (Bullet*)objectA;
		other = objectB;
	}
	else if (objectB->getTag() == BULLET) {
		bullet = (Bullet*)objectB;
		other = objectA;
	}

	// If there's not bullet return
	if (other == nullptr || bullet == nullptr) return true;

	switch (other->getTag())
	{
	case WALL:
	case ROCK: {
		// Play Sound
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("wall.wav");
		experimental::AudioEngine::play2d("wall.mp3");
		FX* bullethitWall = new FX(
			bullet->getPosition(),
			Vec2(0.5, 0.5),
			Director::getInstance()->getRunningScene(),
			"Bullet_Wall_Contact",
			0.05
		);

		// Decrease hit counter
		if (other != _currentHitTarget) {
			_currentHitTarget = other;
			_hitCounter--;
			// Update hit counter display
			if (this->getTag() == BULLET) {
				if (_bulletCountDisplay != nullptr) {
					_bulletCountDisplay->setString(to_string(_hitCounter));
				}
			}
		}
		// If out of hits, remove
		if (_hitCounter < 1) {
			remove();
		}
		break;
	}
	case PAN: {
		_currentHitTarget = other;
		// Player sound
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pan.wav");
		experimental::AudioEngine::play2d("pan.mp3");
		// Spawn FX
		FX* bullethitWall = new FX(
			bullet->getPosition(),
			Vec2(0.5, 0.5),
			Director::getInstance()->getRunningScene(),
			"Bullet_Wall_Contact",
			0.05
		);
		break;
	}
	default:
		break;
	}

	return true;
}

void Bullet::onContactSeparate(PhysicsContact& contact) {
	Vec2 velocity = this->getPhysicsBody()->getVelocity();
	float rotate = atanf(velocity.y / velocity.x);

	if (velocity.x > 0) {
		this->setScaleX(abs(this->getScaleX()));
	}
	else {
		this->setScaleX(-abs(this->getScaleX()));
	}
}

void Bullet::remove() {
	if (_isRemoved == false) {
		_isRemoved = true;

		CallFunc* createRoastedChicken = CallFunc::create([&] {
			// Play Chicken's Dead sound
			experimental::AudioEngine::play2d("roastedChicken.mp3");

			// Spawn Roasted Chicken Sprite
			Sprite* roastedChicken = Sprite::create("items/Knocked_Chicken.png");
			roastedChicken->setScale(this->getScaleX());
			roastedChicken->setName("KnockedChicken");
			roastedChicken->setTag(BULLET);
			roastedChicken->setPosition(this->getPosition());
			_scene->addChild(roastedChicken);

			// Add physicbody and force
			PhysicsBody* roastedChickenBody = PhysicsBody::createBox(roastedChicken->getContentSize() / 2);
			roastedChickenBody->setCategoryBitmask(BULLET);
			roastedChickenBody->setCollisionBitmask(WALL + ROCK + PAN + LAVA);
			roastedChickenBody->setContactTestBitmask(ENEMIES + TNT_DEFINE);
			roastedChicken->addComponent(roastedChickenBody);
			roastedChicken->getPhysicsBody()->setVelocity(this->getPhysicsBody()->getVelocity());
		});

		CallFunc* deleteBullet = CallFunc::create([&] {
			_trailRenderer->removeFromParentAndCleanup(1);
			this->removeFromParentAndCleanup(1);
		});

		this->runAction(Sequence::create(createRoastedChicken, deleteBullet, nullptr));
	}
}

int Bullet::getHit() {
	return _hitCounter;
}

void Bullet::setDisableHitCounter() {
	_bulletCountDisplay = nullptr;
}
