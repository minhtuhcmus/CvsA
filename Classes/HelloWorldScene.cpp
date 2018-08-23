/****************************************************************************
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "HelloWorldScene.h"
#include "MainScene.h"
#include "json\rapidjson.h"
#include "json\document.h"
#include <fstream>
#include "json/stringbuffer.h"
#include "json/writer.h"
#include <string>
#include "Tutorial.h"
#include "AudioEngine.h"
#include "Bullet.h"
#include "Init.h"
#include "PlayerController.h"
#include "SimpleAudioEngine.h"
#include "Alien.h"
#include "Enemies.h"
#include "MissionHandler.h"
#include "MainScene.h"
#include "MyButton.h"
#include "TNT.h"
#include "Pan.h"
#include "Player.h"

using namespace rapidjson;

USING_NS_CC;
using namespace std;

string _levelName;
int _pageMenuIndex;
bool _hasPan = true;

Scene* HelloWorld::createScene(string levelName,int pageMenuIndex, bool hasPan)
{
	Scene* scene = Scene::createWithPhysics();

	// set gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0, -900));

	// optional: set debug draw
	scene->getPhysicsWorld()->setAutoStep(false);
	//scene->getPhysicsWorld()->setDebugDrawMask(0xffff);

	_levelName = levelName;
	_pageMenuIndex = pageMenuIndex;
	_hasPan = hasPan;

	HelloWorld* layer = HelloWorld::create();
	layer->setName("Layer");
	scene->addChild(layer);

	return scene;
	//return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	TextureCache::getInstance()->removeAllTextures();

	// super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//[Huy]Start : Read data using JSON
	Document m_document;
	ssize_t size;
	char* buf = (char*)FileUtils::getInstance()->getFileData("StatusLevel.json", "r", &size);
	string content(buf, size);

	m_document.Parse(content.c_str());

	Document m_document2;
	buf = (char*)FileUtils::getInstance()->getFileData("Submissions.json", "r", &size);
	string submissionsInfo(buf, size);

	m_document2.Parse(submissionsInfo.c_str());

	const char *key = _levelName.c_str();

	_levelTime = m_document2[key]["time"].GetInt();
	_time = _levelTime;
	_timeToFail = 4;
	_step = 1.0f / 5.0f;
	_isBack = false;
	_isChanged = false;
	_isEnd = true;
	_isPlayed = false;
	_isReloaded = false;

	//[Huy]End : Read data using JSON
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("level_bg_music.mp3");
	experimental::AudioEngine::stopAll();
	int _idBgMusic = experimental::AudioEngine::play2d("level_bg_music.mp3",true);

	// Start :: Background with physicEdgeBox
	_background = initBackground("Background/Western_Background.png", this);
	// End :: Background with physicEdgeBox

	// Start :: Init level from tilemap
	auto level = cocos2d::TMXTiledMap::create(_levelName + ".tmx");
	Sprite* objectLayer = Sprite::create("levels/" + _levelName + ".png");
	objectLayer->setAnchorPoint(Vec2(0, 0));
	this->addChild(objectLayer);

	initWall(level, this);
	// End :: Init level from tilemap

	// Start :: Tutorial
	UserDefault* def = UserDefault::getInstance();
	bool isTutorialized = def->getBoolForKey(_levelName.c_str());
	if (isTutorialized == false) {
		_tutorial = new Tutorial(_levelName, this);
		_tutorial->setName("Tutorial");
		def->setBoolForKey(_levelName.c_str(), true);
	}
	// End :: Tutorial

	// Start :: Player and PlayerContoller
	_player = initPlayer(level, m_document2[key]["ammo"].GetInt(), 1.0f, this);
	_controller = initPlayerController(_player, this);
	if (_levelName == "level2") {
		_controller->setTutorial(true);
		_controller->hint(52.0f);
	}
	else if (_levelName == "level3") {
		_controller->hint(27.0f);
	}

	string levelNameTemp = _levelName;
	levelNameTemp.erase(0, 5);
	int currentLevelIndex = atoi(levelNameTemp.c_str());
	if (currentLevelIndex > 16) {
		_controller->setEnableTrajectory(false);
	}
	// End :: Player and PlayerContoller

	// Start :: Init Enemies
	_enemies = initEnemies(level, this);
	_currentNumOfEnemies = _enemies->getNumOfEnemies();
	// End :: Init Enemies

	// Start :: Init items
	initItems(level, this);
	// End :: Init items

	// Start :: Pan
	initPans(level, this);
	if (_levelName == "level3") {
		this->getChildByTag(PAN)->setRotation(0);
	}
	// End :: Pan

	//[Huy]Start
	_missionHandler = new MissionHandler(_controller, _player, this);
	_missionHandler->setLevelName(_levelName);
	_missionHandler->setIDbgMusic(_idBgMusic);
	_enemies->setMissionHandler(_missionHandler);
	//[Huy]End

	// Start :: Time Count Down
	_timeCountDown = Label::createWithTTF(to_string(int(_time)), "fonts/Marker Felt.ttf", 40);
	_timeCountDown->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.9));
	this->addChild(_timeCountDown);
	// End :: Time Count Down

	// Start :: Retry Button
	_retryButton = new MyButton(Vec2(visibleSize.width*0.95, visibleSize.height * 0.93), "UI/buttons/Retry.png", "UI/buttons/Retry_Pressed.png", this);
	_retryButton->setScale(0.6f);
	_retryButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::retry, this));
	// End :: Retry Button

	// Start :: Back Button
	_backButton = new MyButton(Vec2(visibleSize.width*0.85, visibleSize.height * 0.93), "UI/buttons/Menu.png", "UI/buttons/Menu_Pressed.png", this);
	_backButton->setScale(0.6f);
	_backButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::back, this));
	// End :: Back Button

	// Start :: Reload Ammo Button
	_reloadButton = new MyButton(Director::getInstance()->getVisibleSize() / 20, "Plus.png", "UI/buttons/Plus_Pressed.png", this);
	_reloadButton->setOpacity(0);
	_reloadButton->setEnabled(false);
	_reloadButton->setScale(0.6f);
	_reloadButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::reloadAmmo, this));
	// End :: Reload Ammo Button

	// Start :: Submissions
	_submissionsBG = ui::Button::create("UI/buttons/FullsceneButton.png");
	_submissionsBG->setOpacity(0);
	_submissionsBG->setPosition(visibleSize / 2);
	_submissionsBG->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onClickSubmission, this));
	_submissionsTable = ui::Button::create("UI/Border_Named.png");
	_submissionsTable->setScale(0.5f);
	_submissionsTable->setPosition(visibleSize / 2);
	_submissionsTable->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onClickSubmission, this));
	_submissionsBG->addChild(_submissionsTable);
	this->addChild(_submissionsBG, 998);

	Label* subbmissionLabel = Label::createWithTTF("Submissions", "fonts/CarnevaleeFreakshow.ttf", 50);
	subbmissionLabel->setColor(Color3B(94, 36, 14));
	subbmissionLabel->setPosition(Vec2(_submissionsTable->getContentSize().width / 2, _submissionsTable->getContentSize().height * 0.9));
	_submissionsTable->addChild(subbmissionLabel);

	int i = 0;
	int alignX = 70;
	int alignY = 80;

	string missionName[11] = {
		"timeToRush",
		"bulletToCount",
		"bouncesToCount",
		"oneShot",
		"noBouncing",
		"dontRoasted",
		"dontKnocked",
		"dontHitTNT",
		"hitTNT",
		"killAllEnemies",
		"eaten"
	};

	for (int i = 0; i < _numOfSubmissions; i++) {
		int submission = m_document2[key]["submissions"][missionName[i].c_str()].GetInt();
		_submissions[i] = submission;
	}

	if (_submissions[MissionHandler::RUSH_TIME] >= 0) {
		Label* subbmission1 = Label::createWithTTF("- Win in " + to_string(int(_time) - _submissions[MissionHandler::RUSH_TIME]) + " seconds.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission1->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission1->setColor(Color3B(94, 36, 14));
		subbmission1->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7 - alignY * i));
		_submissionsTable->addChild(subbmission1);
		_missionHandler->setRushtime(int(_time) - _submissions[MissionHandler::RUSH_TIME]);
		i++;
	}
	if (_submissions[MissionHandler::BULLET_COUNT] >= 0) {
		Label* subbmission2 = Label::createWithTTF("- Remain " + to_string(_submissions[MissionHandler::BULLET_COUNT]) + " bullets.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission2->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission2->setColor(Color3B(94, 36, 14));
		subbmission2->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission2);
		_missionHandler->setBulletToCount(_submissions[MissionHandler::BULLET_COUNT]);
		i++;
	}
	if (_submissions[MissionHandler::BULLET_BOUNCING] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- Remain " + to_string(_submissions[MissionHandler::BULLET_BOUNCING]) + " bounces.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setBouncesToCount(_submissions[MissionHandler::BULLET_BOUNCING]);
		i++;
	}
	if (_submissions[MissionHandler::ONE_SHOT] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- One shot clear.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setOneShot(_player->getAmmo() - 1);
		i++;
	}
	if (_submissions[MissionHandler::NO_BOUNCING] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- No Bouncing.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setNoBoucing(BULLET_HITS);
		i++;
	}
	if (_submissions[MissionHandler::DONT_ROASTED] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- Dont roast Chick.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setDontRoasted(_submissions[MissionHandler::DONT_ROASTED]);
		i++;
	}
	if (_submissions[MissionHandler::DONT_KNOCKED] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- Dont knock Chick.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setDontKnocked(_submissions[MissionHandler::DONT_KNOCKED]);
		i++;
	}
	if (_submissions[MissionHandler::DONT_HIT_TNT] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- Dont destroy all TNTs.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setDontHitTNT(_submissions[MissionHandler::DONT_HIT_TNT]);
		i++;
	}
	if (_submissions[MissionHandler::HIT_TNT] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- Destroy all TNTs.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setHitTNT(_submissions[MissionHandler::HIT_TNT]);
		i++;
	}
	if (_submissions[MissionHandler::KILL_ALL] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- Eliminate all enemies.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setKillAllEnemies(_submissions[MissionHandler::KILL_ALL]);
		i++;
	}
	if (_submissions[MissionHandler::EATEN] >= 0) {
		Label* subbmission3 = Label::createWithTTF("- All Chick be eaten.", "fonts/CarnevaleeFreakshow.ttf", 50);
		subbmission3->setAnchorPoint(Vec2(0.0f, 0.5f));
		subbmission3->setColor(Color3B(94, 36, 14));
		subbmission3->setPosition(Vec2(alignX, _submissionsTable->getContentSize().height * 0.7f - alignY * i));
		_submissionsTable->addChild(subbmission3);
		_missionHandler->setEaten(_submissions[MissionHandler::EATEN]);
		i++;
	}
	// End :: Submissions

	level->removeFromParentAndCleanup(1);

	this->scheduleUpdate();

	return true;
}

void HelloWorld::update(float dt)
{
	if (_isEnd == false) {
		if (_isPlayed == false) {
			if (this->getChildByTag(PLAYED_TRIGGER)) {
				_isPlayed = true;
			}
		}

		if (_missionHandler->isWon()) {
			ScaleTo* delay = ScaleTo::create(_delayToWin, 1.0f);
			CallFunc* showWinScene = CallFunc::create([&] {
				if (_reloadButton != nullptr) {
					_reloadButton->setEnabled(false);
				}
				Size visibleSize = Director::getInstance()->getVisibleSize();

				Label* getBullet = (Label*)this->getChildByName("Hitcounter");

				int bouncingCount = 0;
				if (getBullet != nullptr) {
					bouncingCount = atoi(getBullet->getString().c_str());
				}
				_missionHandler->winScene(_levelTime - _time, _player->getAmmo(), bouncingCount);

				if (_missionHandler->getStars() >= 1) {
					MyButton* nextLevelButton_Full = new MyButton(visibleSize / 2, "UI/buttons/FullsceneButton.png", "UI/buttons/FullsceneButton.png", this);
					nextLevelButton_Full->setZOrder(999);
					nextLevelButton_Full->setOpacity(0);
					nextLevelButton_Full->addTouchEventListener(CC_CALLBACK_2(HelloWorld::next, this));

					MyButton* nextLevelButton = new MyButton(Vec2(visibleSize.width*0.5, visibleSize.height * 0.3), "UI/buttons/Next.png", "UI/buttons/NextPressed.png", this);
					nextLevelButton->setScale(2.0f);
					nextLevelButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::next, this));
				}
			});
			this->runAction(Sequence::create(delay, showWinScene, nullptr));
			_isEnd = true;
			_isPlayed = false;
		}
		else if (_missionHandler->isFail()) {
			if (_isReloaded == false) {
				if (UserDefault::getInstance()->getIntegerForKey("coin") > 10) {
					if (_reloadButton != nullptr) {
						_reloadButton->setOpacity(255);
						_reloadButton->setEnabled(true);
					}
				}
			}
			if (_enemies->getNumOfEnemies() != _currentNumOfEnemies) {
				_currentNumOfEnemies = _enemies->getNumOfEnemies();
				_timeToFail = 4;
			}
			_timeToFail -= dt;
			if (_timeToFail <= 0) {
				if (_reloadButton != nullptr) {
					_reloadButton->setEnabled(false);
				}
				Size visibleSize = Director::getInstance()->getVisibleSize();
				_retryButton->setPosition(Vec2(visibleSize.width * 0.6, visibleSize.height * 0.15));
				_retryButton->setScale(1.0f);
				_backButton->setPosition(Vec2(visibleSize.width * 0.4, visibleSize.height * 0.15));
				_backButton->setScale(1.0f);
				_missionHandler->failScene();
				_isEnd = true;
				_isPlayed = false;
			}
		}
		else {
			if (_isReloaded == false) {
				if (_reloadButton != nullptr) {
					_reloadButton->setOpacity(0);
					_reloadButton->setEnabled(false);
				}
			}
		}

		if (_time <= 3) {
			_timeCountDown->setColor(Color3B::RED);
		}
		else if (_time <= 10) {
			_timeCountDown->setColor(Color3B::YELLOW);
		}

		if (_time > 0) {
			if (_isPlayed) {
				_time -= dt;
				_timeCountDown->setString(to_string(int(_time)));
			}
		}
		else {
			Size visibleSize = Director::getInstance()->getVisibleSize();
			_retryButton->setPosition(Vec2(visibleSize.width * 0.6, visibleSize.height * 0.15));
			_retryButton->setScale(1.0f);
			_backButton->setPosition(Vec2(visibleSize.width * 0.4, visibleSize.height * 0.15));
			_backButton->setScale(1.0f);
			_missionHandler->failScene();
			_isEnd = true;
		}

		if (_isEnd == true) {
			_isPlayed = false;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		this->getScene()->getPhysicsWorld()->step(_step * dt);
	}
}

void HelloWorld::retry(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		if (_isBack == false) {
			_isBack = true;
			UserDefault* def = UserDefault::getInstance();
			if (def->getIntegerForKey("heart") > 0) {
				if (_isPlayed) {
					_nextSceneName = "GameScene";
					checkSure();
				}
				else {
					Director::getInstance()->replaceScene(TransitionFade::create(0.5, HelloWorld::createScene(_levelName, _pageMenuIndex, _hasPan)));
				}
			}
			else {
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScene::createScene(_pageMenuIndex)));
			}
		}
		break;
	}
	default:
		break;
	}
}

void HelloWorld::next(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		if (_isNext == false) {
			_isNext = true;

			_levelName.erase(0, 5);
			int currentLevelIndex = atoi(_levelName.c_str());
			currentLevelIndex++;
			if (currentLevelIndex <= MAX_LEVEL) {
				_levelName = "level" + to_string(currentLevelIndex);
				_pageMenuIndex = (currentLevelIndex - 1) / 6;
				if (currentLevelIndex > 2) {
					_hasPan = true;
				}
				else {
					_hasPan = false;
				}
				this->unscheduleUpdate();

				Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5, HelloWorld::createScene(_levelName, _pageMenuIndex, _hasPan)));
			}
			else {
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScene::createScene(_pageMenuIndex)));
			}
		}
		break;
	}
	default:
		break;
	}
}

void HelloWorld::back(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		if (_isBack == false) {
			_isBack = true;
			if (_isPlayed) {
				_nextSceneName = "MainScene";
				checkSure();
			}
			else {
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScene::createScene(_pageMenuIndex)));
			}
		}
		break;
	}
	default:
		break;
	}
	
}

void HelloWorld::onClickSubmission(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		_isEnd = false;
		_submissionsBG->removeFromParentAndCleanup(1);
		if (_tutorial != nullptr) {
			_tutorial->setEnable();
		}
		break;
	}
	default:
		break;
	}
}

void HelloWorld::reloadAmmo(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		UserDefault* def = UserDefault::getInstance();
		def->setIntegerForKey("coin",def->getIntegerForKey("coin") - 10);
		def->flush();
		_timeToFail = 4;
		_isReloaded = true;
		_reloadButton->removeFromParentAndCleanup(1);
		_reloadButton = nullptr;
		_player->reloadAmmo(1);
		break;
	}
	default:
		break;
	}
}

void HelloWorld::setPhysicTimeScale(float scale) {
	_step = scale;
}

void HelloWorld::checkSure() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_popupBG = ui::Button::create("UI/buttons/FullsceneButton.png");
	_popupBG->setOpacity(0);
	_popupBG->setPosition(visibleSize / 2);
	_popupBG->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onClickNoPopup, this));
	_popupTable = Sprite::create("UI/Border.png");
	_popupTable->setScale(0.5f);
	_popupTable->setPosition(visibleSize / 2);
	_popupBG->addChild(_popupTable);
	this->addChild(_popupBG, 998);

	Label* popupLabel = Label::createWithTTF("Are you sure?", "fonts/CarnevaleeFreakshow.ttf", 50);
	popupLabel->setColor(Color3B(94, 36, 14));
	popupLabel->setPosition(_popupTable->getContentSize() / 2);
	_popupTable->addChild(popupLabel);

	_yesPopup = ui::Button::create("UI/buttons/Yes.png", "UI/buttons/Yes_Pressed.png");
	_yesPopup->setScale(2.0f);
	_yesPopup->setPosition(Vec2(_popupTable->getContentSize().width * 0.35, 0));
	_yesPopup->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onClickYesPopup, this));
	_popupTable->addChild(_yesPopup);

	_noPopup = ui::Button::create("UI/buttons/No.png", "UI/buttons/No_Pressed.png");
	_noPopup->setScale(2.0f);
	_noPopup->setPosition(Vec2(_popupTable->getContentSize().width * 0.65, 0));
	_noPopup->addTouchEventListener(CC_CALLBACK_2(HelloWorld::onClickNoPopup, this));
	_popupTable->addChild(_noPopup);
}

void HelloWorld::onClickYesPopup(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		if (_isChanged == false) {
			_isChanged = true;
			UserDefault* def = UserDefault::getInstance();
			if (_isPlayed) {
				int heart = def->getIntegerForKey("heart");
				heart--;
				if (heart == 0) {
					def->setIntegerForKey("heart", -1);
				}
				else {
					def->setIntegerForKey("heart", heart);
				}
			}
			this->unscheduleUpdate();

			if (_nextSceneName == "MainScene") {
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScene::createScene(_pageMenuIndex)));
			}
			else if (_nextSceneName == "GameScene") {
				Director::getInstance()->replaceScene(TransitionFade::create(0.5, HelloWorld::createScene(_levelName, _pageMenuIndex, _hasPan)));
			}
		}
		break;
	}
	default:
		break;
	}
}

void HelloWorld::onClickNoPopup(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED: {
		_popupBG->removeFromParentAndCleanup(1);
		_isBack = false;
		break;
	}
	default:
		break;
	}
}