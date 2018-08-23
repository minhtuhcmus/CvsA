#include "PlayerController.h"
#include "AudioEngine.h"
#include <math.h>
#include "Player.h"
#include "Gun.h"
#include "Pan.h"
#include "SimpleAudioEngine.h"


PlayerController::PlayerController(Node* parent)
{
	this->autorelease();

	_dots = _maxDots;

	_aim = DrawNode::create();
	_trajectory = DrawNode::create();
	this->addChild(_aim, -1);
	this->addChild(_trajectory, -1);
	this->setName("PlayerController");

	_cancelShot = Sprite::create("UI/buttons/CancelShot.png");
	_cancelShot->setOpacity(0);
	_cancelShot->setPosition(Vec2(0, Director::getInstance()->getVisibleSize().height));
	parent->addChild(_cancelShot);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PlayerController::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(PlayerController::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(PlayerController::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	parent->addChild(this);

	_fakeBullet = Node::create();

	PhysicsBody* bulletBody = PhysicsBody::createCircle(
		5.4f * 0.7f,
		PhysicsMaterial(0.0f, 1.0f, 0.0f)
	);
	bulletBody->setCategoryBitmask(FAKE_BULLET);
	bulletBody->setCollisionBitmask(WALL + ROCK + PAN);
	bulletBody->setGravityEnable(false);
	bulletBody->setRotationEnable(false);

	this->addChild(_fakeBullet);
	_fakeBullet->addComponent(bulletBody);
}


PlayerController::~PlayerController()
{
	//delete this;
}

void PlayerController::setPlayer(Player* player) {
	_player = player;
}

void PlayerController::update(float dt) {
	_aim->clear();
	Vec2 point;
	float shortestRaycast = 99999;
	auto func = [&point, &shortestRaycast](PhysicsWorld& world,
		const PhysicsRayCastInfo& info, void* data)->bool
	{
		switch (info.shape->getGroup())
		{
		case WALL:
		case PAN:
		case ROCK:
		case LAVA:
		case CANON:
		case CHAINSAW:
		case PORTAL:
			if (info.fraction < shortestRaycast) {
				point = info.contact;
				shortestRaycast = info.fraction;
			}
			break;
		default:
			break;
		}
		return true;
	};
	PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	//_aim->drawSegment(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), _player->_gun->_shootingPoint2->convertToWorldSpace(Vec2::ZERO), 2, Color4F::RED);
	world->rayCast(func, _player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), _player->_gun->_shootingPoint2->convertToWorldSpace(Vec2::ZERO), nullptr);
	_aim->drawSegment(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), point, 1.5f, Color4F::RED);
	_aim->drawDot(point, 3.0f, Color4F::WHITE);
}

bool PlayerController::onTouchBegan(Touch* touch, Event* event) {
	if (this->getParent()->getChildByTag(ITEMS_SELECTED)) return true;
	if (this->getParent()->getChildByName("Bullet")) return true;
	if (_player->getAmmo() == 0) return true;
	_cancelShot->setOpacity(150);

	Node* playedTrigger = Node::create();
	playedTrigger->setTag(PLAYED_TRIGGER);
	this->getParent()->addChild(playedTrigger);

	Vec2 distance = touch->getLocation() - _player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO);
	float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);

	float degrees = atanf(distance.y / distance.x) * 180.0f / PI;
	float currentScaleX = (distance.x > 0) ? abs(_player->getScaleX()) : -abs(_player->getScaleX());
	//_player->setScaleX(currentScaleX);
	if (currentScaleX > 0) {
		_player->setBodyPartsRotateion(-degrees);
	}
	else {
		degrees += 180;
		_player->setBodyPartsRotateion(-degrees);
	}
	this->scheduleUpdate();
	//_aim->clear();
	//Vec2 point;
	//float shortestRaycast = 99999;
	//auto func = [&point, &shortestRaycast](PhysicsWorld& world,
	//	const PhysicsRayCastInfo& info, void* data)->bool
	//{
	//	switch (info.shape->getGroup())
	//	{
	//	case WALL:
	//	case PAN:
	//	case ROCK:
	//	case LAVA:
	//	case CANON:
	//	case CHAINSAW:
	//	case PORTAL:
	//		if (info.fraction < shortestRaycast) {
	//			point = info.contact;
	//			shortestRaycast = info.fraction;
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//	return true;
	//};
	//PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	////_aim->drawSegment(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), _player->_gun->_shootingPoint2->convertToWorldSpace(Vec2::ZERO), 2, Color4F::RED);
	//world->rayCast(func, _player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), _player->_gun->_shootingPoint2->convertToWorldSpace(Vec2::ZERO), nullptr);
	//_aim->drawSegment(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), point, 1, Color4F::RED);
	//_aim->drawDot(point, 3.0f, Color4F::WHITE);

	_trajectory->clear();
	if (_isTrajectory == true) {
		simulateTrajectory();
	}

	return true;
}

