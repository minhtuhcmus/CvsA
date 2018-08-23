#include "Pan.h"
#include "HelloWorldScene.h"

Pan::Pan(Vec2 position, int tag, Node* parent)
{
	// Start :: Init Pan Sprite
	_tag = tag;
	_currentRotation = 0;

	this->autorelease();
	this->setName("Pan" + to_string(_tag));
	this->setTag(PAN);
	this->initWithFile("items/Pan.png");
	this->setAnchorPoint(Vec2(0.5, 0.35));
	this->setPosition(position);
	parent->addChild(this);
	// End :: Init Pan Sprite

	ui::Button* touchBox = ui::Button::create("items/Pan_Button.png");
	touchBox->setAnchorPoint(Vec2(0.5, 0.5));
	touchBox->setPosition(this->getContentSize() / 2);
	touchBox->addTouchEventListener(CC_CALLBACK_2(Pan::touchOnPan, this));
	this->addChild(touchBox);

	// Start :: Init PhysicBody
	Point vers[] = {
		Point(-45.0f, 5.0f),
		Point(40.0f, 5.0f),
		Point(25.0f, -20.0f),
		Point(-25.0f, -20.0f)
	};
	auto panBody = PhysicsBody::createPolygon(vers, (sizeof(vers) / sizeof(vers[0])), PhysicsMaterial(0.0f, 1.0f, 0.0f));

	//PhysicsBody* panBody = PhysicsBody::createBox(Size(this->getContentSize().width, this->getContentSize().height * 0.75), PhysicsMaterial(0.0f, 1.0f, 0.0f), Vec2(0, -this->getContentSize().height / 5));
	panBody->setDynamic(false);
	panBody->setGroup(PAN);
	panBody->setCategoryBitmask(PAN);
	panBody->setCollisionBitmask(BULLET + FAKE_BULLET);
	panBody->setContactTestBitmask(BULLET);
	this->addComponent(panBody);
	int rotateIndex = random(0, 4);
	_currentRotation = _rotateAngel * rotateIndex;
	this->setRotation(_currentRotation);
	// End :: Init PhysicBody
}


Pan::~Pan()
{
}

void Pan::touchOnPan(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		if (_isClicked == false) {
			_isClicked = true;
			_currentRotation += _rotateAngel;
			this->setRotation(_currentRotation);
		}
		break;
	case ui::Widget::TouchEventType::ENDED: {
		_isClicked = false;
		break;
	}
	default:
		break;
	}
}

