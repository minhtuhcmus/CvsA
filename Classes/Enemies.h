#include "BWTP_Func.h"

class MissionHandler;
#pragma once
class Enemies : public Node
{
public:
	Enemies();
	~Enemies();

	void addEnemy();
	int getNumOfEnemies();
	void removeEnemy(string hitEnemy);
	void setMissionHandler(MissionHandler* missionHandler);

private:
	int _numOfEnemies = 0;
	MissionHandler* _missionHandler;
	string _currentHitEnemy;
};

