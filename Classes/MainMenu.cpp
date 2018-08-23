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

#include "MainMenu.h"
#include "SelectMap.h"
#include "MainScene.h"
#include "AudioEngine.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
USING_NS_CC;

Scene* MainMenu::createScene()
{
    return MainMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
	TextureCache::getInstance()->removeAllTextures();

    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = ui::Button::create("bgMainMenu.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
	background->addTouchEventListener(CC_CALLBACK_1(MainMenu::playGameCallback, this));

	auto playButton = MenuItemImage::create("button_play.png", "button_play_Pressed.png", CC_CALLBACK_1(MainMenu::playGameCallback, this));
	playButton->setScale(0.7f);
	playButton->setPosition(Vec2(visibleSize.width * 0.2, -visibleSize.height * 0.5));
	
	auto menu = Menu::create(playButton,nullptr);
	menu->setPosition(Vec2(visibleSize.width*2/ 3  + origin.x, visibleSize.height * 2/3 + origin.y));

	Label* touchToPlay = Label::create("touch to play", "", 24);
	touchToPlay->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.2f));
	FadeTo* fadeIn = FadeTo::create(0.5f, 255);
	FadeTo* fadeOut = FadeTo::create(0.5f, 100);
	Sequence* blink = Sequence::create(fadeOut, fadeIn, nullptr);
	touchToPlay->runAction(RepeatForever::create(blink));

	this->addChild(menu, 1);
	this->addChild(background,0);
	this->addChild(touchToPlay, 1);
	
    return true;
}

void MainMenu::playGameCallback(cocos2d::Ref* pSender) {
	auto Scene = SelectMap::create();
	experimental::AudioEngine::play2d("click.mp3");
	Director::getInstance()->replaceScene(TransitionFade::create(0.4, Scene, Color3B(0, 0, 0)));
	//Director::getInstance()->replaceScene(Scene);
	//CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

