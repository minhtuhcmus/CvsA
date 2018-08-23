#include "MovingGround.h"

MovingGround::MovingGround(Vec2 position, float scale,int tag, Layer* parent)
{
	this->autorelease();

	_scene = parent;
	_a_Pos = position;

	//// Start :: Init and set idle animation
	_tag = tag;

	this->initWithFile("items/MovingGround.png");
	this->setScale(scale, scale);
	this->setPosition(_a_Pos);
	this->setAnchorPoint(Vec2(0.5f, 1.0f));
	this->setName("MovingGround" + to_string(_tag));
	this->setTag(WALL);
	parent->addChild(this);
	//// End :: Init and set idle animation

	//// Start :: init physicbody and hitbox
	PhysicsBody* movingGroundBody = PhysicsBody::createBox(
		Size(this->getContentSize().width, this->getContentSize().height * 0.3f),
		PhysicsMaterial(0.0f, 1.0f, 0.0f),
		Vec2(0.0f, this->getContentSize().height * 0.3f)
	);
	movingGroundBody->setGroup(WALL);
	movingGroundBody->setCategoryBitmask(WALL);
	movingGroundBody->setCollisionBitmask(BULLET + ENEMIES_PARTS + ROCK + FAKE_BULLET);
	movingGroundBody->setContactTestBitmask(BULLET);
	movingGroundBody->setDynamic(false);
	this->addComponent(movingGroundBody);
	//// End :: init physicbody and hitbox
}


MovingGround::~MovingGround()
{
	//delete this;
}

void MovingGround::setMoveToPos(Vec2 position) {
	_b_Pos = position;
	Vec2 distance = _b_Pos - _a_Pos;
	float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
	float timeToMove = dist / _speed;

	if (abs(distance.x) > 10.0f) {
		_isHorizontal = true;
	}
	if (abs(distance.y) > 10.0f) {
		_isVertical = true;
	}

	MoveTo* moveA_B = MoveTo::create(timeToMove, _b_Pos);
	MoveTo* moveB_A = MoveTo::create(timeToMove, _a_Pos);

	Sequence* action = Sequence::create(moveA_B, moveB_A, nullptr);
	this->runAction(RepeatForever::create(action));
}

void MovingGround::setFaceTo(Vec2 position) {
	Vec2 fromThisToPoint = position - this->getPosition();

	if (_isVertical) {
		if (fromThisToPoint.x > 0) {
			this->setRotation(90);
		}
		else {
			this->setRotation(-90);
		}
	}
	else if (_isHorizontal) {
		if (fromThisToPoint.y > 0) {
			this->setRotation(0);
		}
		else {
			this->setRotation(180);
		}
	}
}