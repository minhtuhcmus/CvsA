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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "BWTP_Func.h"
#include "BWTP_Func.h"

class Player;
class PlayerController;
class Enemies;
class MissionHandler;
class MyButton;
class Tutorial;

USING_NS_CC;

#define MAX_LEVEL 25

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(string levelName, int pageMenuIndex, bool hasPan);

    virtual bool init();
	void update(float dt) override;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void setPhysicTimeScale(float scale);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	MyButton* _retryButton;
	MyButton* _backButton;
	MyButton* _reloadButton;
	ui::Button* _submissionsBG;
	ui::Button* _submissionsTable;
	Label* _timeCountDown;
	ui::LoadingBar* _failCountDown;
	ui::Button* _popupBG;
	Sprite* _popupTable;
	ui::Button* _yesPopup;
	ui::Button* _noPopup;

	float _time;
	float _levelTime;
	float _timeToFail = 4;
	int _currentNumOfEnemies;
	bool _isEnd;
	bool _isPlayed;
	bool _isReloaded;
	float _step;
	float _delayToWin = 0.0f;

	int _submissions[11];
	int _numOfSubmissions = 11;

	bool _isBack = false;
	bool _isNext = false;
	bool _isChanged = false;

	Sprite* _background;
	Player* _player;
	PlayerController* _controller;
	Enemies* _enemies;
	MissionHandler* _missionHandler;
	Tutorial* _tutorial;
	string _nextSceneName;

	void retry(Ref* sender, ui::Widget::TouchEventType type);
	void back(Ref* sender, ui::Widget::TouchEventType type);
	void next(Ref* sender, ui::Widget::TouchEventType type);
	void onClickSubmission(Ref* sender, ui::Widget::TouchEventType type);
	void reloadAmmo(Ref* sender, ui::Widget::TouchEventType type);
	void checkSure();
	void onClickYesPopup(Ref* sender, ui::Widget::TouchEventType type);
	void onClickNoPopup(Ref* sender, ui::Widget::TouchEventType type);
};

#endif //__HELLOWORLD_SCENE_H__
