#include "ObjectTutorial.h"

ObjectTutorial::ObjectTutorial(Vec2 position, float rotation, string path, string message, ui::Button* nextObjTutorial, Node* parent)
{
	this->autorelease();

	this->init("tutorial/Background.png");
	this->setName("ObjectTutorial");
	this->setAnchorPoint(Vec2::ZERO);
	this->setColor(Color3B::BLACK);
	this->setOpacity(100);
	this->setVisible(false);
	this->setEnabled(false);
	_next = nextObjTutorial;

	string temp = path + "_01.png";
	Sprite* pointer = Sprite::createWithSpriteFrame(getSprite(temp.c_str()));
	temp = (path + "_%02d.png").c_str();
	Animation* animation = Animation::createWithSpriteFrames(getAnimation(temp.c_str()), 1/25.0f);
	Animate* animate = Animate::create(animation);
	pointer->runAction(RepeatForever::create(animate));
	pointer->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.5f, Director::getInstance()->getVisibleSize().height * 0.7f));
	float scale = (Director::getInstance()->getVisibleSize().height / pointer->getContentSize().height) * 0.5;
	pointer->setScale(scale);
	this->addChild(pointer);

	Label* messageOut = Label::create(message, "", 24);
	//messageOut->setAnchorPoint(Vec2(0, 1));
	messageOut->setColor(Color3B::WHITE);
	messageOut->setPosition(Vec2(400,112));
	//messageOut->setRotation(rotation);
	this->addChild(messageOut);

	this->addTouchEventListener(CC_CALLBACK_2(ObjectTutorial::next, this));

	if (parent != nullptr) {
		parent->addChild(this, 997);
	}
}


ObjectTutorial::~ObjectTutorial()
{

}

void ObjectTutorial::next(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		CallFunc* nextTutorial = CallFunc::create([&] {
			if (_next != nullptr) {
				_next->setVisible(true);
				_next->setEnabled(true);
			}
		});

		CallFunc* remove = CallFunc::create([&] {
			this->removeFromParentAndCleanup(1);
		});

		this->runAction(Sequence::create(nextTutorial, remove, nullptr));
		break;
	}
	default:
		break;
	}
}
