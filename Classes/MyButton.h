#include "BWTP_Func.h"

#pragma once
class MyButton : public ui::Button
{
public:
	MyButton(Vec2 position, string path, string path2, Node* parent);
	MyButton(Vec2 position, string path, string path2, Node* parent, int LocalZOrder);
	~MyButton();
};

