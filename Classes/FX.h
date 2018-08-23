#include "BWTP_Func.h"

using namespace std;

#pragma once
class FX : public Sprite
{
public:
	FX(Vec2 position, Vec2 anchorPoint, Node* parent, string key, float time);
	~FX();

	void remove();
};

