#include "FX.h"

FX::FX(Vec2 position, Vec2 anchorPoint, Node* parent, string key, float time)
{
	this->autorelease();

	this->setPosition(position);
	this->setAnchorPoint(anchorPoint);
	Vector<SpriteFrame*> fx = getAnimation((key + "_%02d.png").c_str());
	this->initWithSpriteFrame(getSprite((key + "_01.png").c_str()));
	Animation* animation = Animation::createWithSpriteFrames(fx, time);
	Animate* animate = Animate::create(animation);
	CallFunc* remove = CallFunc::create(CC_CALLBACK_0(FX::remove, this));
	this->runAction(Sequence::create(animate, remove, NULL));

	parent->addChild(this, 100);
}


FX::~FX()
{
	//delete this;
}

void FX::remove() {
	this->removeFromParentAndCleanup(1);
}