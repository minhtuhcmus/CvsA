#include "Canon.h"
#include "AudioEngine.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "SimpleAudioEngine.h"

Canon::Canon(Vec2 position, float scale,int tag, Layer* parent)
{
	this->autorelease();

	// Start :: Init and set idle animation
	_tag = tag;
	this->init("items/Canon.png");
	this->setTag(CANON);
	this->setPosition(position);
	this->setName("Canon" + to_string(_tag));
	parent->addChild(this);

	ScaleTo* scaleTo1 = ScaleTo::create(0.5, 0.75);
	ScaleTo* scaleTo2 = ScaleTo::create(0.5, 0.7);

	this->runAction(RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, nullptr)));
	// End :: Init and set idle animation

	// Start :: Init Physicbody
	PhysicsBody* CanonBody = PhysicsBody::createCircle(this->getContentSize().width / 2);
	CanonBody->setGroup(CANON);
	CanonBody->setCategoryBitmask(CANON);
	CanonBody->setCollisionBitmask(0);
	CanonBody->setContactTestBitmask(BULLET);
	CanonBody->setDynamic(false);

	this->addComponent(CanonBody);

	auto colissionListener = EventListenerPhysicsContact::create();
	colissionListener->onContactBegin = CC_CALLBACK_1(Canon::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(colissionListener, this);

	this->addTouchEventListener(CC_CALLBACK_2(Canon::onTouch, this));
	// End :: Init Physicbody

	_shootingPoint = Sprite::create("player/bullet/ShootingPoint.png");
	_shootingPoint->setOpacity(0);
	_shootingPoint->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height * 0.5));
	this->addChild(_shootingPoint);

}


Canon::~Canon()
{
	//delete this;
}

bool Canon::onContactBegin(PhysicsContact& contact) {
	auto objectB = (Canon*)contact.getShapeB()->getBody()->getNode();
	auto objectA = (Canon*)contact.getShapeA()->getBody()->getNode();
	if (objectB == nullptr || objectA == nullptr) {
		return true;
	}

	Canon* other = nullptr;
	Canon* canon = nullptr;

	// Check if Alien hit
	if (objectA->getName() == "Canon" + to_string(objectA->getEnemyTag())) {
		canon = objectA;
		other = objectB;
	}
	else if (objectB->getName() == "Canon" + to_string(objectB->getEnemyTag())) {
		canon = objectB;
		other = objectA;
	}

	if (other == nullptr || canon == nullptr) return true;

	switch (other->getTag())
	{
	case BULLET:
		if (other->getName() == "Bullet") {
			canon->load(other);
		}
		break;
	default:
		break;
	}
	
	return true;
}

void Canon::load(Node* bullet) {
	// Play Canon explode sound
	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("boom.wav");
	//experimental::AudioEngine::play2d("boom.mp3");
	// If Canon's not exploded yet
	if (_isExploded == true) return;
	_isExploded = true;
	
	experimental::AudioEngine::play2d("click.mp3");
	_bullet = bullet;
	_bullet->getPhysicsBody()->setVelocity(Vec2::ZERO);
	_bullet->getPhysicsBody()->setEnabled(false);
	_bullet->setOpacity(0.0f);
	CallFunc* rotation = CallFunc::create([&] {
		_bullet->setPosition(_shootingPoint->convertToWorldSpaceAR(Vec2::ZERO));
	});
	RotateBy* rotate = RotateBy::create(0.0f, 45.0f);
	ScaleTo* delay = ScaleTo::create(_timeDelay, 1.0f);
	Sequence* action = Sequence::create(rotate, rotation, delay, nullptr);

	this->runAction(RepeatForever::create(action));

	_bgButton = ui::Button::create("UI/buttons/FullsceneButton.png");
	_bgButton->addTouchEventListener(CC_CALLBACK_2(Canon::onTouch, this));
	_bgButton->setScale(50);
	_bgButton->setOpacity(0);
	this->addChild(_bgButton);
}

int Canon::getEnemyTag() {
	return this->_tag;
}

void Canon::onTouch(Ref* sender, ui::Widget::TouchEventType type) {
	if (_bullet == nullptr) return;
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		experimental::AudioEngine::play2d("gunshot.mp3");
		// Stop Idle state and move to loaded state
		this->stopAllActions();
		float rotate = this->getRotation();
		int direction = (_shootingPoint->convertToWorldSpace(Vec2::ZERO).x - this->getPositionX() > 0) ? 1 : -1;
		_bullet->setOpacity(255);
		_bullet->setRotation(-rotate);
		_bullet->setPosition(_shootingPoint->convertToWorldSpaceAR(Vec2::ZERO));

		// Add force to bullet
		float rotationRadians = rotate * (PI / 180);
		float vX = cos(-rotationRadians) * BULLET_VELOCITY;
		float vY = sin(-rotationRadians) * BULLET_VELOCITY;
		_bullet->getPhysicsBody()->setVelocity(Vec2(vX, vY));
		_bullet->getPhysicsBody()->setEnabled(true);
		_bullet = nullptr;

		// Continue idle state
		_isExploded = false;
		ScaleTo* scaleTo1 = ScaleTo::create(0.5, 0.75);
		ScaleTo* scaleTo2 = ScaleTo::create(0.5, 0.7);
		this->runAction(RepeatForever::create(Sequence::create(scaleTo1, scaleTo2, nullptr)));
		_bgButton->removeFromParentAndCleanup(1);
	}
	default:
		break;
	}
}

