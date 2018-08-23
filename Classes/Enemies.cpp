#include "Enemies.h"
#include "MissionHandler.h"

Enemies::Enemies()
{
	this->autorelease();
}


Enemies::~Enemies()
{
	//delete this;
}

void Enemies::setMissionHandler(MissionHandler* missionHandler) {
	_missionHandler = missionHandler;
}

// Add Alien to Enemies Handler, increase counter
void Enemies::addEnemy() {
	_numOfEnemies++;
}

// Decrease counter
void Enemies::removeEnemy(string hitEnemy) {
	if (hitEnemy != _currentHitEnemy) {
		_numOfEnemies--;
		_currentHitEnemy = hitEnemy;
	}

	// If there're nomore alien checkout and show win scene
	if (_numOfEnemies == 0) {
		_missionHandler->checkout();
	}
}

int Enemies::getNumOfEnemies() {
	return _numOfEnemies;
}