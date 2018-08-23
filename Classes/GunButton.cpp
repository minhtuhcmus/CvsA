#include "GunButton.h"

GunButton::GunButton(Node* parent)
{
	this->autorelease();

	this->init("UI/items/GunButton.png");
	this->addTouchEventListener(CC_CALLBACK_2(GunButton::onClick, this));
	parent->addChild(this);
}


GunButton::~GunButton()
{
}

void GunButton::onClick(Ref* sender, ui::Widget::TouchEventType type) {
	Node* isItemsSelected = this->getParent()->getChildByTag(ITEMS_SELECTED);
	if (isItemsSelected != nullptr) {
		isItemsSelected->removeFromParentAndCleanup(1);
	}
}