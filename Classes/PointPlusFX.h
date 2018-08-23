#include "BWTP_Func.h"
#include <string>

#pragma once
class PointPlusFX : public Label
{
public:
	PointPlusFX(Vec2 position, int point, float size, Node* parent);
	~PointPlusFX();

private:
	void remove();
};

