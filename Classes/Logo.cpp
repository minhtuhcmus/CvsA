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

#include "Logo.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "AudioEngine.h"
USING_NS_CC;

Scene* Logo::createScene()
{
    return Logo::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Logo::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	//string outPutDirPath = FileUtils::getInstance()->getWritablePath() + "/Android/data/com.test.app/resources";

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	experimental::AudioEngine::preload("bgMusic.mp3");
	experimental::AudioEngine::preload("level_bg_music.mp3");
	experimental::AudioEngine::preload("pan.mp3");
	experimental::AudioEngine::preload("alien.mp3");
	experimental::AudioEngine::preload("boom.mp3");
	experimental::AudioEngine::preload("gunshot.mp3");
	experimental::AudioEngine::preload("roastedChicked.mp3");
	experimental::AudioEngine::preload("winning.mp3");
	experimental::AudioEngine::preload("wasted.mp3");
	experimental::AudioEngine::preload("teleport.mp3");

	experimental::AudioEngine::play2d("bgMusic.mp3", true,0.8f);
	
	//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	//audio->preloadBackgroundMusic("bgMusic.mp3");
	//audio->preloadBackgroundMusic("level_bg_music.mp3");
	//audio->preloadEffect("pan.wav");
	//audio->preloadEffect("alien.wav");
	//audio->preloadEffect("boom.wav");
	//audio->preloadEffect("gunshot.wav");
	//audio->preloadEffect("roastedChicked.wav");
	//audio->playBackgroundMusic("bgMusic.mp3", true);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/Cowboy.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/Canon_Gun.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/bullet/FX/Gun_Shot_FX.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player/bullet/FX/Bullet_Wall_Contact.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemies/Alien.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemies/AnoyingFly.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemies/Alien_Body_Parts.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemies/CE_Body_Parts.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemies/AF_Body_Parts.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemies/FX/Blood_Hit.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("UI/rewards/FX/Firework.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/TNT_Explosion.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/Portal.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items/Portal_Blue.plist");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/Tutorial.plist");

	auto background = Sprite::create("logo.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);

	Sprite* border = Sprite::create("loadingbar.png");
	border->setScale(visibleSize.width/border->getContentSize().width, 0.6);
	border->setAnchorPoint(Vec2(0, 0));
	border->setPosition(Vec2(0.0, 0.0));
	
	loadingBar = ui::LoadingBar::create("loadingbar_full.png");
	loadingBar->setScale(visibleSize.width / border->getContentSize().width, 0.6);
	loadingBar->setAnchorPoint(Vec2(0, 0));
	loadingBar->setPosition(Vec2(0.0, 0.0));
	loadingBar->setPercent(0);

	this->scheduleUpdate();
	this->addChild(loadingBar, 1);
	this->addChild(background,0);
	this->addChild(border, 1);
	
    return true;
}

void Logo::update(float dt) {
	if (loadingBar->getPercent() < 100) {
		loadingBar->setPercent(loadingBar->getPercent() + 2);
	}
	else {
		Director::getInstance()->replaceScene(MainMenu::createScene());
	}
	
}