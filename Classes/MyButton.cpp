#include "MyButton.h"

MyButton::MyButton(Vec2 position,string path, string path2, Node* parent)
{
	this->autorelease();

	this->init(path, path2);
	this->setPosition(position);
	parent->addChild(this, 1000);
}
MyButton::MyButton(Vec2 position, string path, string path2, Node* parent,int LocalZOrder)
{
	this->autorelease();

	this->init(path, path2);
	this->setPosition(position);
	parent->addChild(this, LocalZOrder);
}

MyButton::~MyButton()
{
	//delete this;
}