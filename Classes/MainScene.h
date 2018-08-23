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

#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "SelectMap.h"
#include "Store.h"
class MainScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int currPage);

	virtual bool init();
	LayerColor* layer;
	Sprite* shopCoin; 
	Sprite* shopHeart;
	// a selector callback
	void MenuCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);
private:
	MyButton* _nextButton;
	MyButton* _previousButton;
	MyButton* _buttonClose;
	ui::PageView* _pageView;
	ui::Button* _popupBG;
	int _coin;
	Label *_heartLabel;
	Label *_timeLabel;
	Label *_coinLabel;
	UserDefault* def;
	vector<int> coinToDollarArr;
	vector<float> dollarArr;
	vector<int> coinToHeartArr;
	vector<int> heartArr;

	void next(Ref* sender, ui::Widget::TouchEventType type);
	void previous(Ref* sender, ui::Widget::TouchEventType type);
	void close(Ref* sender, ui::Widget::TouchEventType type);
	void reset(Ref* sender, ui::Widget::TouchEventType type);
	void store(Ref* sender,ui::Widget::TouchEventType type);
	void plusHeart(Ref* sender, ui::Widget::TouchEventType type);
	void plusCoin(Ref* sender, ui::Widget::TouchEventType type);
	void buyCoin(Ref* sender, ui::Widget::TouchEventType type);
	void buyHeart(Ref* sender, ui::Widget::TouchEventType type);
	void back(Ref* sender, ui::Widget::TouchEventType type);
	void checkRemoveSure();
	void onClickYesPopupRemove(Ref* sender, ui::Widget::TouchEventType type);
	void onClickNoPopup(Ref* sender, ui::Widget::TouchEventType type);
	void update(float dt);
};

#endif // __MAINSCENE_H__
