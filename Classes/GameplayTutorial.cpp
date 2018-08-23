#include "GameplayTutorial.h"

GameplayTutorial::GameplayTutorial(Vec2 positionFrom, Vec2 positionTo, bool isDeleteOnBegin, ui::Button* nextObjTutorial, Node* parent)
{
	//this->retain();
	this->autorelease();

	_isDeleteOnBegin = isDeleteOnBegin;

	this->init("tutorial/Background.png");
	this->setAnchorPoint(Vec2::ZERO);
	this->setName("GameplayTutorial");
	this->setOpacity(0);
	this->setVisible(false);
	this->setEnabled(false);

	_next = nextObjTutorial;

	Sprite* finger = Sprite::createWithSpriteFrame(getSprite("Finger.png"));
	finger->setPosition(positionFrom);
	this->addChild(finger);

	ScaleTo* delay = ScaleTo::create(1.0f, 1.0f);
	Sequence* action = Sequence::create(MoveTo::create(1, positionTo), delay, MoveTo::create(0, positionFrom), nullptr);
	finger->runAction(RepeatForever::create(action));

	this->addTouchEventListener(CC_CALLBACK_2(GameplayTutorial::next, this));

	if (parent != nullptr) {
		parent->addChild(this);
	}
}


GameplayTutorial::~GameplayTutorial()
{

}

void GameplayTutorial::next(Ref* sender, ui::Widget::TouchEventType type) {

	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN: {
		if (_isDeleteOnBegin == true) {
			CallFunc* nextTutorial = CallFunc::create([&] {
				if (_next != nullptr) {
					//if (_next->getName() == "ObjectTutorial") {
						_next->setVisible(true);
						_next->setEnabled(true);
						//this->getParent()->addChild(_next, 997);
					//}
					//else {
					//	//this->getParent()->addChild(_next, 0);
					//}
				}
			});

			CallFunc* remove = CallFunc::create([&] {
				this->removeFromParentAndCleanup(1);
			});

			this->runAction(Sequence::create(nextTutorial, remove, nullptr));
		}
		break;
	}
	case ui::Widget::TouchEventType::ENDED: {
		if (_isDeleteOnBegin == false) {
			CallFunc* nextTutorial = CallFunc::create([&] {
				if (_next != nullptr) {
					_next->setVisible(true);
					_next->setEnabled(true);
				}
			});

			CallFunc* remove = CallFunc::create([&] {
				//this->release();
				this->removeFromParentAndCleanup(1);

			});

			this->runAction(Sequence::create(nextTutorial, remove, nullptr));
		}
		break;
	}
	default:
		break;
	}
}
