#include "MissionHandler.h"
#include "AudioEngine.h"
#include "PlayerController.h"
#include "RewardPoster.h"
#include "FailPoster.h"
#include "Player.h"


MissionHandler::MissionHandler(PlayerController* playerController, Player* player, Node* parent)
{

	this->autorelease();
	_numOfStars = 0;
	_playerController = playerController;
	_player = player;
	_runningScene = parent;
	_isWon = false;
	_isFailed = false;

	parent->addChild(this, 996);

	for (int i = 0; i < 11; i++) {
		_submissionsEnable[i] = false;
	}

	this->scheduleUpdate();
}

void MissionHandler::update(float delta) {
	if (_player->getAmmo() == 0 && !_isWon) {
		if (_runningScene->getChildByName("Bullet") == nullptr) {
			fail();
		}
	}
	if (_player->getAmmo() > 0 || _runningScene->getChildByName("Bullet") != nullptr) {
		_isFailed = false;
	}
}

MissionHandler::~MissionHandler()
{
	//delete this;
}

void MissionHandler::addScore(int score) {
	_missionScore += score;
}

void MissionHandler::setLevelName(string levelname) {
	_levelname = levelname;
}

void MissionHandler::setIDbgMusic(int id) {
	_idBgMusic = id;
}

void MissionHandler::checkout() {
	if (_isWon == true) return;

	Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_playerController);
	_isWon = true;
	_isFailed == false;
}

void MissionHandler::fail() {
	if (_isWon == true || _isFailed == true) return;

	_isFailed = true;
}

