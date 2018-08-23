#include "PointPlusFX.h"

PointPlusFX::PointPlusFX(Vec2 position,int point, float size, Node* parent)
{
	this->autorelease();

	this->initWithTTF("+" + to_string(point), "fonts/Marker Felt.ttf", size);
	this->setPosition(position);
	parent->addChild(this, 999);

	CallFunc* remove = CallFunc::create(CC_CALLBACK_0(PointPlusFX::remove, this));
	auto moveup = MoveBy::create(1,Vec2(0, Director::getInstance()->getVisibleSize().height / 20));
	auto fadeout = FadeOut::create(0.5);
	this->runAction(Sequence::create(moveup, fadeout, remove, NULL));
}


PointPlusFX::~PointPlusFX()
{
	//delete this;
}

void PointPlusFX::remove() {
	this->removeFromParentAndCleanup(1);
}