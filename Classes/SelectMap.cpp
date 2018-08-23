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

#include "SelectMap.h"
#include "AudioEngine.h"
USING_NS_CC;

#define TOL_MAP 3
Scene* SelectMap::createScene()
{
    return SelectMap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SelectMap::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("bgSelectMap.png");
	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
	background->setPosition(Vec2(visibleSize.width/2 + origin.x,visibleSize.height/2 + origin.y));

	auto scrollView = ui::ScrollView::create();
	scrollView->setContentSize(background->getContentSize());
	scrollView->setInnerContainerSize(Size(background->getContentSize().width*(((float)TOL_MAP/2)+ 0.5), background->getContentSize().height));
	scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setPosition(background->getContentSize() / 2);
	background->addChild(scrollView, 1);

	for (int i = 0; i < TOL_MAP; i++) {
		auto position = Vec2(scrollView->getContentSize().width * i / 2 + background->getContentSize().width/2, scrollView->getContentSize().height* (i%2+1)/3);
		MyButton* button = new MyButton(position, "Map/Map"+ to_string(i)+".png", "Map/Map" + to_string(i) + "_Selected.png", scrollView);
		button->setScale(2.5);
		if(i == 0)
			button->addTouchEventListener(CC_CALLBACK_2(SelectMap::onClickMap,this));
		button->setTag(i);
	}
	this->addChild(background,1);
    return true;
}
void SelectMap::onClickMap(Ref* sender, ui::Widget::TouchEventType type) {
	MyButton* button = (MyButton*)sender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		experimental::AudioEngine::play2d("click.mp3");
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScene::createScene(0)));
		break;
	default:
		break;
	}
}