void MissionHandler::checkSubmissions(int timeRemain, int ammo, int bounces) {
	int i = 0;
	if (_submissionsEnable[RUSH_TIME]) {
		if (_timeToRush >= timeRemain) {
			_messageOut[i] = Label::createWithTTF("Win in " + to_string(_timeToRush) + " seconds.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Win in " + to_string(_timeToRush) + " seconds.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[BULLET_COUNT]) {
		if (ammo >= _bulletToCount) {
			_messageOut[i] = Label::createWithTTF("Remain " + to_string(_bulletToCount) + " bullets.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Remain " + to_string(_bulletToCount) + " bullets.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[BULLET_BOUNCING]) {
		if (_bouncesToCount <= bounces) {
			_messageOut[i] = Label::createWithTTF("Remain " + to_string(_bouncesToCount) + " bounces.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Remain " + to_string(_bouncesToCount) + " bounces.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[ONE_SHOT]) {
		if (_oneShot <= ammo) {
			_messageOut[i] = Label::createWithTTF("One shot clear.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("One shot clear.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[NO_BOUNCING]) {
		if (_noBouncing <= bounces) {
			_messageOut[i] = Label::createWithTTF("No bouncing.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("No bouncing.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[DONT_ROASTED]) {
		if (this->getParent()->getChildByName("RoastedChicken") == nullptr) {
			_messageOut[i] = Label::createWithTTF("Dont roast Chick.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Dont roast Chick.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[DONT_KNOCKED]) {
		if (this->getParent()->getChildByName("KnockedChicken") == nullptr) {
			_messageOut[i] = Label::createWithTTF("Dont knock Chick.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Dont knock Chick.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[DONT_HIT_TNT]) {
		if (this->getParent()->getChildByTag(TNT_DEFINE) != nullptr) {
			_messageOut[i] = Label::createWithTTF("Dont destroy all TNTs.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Dont destroy all TNTs.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[HIT_TNT]) {
		if (this->getParent()->getChildByTag(TNT_DEFINE) == nullptr) {
			_messageOut[i] = Label::createWithTTF("Destroy all TNTs.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Destroy all TNTs.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[KILL_ALL]) {
		if (ammo >= 0) {
			_messageOut[i] = Label::createWithTTF("Eliminate all enemies.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("Eliminate all enemies.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
	if (_submissionsEnable[EATEN]) {
		if (this->getParent()->getChildByName("KnockedChicken") == nullptr && this->getParent()->getChildByName("RoastedChicken") == nullptr && this->getParent()->getChildByName("Bullet") == nullptr) {
			_messageOut[i] = Label::createWithTTF("All Chick be eaten.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(94, 36, 14));
			_missionScore += 50;
		}
		else {
			_messageOut[i] = Label::createWithTTF("All Chick be eaten.", "fonts/CarnevaleeFreakshow.ttf", _fontSize);
			_messageOut[i]->setColor(Color3B(200, 200, 200));
		}
		i++;
	}
}

void MissionHandler::winScene(int timeRemain, int ammo, int bounces) {
	experimental::AudioEngine::pause(_idBgMusic);
	experimental::AudioEngine::play2d("winning.mp3");

	checkSubmissions(timeRemain, ammo, bounces);

	RewardPoster* rewardPoster = new RewardPoster(this);
	for (int i = 0; i < 3; i++) {
		if (_messageOut[i] == nullptr) break;
		rewardPoster->addChild(_messageOut[i]);
		_messageOut[i]->setAnchorPoint(Vec2(0, 0.5));
		_messageOut[i]->setPosition(Vec2(60, rewardPoster->getContentSize().height * 0.65 - i * 50));
	}

	Sprite* secondStar_Empty = Sprite::create("UI/rewards/Star.png");
	secondStar_Empty->setScale(1.3f);
	secondStar_Empty->setColor(Color3B(0, 0, 0));
	secondStar_Empty->setPosition(Vec2(rewardPoster->getContentSize().width / 2, 0));
	Sprite* firstStar_Empty = Sprite::create("UI/rewards/Star.png");
	firstStar_Empty->setColor(Color3B(0, 0, 0));
	firstStar_Empty->setPosition(secondStar_Empty->getPosition() - Vec2(secondStar_Empty->getContentSize().width, 0));
	Sprite* thirdStar_Empty = Sprite::create("UI/rewards/Star.png");
	thirdStar_Empty->setColor(Color3B(0, 0, 0));
	thirdStar_Empty->setPosition(secondStar_Empty->getPosition() + Vec2(secondStar_Empty->getContentSize().width, 0));

	rewardPoster->addChild(firstStar_Empty);
	rewardPoster->addChild(secondStar_Empty);
	rewardPoster->addChild(thirdStar_Empty);

	if (_missionScore >= 50) {
		_numOfStars = 1;
		Sprite* firstStar = Sprite::create("UI/rewards/Star.png");
		firstStar->setPosition(firstStar_Empty->getPosition());
		rewardPoster->addChild(firstStar);
	}
	if (_missionScore >= 100) {
		_numOfStars = 2;
		Sprite* secondStar = Sprite::create("UI/rewards/Star.png");
		secondStar->setScale(1.3);
		secondStar->setPosition(secondStar_Empty->getPosition());
		rewardPoster->addChild(secondStar);
	}
	if (_missionScore >= 150) {
		_numOfStars = 3;
		Sprite* thirdStar = Sprite::create("UI/rewards/Star.png");
		thirdStar->setPosition(thirdStar_Empty->getPosition());
		rewardPoster->addChild(thirdStar);
	}

	string tmp = _levelname + "star";
	const char* keyStar = tmp.c_str();
	string tmp1 = _levelname + "isEnable";
	const char* keyIE = tmp1.c_str();

	UserDefault* def = UserDefault::getInstance();
	int hightStar = def->getIntegerForKey(keyStar);
	if (_numOfStars > hightStar) {
		def->setIntegerForKey(keyStar, _numOfStars);
		int coin = def->getIntegerForKey("coin")+ 50* (_numOfStars - hightStar);
		def->setIntegerForKey("coin", coin);
	}
	if (_numOfStars > 0) {
		def->setIntegerForKey(keyIE, true);
	}

	def->flush();

	this->unscheduleUpdate();
}

void MissionHandler::failScene() {
	UserDefault* def = UserDefault::getInstance();
	int heart = def->getIntegerForKey("heart");
	heart--;
	if (heart == 0) {
		def->setIntegerForKey("heart", -1);
	}
	else {
		def->setIntegerForKey("heart", heart);
	}
	def->flush();
	experimental::AudioEngine::pause(_idBgMusic);
	experimental::AudioEngine::play2d("wasted.mp3");
	Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(_playerController);

	FailPoster* failPoster = new FailPoster(this);

	this->unscheduleUpdate();
}

bool MissionHandler::isWon() {
	return _isWon;
}

bool MissionHandler::isFail() {
	return _isFailed;
}

void MissionHandler::setRushtime(int timeToRush) {
	if (timeToRush < 0) return;

	_submissionsEnable[RUSH_TIME] = true;
	_timeToRush = timeToRush;
}

void MissionHandler::setBulletToCount(int bulletToCount) {
	if (bulletToCount < 0) return;

	_submissionsEnable[BULLET_COUNT] = true;
	_bulletToCount = bulletToCount;
}

void MissionHandler::setBouncesToCount(int bouncesToCount) {
	if (bouncesToCount < 0) return;

	_submissionsEnable[BULLET_BOUNCING] = true;
	_bouncesToCount = bouncesToCount;
}

void MissionHandler::setOneShot(int bulletToCount) {
	if (bulletToCount < 0) return;

	_submissionsEnable[ONE_SHOT] = true;
	_oneShot = bulletToCount;
}

void MissionHandler::setNoBoucing(int bouncesToCount) {
	if (bouncesToCount < 0) return;

	_submissionsEnable[NO_BOUNCING] = true;
	_noBouncing = bouncesToCount;
}

void MissionHandler::setDontRoasted(int dontRoasted) {
	if (dontRoasted < 0) return;

	_submissionsEnable[DONT_ROASTED] = true;
}

void MissionHandler::setDontKnocked(int dontKnocked) {
	if (dontKnocked < 0) return;

	_submissionsEnable[DONT_KNOCKED] = true;
}

void MissionHandler::setDontHitTNT(int dontHitTNT) {
	if (dontHitTNT < 0) return;

	_submissionsEnable[DONT_HIT_TNT] = true;
}

void MissionHandler::setHitTNT(int hitTNT) {
	if (hitTNT < 0) return;

	_submissionsEnable[HIT_TNT] = true;
}

void MissionHandler::setKillAllEnemies(int killALL) {
	if (killALL < 0) return;

	_submissionsEnable[KILL_ALL] = true;
}

void MissionHandler::setEaten(int eaten) {
	if (eaten < 0) return;

	_submissionsEnable[EATEN] = true;
}