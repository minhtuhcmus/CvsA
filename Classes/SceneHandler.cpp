#include "SceneHandler.h"

void SceneHandler::checkout() {
	MissionHandler* missionHandler = (MissionHandler*)Director::getInstance()->getRunningScene()->getChildByName("Missionhandler");
	missionHandler->checkout();
}

void SceneHandler::fail() {
	MissionHandler* missionHandler = (MissionHandler*)Director::getInstance()->getRunningScene()->getChildByName("Missionhandler");
	missionHandler->fail();
}