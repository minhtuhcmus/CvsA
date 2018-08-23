#include "BWTP_Func.h"

#pragma once
class Ammo : public Node
{
public:
	Ammo(int ammo);
	~Ammo();

	void popBullet();
	int getAmmo();
	void reloadAmmo(int ammo);
	Vector<Node*>* getListOfBullets();
private:
	int _ammo;
	Vector<Node*> _bulletList;
};

