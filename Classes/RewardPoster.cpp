#include "RewardPoster.h"

RewardPoster::RewardPoster(Node* parent)
{
	this->autorelease();

	this->initWithFile("UI/rewards/Mission_Completed.png");
	this->setScale(0.0);
	this->setPosition(Director::getInstance()->getVisibleSize() / 2 + Size(0,15));
	parent->addChild(this);

	auto fadeIn = FadeIn::create(1);
	auto scaleTo = ScaleTo::create(1, 0.9f);
	auto rotateBy = RotateBy::create(1, 3600);

	CallFunc* firework = CallFunc::create(CC_CALLBACK_0(RewardPoster::firework, this));

	this->runAction(scaleTo);
	this->runAction(fadeIn);
	this->runAction(Sequence::create(rotateBy, firework, nullptr));
}


RewardPoster::~RewardPoster()
{
	//delete this;
}

void RewardPoster::firework() {
	//FX* firework = new FX(
	//	this->getPosition(),
	//	Vec2(0.5, 0),
	//	Director::getInstance()->getRunningScene(),
	//	"Firework",
	//	0.05f
	//);

	//firework->setScale(3);
}