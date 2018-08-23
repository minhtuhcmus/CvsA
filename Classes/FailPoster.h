#include "BWTP_Func.h"

#pragma once
class FailPoster : public Sprite
{
public:
	FailPoster(Node* parent);
	~FailPoster();

	ui::Button _retry;
	ui::Button _back;
};

