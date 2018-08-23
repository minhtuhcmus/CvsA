#include "PanButton.h"

PanButton::PanButton(Node* parent)
{
	this->autorelease();

	this->init("UI/items/PanButton.png");
	this->addTouchEventListener(CC_CALLBACK_2(PanButton::onClick, this));

	parent->addChild(this);
}


PanButton::~PanButton()
{
}

void PanButton::onClick(Ref* sender, ui::Widget::TouchEventType type) {
	Node* isItemsSelected = this->getParent()->getChildByTag(ITEMS_SELECTED);
	if (isItemsSelected == nullptr) {
		Node* itemsSelected = Node::create();
		itemsSelected->setTag(ITEMS_SELECTED);
		this->getParent()->addChild(itemsSelected);
	}
}