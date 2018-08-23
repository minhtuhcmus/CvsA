#include "Vortex.h"

Vortex::Vortex(Vec2 position, Node* parent)
{
	this->autorelease();

	this->init("items/Vortex.png");
	this->setPosition(position);
	parent->addChild(this);

	this->addTouchEventListener(CC_CALLBACK_2(Vortex::onTouch, this));

	_scene = parent;
}

Vortex::~Vortex()
{
	//delete this;
}

void Vortex::onTouch(Ref* sender, ui::Widget::TouchEventType type) {
	Node* bullet = _scene->getChildByTag(BULLET);
	if (bullet == nullptr) return;
	Vec2 distance = bullet->getPosition() - this->getPosition();
	float dist = sqrtf(distance.x*distance.x + distance.y*distance.y);
	if (dist > this->getContentSize().width / 2) return;

	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN: {
		break;
	}
	case ui::Widget::TouchEventType::ENDED: {
		break;
	}
	default:
		break;
	}
}