#include "FailPoster.h"

FailPoster::FailPoster(Node* parent)
{
	this->autorelease();

	this->initWithFile("UI/rewards/FailPoster.png");
	this->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->setOpacity(0);
	parent->addChild(this, 0);

	auto fadeIn = FadeTo::create(2,210);

	this->runAction(fadeIn);
}


FailPoster::~FailPoster()
{
	//delete this;
}