void PlayerController::onTouchMoved(Touch* touch, Event* event) {
	if (this->getParent()->getChildByTag(ITEMS_SELECTED) != nullptr) return;
	if (this->getParent()->getChildByName("Bullet") != nullptr) return;
	if (touch->getLocation() == touch->getPreviousLocation()) return;
	if (_player->getAmmo() == 0) return;

	Vec2 distance = touch->getLocation() - _player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO);
	float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);

	float degrees = atanf(distance.y / distance.x) * 180.0f / PI;
	if (abs(_currentDegree - degrees) < 0.25f) return;
	_currentDegree = degrees;

	float currentScaleX = (distance.x > 0) ? abs(_player->getScaleX()) : -abs(_player->getScaleX());
	//_player->setScaleX(currentScaleX);
	if (currentScaleX > 0) {
		_player->setBodyPartsRotateion(-degrees);
	}
	else {
		degrees += 180;
		_player->setBodyPartsRotateion(-degrees);
	}

	//_aim->clear();
	//Vec2 point;
	//float shortestRaycast = 99999;
	//auto func = [&point, &shortestRaycast](PhysicsWorld& world,
	//	const PhysicsRayCastInfo& info, void* data)->bool
	//{
	//	switch (info.shape->getGroup())
	//	{
	//	case WALL:
	//	case PAN:
	//	case ROCK:
	//	case LAVA:
	//	case CANON:
	//	case CHAINSAW:
	//	case PORTAL:
	//		if (info.fraction < shortestRaycast) {
	//			point = info.contact;
	//			shortestRaycast = info.fraction;
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//	return true;
	//};
	//PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	////_aim->drawSegment(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), _player->_gun->_shootingPoint2->convertToWorldSpace(Vec2::ZERO), 2, Color4F::RED);
	//world->rayCast(func, _player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), _player->_gun->_shootingPoint2->convertToWorldSpace(Vec2::ZERO), nullptr);
	//_aim->drawSegment(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), point, 1, Color4F::RED);
	//_aim->drawDot(point, 3.0f, Color4F::WHITE);

	_trajectory->clear();
	if (_isTrajectory == true) {
		simulateTrajectory();
	}
}

void PlayerController::onTouchEnded(Touch* touch, Event* event) {
	if (_player->getAmmo() == 0) return;

	_cancelShot->setOpacity(0);
	this->stopAllActions();
	_aim->clear();
	_trajectory->clear();
	this->unscheduleUpdate();

	Vec2 distance = touch->getLocation() - _cancelShot->getPosition();
	float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);

	if (dist < _cancelShot->getContentSize().width / 2) {
		return;
	}

	if (this->getParent()->getChildByTag(ITEMS_SELECTED)) return;
	if (this->getParent()->getChildByName("Bullet")) return;

	//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gunshot.wav");
	experimental::AudioEngine::play2d("gunshot.mp3");
	_player->shoot();
}

void PlayerController::setTutorial(bool isTutorial) {
	_isTutorial = isTutorial;
}

void PlayerController::simulateTrajectory() {
	if (_fakeBullet == nullptr) return;
	_fakeBullet->getPhysicsBody()->setDynamic(true);
	_fakeBullet->setPosition(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO));
	previousDot = _fakeBullet->getPosition();
	this->stopAllActions();
	_dots = _maxDots;

	float rotationRadians = _player->_gun->getRotation() * (PI / 180);
	int currentPlayerScale = (_player->getScaleX() > 0) ? 1 : -1;
	float vX = currentPlayerScale * cos(-rotationRadians * currentPlayerScale) * 2000;
	float vY = currentPlayerScale * sin(-rotationRadians * currentPlayerScale) * 2000;
	_fakeBullet->getPhysicsBody()->setVelocity(Vec2(vX, vY));

	//PhysicsWorld* world = Director::getInstance()->getRunningScene()->getPhysicsWorld();
	//for (int i = 1; i <= 35; i++) {
	//	world->step(1/100.0f);
	//	_aim->drawDot(_fakeBullet->getPosition(), 3.0f, Color4F::WHITE);
	//}

	CallFunc* drawDots = CallFunc::create([&] {
		if (_dots > 0) {
			_dots--;
			Vec2 currentDot = _fakeBullet->getPosition();
			//_aim->drawDot(previousDot + (currentDot - previousDot) / 2, 3.0f, Color4F::WHITE);
			//previousDot = currentDot;
			_trajectory->drawDot(currentDot, 3.0f, Color4F::WHITE);
		}
		else {
			_fakeBullet->getPhysicsBody()->setDynamic(false);
		}
	});

	ScaleTo* delay = ScaleTo::create(0.00000001f, 1.0f);

	this->runAction(RepeatForever::create(Sequence::create(drawDots, delay, nullptr)));

	//_fakeBullet->getPhysicsBody()->setDynamic(false);
}

void PlayerController::hint(float angle) {
	int currentPlayerScale = (angle < 90 && angle > -90) ? 1 : -1;
	int rotate = (angle < 90 && angle > -90)? angle : 180 - angle;
	_player->setBodyPartsRotateion(-1 * currentPlayerScale *rotate);

	DrawNode* hintDraw = DrawNode::create();
	hintDraw->setName("Hint");
	hintDraw->setOpacity(150);
	this->getParent()->addChild(hintDraw);

	Vec2 shootingPoint = _player->_gun->_shootingPoint->getPosition();

	for (int i = 1; i <= 35; i++) {
		_player->_gun->_shootingPoint->setPosition(_player->_gun->_shootingPoint->getPosition() + Vec2(i * 3, 0));
		hintDraw->drawDot(_player->_gun->_shootingPoint->convertToWorldSpace(Vec2::ZERO), 3.0f, Color4F::WHITE);
	}

	_player->_gun->_shootingPoint->setPosition(shootingPoint);
}

void PlayerController::setEnableTrajectory(bool isEnable) {
	_isTrajectory = isEnable;
}