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

#include "MainScene.h"
#include "AudioEngine.h"
#include <ctime>
#include "MyButton.h"

#define MENU_ITEMS_ACROSS 3
#define MENU_ITEMS_DOWN 2
#define MENU_ITEMS_PAGE (MENU_ITEMS_ACROSS*MENU_ITEMS_DOWN)
#define MENU_ITEMS_TOTAL 6

#define MENU_FRACTION (Vec2(0.8,0.8))
#define MENU_ANCHOR (Vec2(0.5,0.5))

#define SLIDE_DURATION 1.0

#define TOL_SEC 1500
#define HEART 5
#define MIN 2
#define SEC 0

int _currentPage;
float _min = 0;
float _sec = 5;
int _heart = 3;
int _coin;
float _timeToUpdate = 1.0f;
int MENU_PAGES = 5;
Scene* MainScene::createScene(int currentPage)
{
	_currentPage = currentPage;
	return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
static string timeToString(float min,float sec) {
	string s;
	if (min < 10) {
		s = "0" + to_string(int(min));
	}
	else {
		s = to_string(int(min));
	}
	if (sec < 10) {
		s = s + ":0" + to_string(int(sec));
	}
	else {
		s = s + ":" + to_string(int(sec));
	}
	return s;
}

static void updateHeart(int oldTime,int newTime) {
	int secOfSystem = newTime - oldTime;
	//int secOfGame = (_heart * HEART + _min) * 60;
	/*_heart = oldTime / (60 * HEART);
	_min = (oldTime % (60 * HEART)) / 60;
	_sec = (oldTime % (60 * HEART)) % 60;*/
	if (_heart >= HEART) {
		_min = MIN;
		_sec = SEC;
	}
	else {
		if (secOfSystem != 0) {
			_heart = _heart + (secOfSystem / (60 * HEART));
			_min = _min - (secOfSystem % (60 * HEART)) / 60;
			_sec = _sec - (secOfSystem % (60 * HEART)) % 60;
			if (_sec < 0) {
				_sec = 60 + _sec;
				_min--;
			}
			if (_min < 0) {
				_min = MIN + _min;
				_heart++;
			}
			if (_heart >= HEART) {
				_heart = HEART;
				_min = MIN;
				_sec = SEC;
			}

		}
	}
	
}
static Point CalculatePosition(int itemNum,Size scrSize)
{

	//auto scrSize = Director::getInstance()->getVisibleSize();
	float Xs = scrSize.width;
	float Ys = scrSize.height;
	int gRows = MENU_ITEMS_DOWN;
	int gCols = MENU_ITEMS_ACROSS;
	int gBins = gRows*gCols;
	float Xb = MENU_FRACTION.x*Xs / gCols;
	float Yb = MENU_FRACTION.y*Ys / gRows;
	float Xa = MENU_ANCHOR.x * Xs;
	float Ya = MENU_ANCHOR.y * Ys;
	int page = itemNum / gBins;

	int binCol = itemNum % gCols;
	int binRow = (itemNum - page*gBins) / gCols;

	float xPos = binCol * Xb + Xb / 2 + Xa - MENU_FRACTION.x*Xs / 2 + page * Xs;
	float yPos = Ya - binRow*Yb - Yb / 2 + MENU_FRACTION.y * Ys / 2;

	Point pos = Point(xPos, yPos);

	return pos;
}


// on "init" you need to initialize your instance


bool MainScene::init()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgMusic.mp3");
	TextureCache::getInstance()->removeAllTextures();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/ChickBouncing.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/ChickRoasting.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/PanBouncing.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/PanRotate.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/TNTExplode.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/RockTutorial.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("tutorial/ChickenEater.plist");

	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	experimental::AudioEngine::stopAll();
	experimental::AudioEngine::play2d("bgMusic.mp3",true);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	layer = LayerColor::create(Color4B::BLACK);
	layer->setOpacity(225);
	layer->setVisible(false);
	this->addChild(layer, 5);

	// Start: Shop Coin
	shopCoin = Sprite::create("bgShop.png");
	shopCoin->setContentSize(visibleSize * 3 / 4);
	auto scrollView = ui::ScrollView::create();
	scrollView->setContentSize(shopCoin->getContentSize() * 7 / 8);
	scrollView->setInnerContainerSize(Size(shopCoin->getContentSize().width*2,shopCoin->getContentSize().height));
	//scrollView->setBackGroundImage("bgShop.png");
	//scrollView->setBounceEnabled(true);
	scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setAnchorPoint(Vec2(0.5, 0.5));
	scrollView->setPosition(shopCoin->getContentSize() / 2);
	shopCoin->setPosition(visibleSize / 2);
	shopCoin->setVisible(false);
	shopCoin->addChild(scrollView);

	coinToDollarArr = { 100,220,500,1100,1500,5000 };
	dollarArr = { 0.99f,1.99f,3.99f,7.99f,9.99f,39.99f };

	for (int i = 0; i < coinToDollarArr.size(); i++) {
		//auto position = CalculatePosition(i, scrollView->getContentSize());
		auto position = Vec2(scrollView->getContentSize().width*i/3 + shopCoin->getContentSize().width/6,shopCoin->getContentSize().height/2);
		MyButton* button = new MyButton(position,"ShopCoin/BuyCoin.png","ShopCoin/BuyCoin_Pressed.png",scrollView);
		button->setTag(i);
		button->setAnchorPoint(Vec2(0.5,0.4));
		button->addTouchEventListener(CC_CALLBACK_2(MainScene::buyCoin,this));
		auto labelDollar = Label::createWithTTF("$" + to_string(dollarArr.at(i)).substr(0,4), "fonts/zrnic rg.ttf", button->getContentSize().height / 12);
		labelDollar->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height * 1 / 7));
		labelDollar->setColor(Color3B(252, 251, 162));
		//labelDollar->enableBold();
		
		auto labelCoin = Label::createWithTTF(to_string((int)coinToDollarArr.at(i))+" Coins", "fonts/zrnic rg.ttf", button->getContentSize().height/12);
		labelCoin->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height * 7 / 8));
		labelCoin->setColor(Color3B(104,44,34));
		labelCoin->enableBold();

		button->addChild(labelCoin,1);
		button->addChild(labelDollar, 1);

		if (i != 0) {
			auto sprite = Sprite::create("ShopCoin/CoinImage" + to_string(i) + ".png");
			sprite->setPosition(button->getContentSize()/2);
			button->addChild(sprite,0);
		}
		//button->setScale(0.9);
	}
	this->addChild(shopCoin, 6);

	_buttonClose = new MyButton(Vec2(shopCoin->getPosition().x + shopCoin->getContentSize().width/2, shopCoin->getPosition().y + shopCoin->getContentSize().height / 2),"Close.png","Close_Pressed.png",this,7);
	_buttonClose->setAnchorPoint(Vec2(1.1,1));
	_buttonClose->addTouchEventListener(CC_CALLBACK_2(MainScene::close,this));
	_buttonClose->setVisible(false);
	//End: Shop Coin

	// Start: Shop Heart
	shopHeart = Sprite::create("bgShop.png");
	shopHeart->setContentSize(visibleSize * 3 / 4);
	auto scrollViewHeart = ui::ScrollView::create();
	scrollViewHeart->setContentSize(shopHeart->getContentSize() * 7 / 8);
	scrollViewHeart->setInnerContainerSize(Size(shopHeart->getContentSize().width *4/3, shopHeart->getContentSize().height));
	//scrollViewHeart->setBackGroundImage("bgShop.png");
	//scrollView->setBounceEnabled(true);
	scrollViewHeart->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	scrollViewHeart->setAnchorPoint(Vec2(0.5, 0.5));
	scrollViewHeart->setPosition(shopHeart->getContentSize() / 2);
	shopHeart->setPosition(visibleSize / 2);
	shopHeart->setVisible(false);
	shopHeart->addChild(scrollViewHeart);

	coinToHeartArr = { 50,100,200};
	heartArr = { 1,3,6 };

	for (int i = 0; i < coinToHeartArr.size(); i++) {
		//auto position = CalculatePosition(i, scrollView->getContentSize());
		auto position = Vec2(scrollViewHeart->getContentSize().width*i / 3 + shopHeart->getContentSize().width / 6, shopHeart->getContentSize().height / 2);
		MyButton* button = new MyButton(position, "ShopCoin/BuyHeart.png", "ShopCoin/BuyHeart_Pressed.png", scrollViewHeart);
		button->setTag(i);
		button->setAnchorPoint(Vec2(0.5, 0.4));
		button->addTouchEventListener(CC_CALLBACK_2(MainScene::buyHeart, this));
		auto labelBuy = Label::createWithTTF(to_string(coinToHeartArr.at(i)) + "coins", "fonts/zrnic rg.ttf", button->getContentSize().height / 12);
		labelBuy->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height * 1 / 7));
		labelBuy->setColor(Color3B(252, 251, 162));
		//labelDollar->enableBold();

		auto labelHeart = Label::createWithTTF(to_string((int)heartArr.at(i)) + " Hearts", "fonts/zrnic rg.ttf", button->getContentSize().height / 12);
		labelHeart->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height * 7 / 8));
		labelHeart->setColor(Color3B(104, 44, 34));
		labelHeart->enableBold();

		button->addChild(labelHeart, 1);
		button->addChild(labelBuy, 1);

		auto sprite = Sprite::create("ShopCoin/HeartImage" + to_string(i) + ".png");
		sprite->setPosition(button->getContentSize() / 2);
		button->addChild(sprite, 0);

		//button->setScale(0.9);
	}
	this->addChild(shopHeart, 6);

	_buttonClose = new MyButton(Vec2(shopCoin->getPosition().x + shopCoin->getContentSize().width / 2, shopCoin->getPosition().y + shopCoin->getContentSize().height / 2), "Close.png", "Close_Pressed.png", this, 7);
	_buttonClose->setAnchorPoint(Vec2(1.1, 1));
	_buttonClose->addTouchEventListener(CC_CALLBACK_2(MainScene::close, this));
	_buttonClose->setVisible(false);
	//End: Shop Heart

	def = UserDefault::getInstance();

	auto background = Sprite::create("bgMainScene.png");
	background->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);

	_pageView = ui::PageView::create();
	_pageView->setContentSize(visibleSize);
	
	_heart = def->getIntegerForKey("heart");
	if (_heart == 0) {
		_heart = HEART;
	}
	if (_heart < 0) {
		_heart = 0;
	}
	_min = def->getIntegerForKey("min");
	_sec = def->getIntegerForKey("sec");
	time_t now = time(0);
	int oldtime = def->getIntegerForKey("time");
	if (oldtime == NULL) {
		oldtime = now;
	}
	updateHeart(oldtime, now);
	//def->setIntegerForKey("time", time(0));
	
	for (int i = 0; i < MENU_PAGES; i++)
	{
		auto pMenu = Menu::create();
		pMenu->setPosition(Vec2(0, 0));

		auto page = ui::Layout::create();
		page->setContentSize(_pageView->getContentSize());
		for (int idx = i*MENU_ITEMS_TOTAL; idx < MENU_ITEMS_TOTAL*(i + 1); idx++)
		{
			string str = "level" + to_string(idx + 1) + "star";
			const char *keyStar = str.c_str();
			int star = def->getIntegerForKey(keyStar);
			string str1 = "level" + to_string(idx) + "isEnable";
			const char *keyIE = str1.c_str();
			bool isEnable = def->getIntegerForKey(keyIE);


			auto normalLevel = Sprite::create("star" + to_string(star) + ".png");
			auto selectedLevel = Sprite::create("star" + to_string(star) + ".png");
			auto disLevel = Sprite::create("disLevel.png");
			//selectedLevel->setColor({ 127, 127, 127 });
			selectedLevel->setScale(1.1f);

			/*int star = m_document[key]["star"].GetInt();*/
			auto pItem = MenuItemSprite::create(normalLevel, selectedLevel, disLevel, CC_CALLBACK_1(MainScene::MenuCallback, this));
			//auto pItem = MenuItemImage::create("star0.png", "star0.png", "disLevel.png", CC_CALLBACK_1(MainScene::MenuCallback, this));
			pItem->setScale(0.5);
			pItem->setTag(idx + 1);
			auto position = CalculatePosition(idx % MENU_ITEMS_TOTAL,visibleSize);
			pItem->setPosition(position);

			//Start :: Add level number
			auto normalLabel = Label::createWithTTF(to_string(idx + 1), "fonts/CarnevaleeFreakshow.ttf", pItem->getContentSize().height / 2);
			normalLabel->setAnchorPoint(Vec2(0.5, 0.55));
			normalLabel->setColor(Color3B(94, 36, 14));
			normalLabel->setRotation(-8);
			normalLabel->setPosition(pItem->getContentSize() / 2);
			normalLevel->addChild(normalLabel);

			auto selectedLabel = Label::createWithTTF(to_string(idx + 1), "fonts/CarnevaleeFreakshow.ttf", pItem->getContentSize().height / 2);
			selectedLabel->setAnchorPoint(Vec2(0.5, 0.55));
			selectedLabel->setColor(Color3B(94, 36, 14));
			selectedLabel->setRotation(-8);
			selectedLabel->setPosition(pItem->getContentSize() / 2);
			selectedLevel->addChild(selectedLabel);
			// End :: Add level number
			
			


			if (isEnable == false && idx != 0) {
				pItem->setEnabled(false);
				normalLabel->setOpacity(0);

			}
			else {
				pItem->setEnabled(true);
				normalLabel->setOpacity(255);
			}

			//pItem->addChild(label);
			pMenu->addChild(pItem);
		}
		page->addChild(pMenu, 0);
		_pageView->insertPage(page, i);
	}
	

	_nextButton = new MyButton(Vec2(visibleSize.width*19/20, visibleSize.height / 2), "NextMenu.png", "NextMenu_Pressed.png", this,2);
	_nextButton->addTouchEventListener(CC_CALLBACK_2(MainScene::next, this));

	_previousButton = new MyButton(Vec2(visibleSize.width*1/20, visibleSize.height / 2), "PreviousMenu.png", "PreviousMenu_Pressed.png", this,2);
	_previousButton->addTouchEventListener(CC_CALLBACK_2(MainScene::previous, this));

	MyButton* _storeButton = new MyButton(Vec2(visibleSize.width*0.5, visibleSize.height*14/15), "Shop.png", "Shop_Pressed.png", this,2);
	_storeButton->addTouchEventListener(CC_CALLBACK_2(MainScene::store, this));

	MyButton* _resetButton = new MyButton(Vec2(visibleSize.width / 20, visibleSize.height / 15), "UI/menu/buttons/Retry.png", "UI/menu/buttons/Retry_Pressed.png", this, 2);
	_resetButton->addTouchEventListener(CC_CALLBACK_2(MainScene::reset, this));

	MyButton* _backButton = new MyButton(Vec2(visibleSize.width/20,visibleSize.height*14/15),"UI/menu/buttons/Back.png","UI/menu/buttons/Back_Pressed.png",this,2);
	_backButton->addTouchEventListener(CC_CALLBACK_2(MainScene::back,this));

	//Start: Heart
	auto heartSprite = ui::Button::create("Heart.png", "Heart.png");
	heartSprite->setPosition(Vec2(visibleSize.width*9/10, visibleSize.height*14/15));
	heartSprite->setScale(0.65);
	heartSprite->addTouchEventListener(CC_CALLBACK_2(MainScene::plusHeart, this));

	auto plusHeart = new MyButton(Vec2(heartSprite->getContentSize().width, heartSprite->getContentSize().height / 2), "Plus.png", "Plus_Pressed.png", heartSprite);
	plusHeart->setAnchorPoint(Vec2(1, 0.55));
	plusHeart->addTouchEventListener(CC_CALLBACK_2(MainScene::plusHeart,this));

	_heartLabel = Label::createWithTTF(to_string(_heart), "fonts/CarnevaleeFreakshow.ttf", heartSprite->getContentSize().height*9/10);
	_heartLabel->setAnchorPoint(Vec2(0.85, 0.5));
	_heartLabel->setPosition(Vec2(heartSprite->getContentSize().width / 5, heartSprite->getContentSize().height / 2));
	_heartLabel->setScale(0.8);
	heartSprite->addChild(_heartLabel,1);

	_timeLabel = Label::createWithTTF(timeToString(_min,_sec), "fonts/CarnevaleeFreakshow.ttf", heartSprite->getContentSize().height*4/7);
	_timeLabel->setAnchorPoint(Vec2(1, 0.5));
	_timeLabel->setPosition(Vec2(heartSprite->getContentSize().width*5/7, heartSprite->getContentSize().height / 2));
	heartSprite->addChild(_timeLabel,1);
	//End: Heart

	// Start: Coin 
	auto coinSprite = ui::Button::create("Coin.png", "Coin.png");
	coinSprite->setPosition(Vec2(visibleSize.width * 7 / 10, visibleSize.height * 14 / 15));
	coinSprite->setScale(0.65);
	coinSprite->addTouchEventListener(CC_CALLBACK_2(MainScene::plusCoin, this));

	auto plusCoin = new MyButton(Vec2(coinSprite->getContentSize().width, coinSprite->getContentSize().height/2),"Plus.png","Plus_Pressed.png", coinSprite);
	plusCoin->setAnchorPoint(Vec2(1,0.55));
	plusCoin->addTouchEventListener(CC_CALLBACK_2(MainScene::plusCoin,this));

	_coin = def->getIntegerForKey("coin");
	_coinLabel = Label::createWithTTF(to_string(_coin), "fonts/CarnevaleeFreakshow.ttf", coinSprite->getContentSize().height*2/3);
	_coinLabel->setAnchorPoint(Vec2(1,0.5));
	_coinLabel->setPosition(Vec2(coinSprite->getContentSize().width*4/5, coinSprite->getContentSize().height / 2));
	coinSprite->addChild(_coinLabel);
	// End: Coin 
	_pageView->setCurrentPageIndex(def->getIntegerForKey("curPage"));
	this->addChild(heartSprite,6);
	this->addChild(coinSprite,6);
	this->addChild(background, 0);
	this->addChild(_pageView, 1);
	this->scheduleUpdate();

	//remove(def->getXMLFilePath().c_str());
	return true;
}
void MainScene::update(float dt) {
	if (_pageView->getCurPageIndex() == 0) {
		_previousButton->setEnabled(false);
	}
	else if (_pageView->getCurPageIndex() == (MENU_PAGES - 1)) {
		_nextButton->setEnabled(false);
	}
	else {
		_nextButton->setEnabled(true);
		_previousButton->setEnabled(true);
	}

	_timeToUpdate -= dt;
	if (_timeToUpdate >= 0) return;
	_timeToUpdate = 1.0f;

	
	_timeLabel->setString(timeToString(0, 0));
	_heartLabel->setString(to_string(_heart));
	if (_heart < HEART) {
		if (_min >= 0) {
			if (_sec > 0) {
				_sec -= _timeToUpdate;
				_timeLabel->setString(timeToString((int)_min, (int)_sec));
				_heartLabel->setString(to_string(_heart));
			}
			else {
				_min--;
				_sec = 59;
			}
		}
		else {
			_heart++;
			_min = MIN;
			_sec = 0;
			_timeLabel->setString(timeToString((int)_min, (int)_sec));
			_heartLabel->setString(to_string(_heart));
		}
	}
	else {
		_min = MIN;
		_sec = SEC;
		_timeLabel->setString(timeToString(0, 0));
		_heartLabel->setString(to_string(_heart));
	}
	if (_heart == 0) {
		UserDefault::getInstance()->setIntegerForKey("heart", -1);
	}
	else {
		UserDefault::getInstance()->setIntegerForKey("heart", _heart);
	}
	UserDefault::getInstance()->setIntegerForKey("min", _min);
	UserDefault::getInstance()->setIntegerForKey("sec", _sec);
	UserDefault::getInstance()->setIntegerForKey("curPage", _pageView->getCurPageIndex());
	UserDefault::getInstance()->flush();
}
void MainScene::next(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		if (_pageView->getCurPageIndex() < (MENU_PAGES - 1)) {
			_currentPage = (int)_pageView->getCurPageIndex();
			_currentPage++;
			_pageView->scrollToItem(_currentPage);
		}
		break;
	default:
		break;
	}
}
void MainScene::buyHeart(Ref* sender, ui::Widget::TouchEventType type) {
	MyButton* button = (MyButton*)sender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		if (_coin > coinToHeartArr.at(button->getTag())) {
			experimental::AudioEngine::play2d("buy.mp3");
			_heart += heartArr.at(button->getTag());
			_heartLabel->setString(to_string(_heart));
			//_heart = 0;
			def->setIntegerForKey("heart", _heart);

			_coin -= coinToHeartArr.at(button->getTag());
			_coinLabel->setString(to_string(_coin));
			def->setIntegerForKey("coin", _coin);
			def->flush();
		}
		else {
			experimental::AudioEngine::play2d("error.mp3");
		}
		break;
	default:
		break;
	}
}
void MainScene::buyCoin(Ref* sender, ui::Widget::TouchEventType type) {
	MyButton* button = (MyButton*)sender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("buy.mp3");
		_coin += coinToDollarArr.at(button->getTag());
		def->setIntegerForKey("coin",_coin);
		_coinLabel->setString(to_string(_coin));
		def->flush();
		break;
	default:
		break;
	}
}
void MainScene::previous(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;
	case ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		if (_pageView->getCurPageIndex() > 0) {
			_currentPage = (int)_pageView->getCurPageIndex();
			_currentPage--;
			_pageView->scrollToItem(_currentPage);
		}
		
		break;
	default:
		break;
	}
}
void MainScene::plusHeart(Ref* sender, ui::Widget::TouchEventType type) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		_pageView->setVisible(false);
		shopCoin->setVisible(false);
		shopHeart->setVisible(true);
		layer->setVisible(true);
		_buttonClose->setVisible(true);
		break;
	default:
		break;
	}
}
void MainScene::plusCoin(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		_pageView->setVisible(false);
		shopHeart->setVisible(false);
		shopCoin->setVisible(true);
		layer->setVisible(true);
		_buttonClose->setVisible(true);
		break;
	default:
		break;
	}
}
void  MainScene::close(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		_pageView->setVisible(true);
		_buttonClose->setVisible(false);
		shopCoin->setVisible(false);
		shopHeart->setVisible(false);
		layer->setVisible(false);
		break;
	default:
		break;
	}
}
void MainScene::store(Ref* sender,ui::Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		if (_heart == 0) {
			def->setIntegerForKey("heart", -1);
		}
		else {
			def->setIntegerForKey("heart", _heart);
		}
		def->setIntegerForKey("time", time(0));
		UserDefault::getInstance()->setIntegerForKey("min", _min);
		UserDefault::getInstance()->setIntegerForKey("sec", _sec);
		UserDefault::getInstance()->setIntegerForKey("curPage", _pageView->getCurPageIndex());
		UserDefault::getInstance()->flush();

		Director::getInstance()->replaceScene(TransitionFade::create(0.5,Store::createScene()));
		break;
	default:
		break;
	}
}
void MainScene::reset(Ref* sender,ui::Widget::TouchEventType type){
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		checkRemoveSure();
	default:
		break;
	}
}
void MainScene::checkRemoveSure() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_popupBG = ui::Button::create("UI/buttons/FullsceneButton.png");
	_popupBG->setOpacity(0);
	_popupBG->setPosition(visibleSize / 2);
	_popupBG->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickNoPopup, this));
	auto _popupTable = Sprite::create("UI/Border.png");
	_popupTable->setScale(0.5f);
	_popupTable->setPosition(visibleSize / 2);
	_popupBG->addChild(_popupTable);
	this->addChild(_popupBG, 998);

	Label* popupLabel = Label::createWithTTF("You will remove all data.\n       Are you sure?", "fonts/CarnevaleeFreakshow.ttf", 50);
	popupLabel->setColor(Color3B(94, 36, 14));
	popupLabel->setPosition(_popupTable->getContentSize() / 2);
	_popupTable->addChild(popupLabel);

	auto _yesPopup = ui::Button::create("UI/buttons/Yes.png", "UI/buttons/Yes_Pressed.png");
	_yesPopup->setScale(2.0f);
	_yesPopup->setPosition(Vec2(_popupTable->getContentSize().width * 0.35, 0));
	_yesPopup->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickYesPopupRemove, this));
	_popupTable->addChild(_yesPopup);

	auto _noPopup = ui::Button::create("UI/buttons/No.png", "UI/buttons/No_Pressed.png");
	_noPopup->setScale(2.0f);
	_noPopup->setPosition(Vec2(_popupTable->getContentSize().width * 0.65, 0));
	_noPopup->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickNoPopup, this));
	_popupTable->addChild(_noPopup);
}
void MainScene::onClickYesPopupRemove(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::ENDED: {
		const char* path = def->getXMLFilePath().c_str();
		MessageBox(path, "a");
		remove(path);
		remove(UserDefault::sharedUserDefault()->getXMLFilePath().c_str());
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainScene::createScene(0)));
	}
	default:
		break;
	}
}
void MainScene::onClickNoPopup(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		_popupBG->removeFromParentAndCleanup(1);
		break;
	default:
		break;
	}
}
void MainScene::back(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		if (_heart == 0) {
			def->setIntegerForKey("heart", -1);
		}
		else {
			def->setIntegerForKey("heart", _heart);
		}
		def->setIntegerForKey("time", time(0));
		UserDefault::getInstance()->setIntegerForKey("min", _min);
		UserDefault::getInstance()->setIntegerForKey("sec", _sec);
		UserDefault::getInstance()->setIntegerForKey("curPage", _pageView->getCurPageIndex());
		UserDefault::getInstance()->flush();

		Director::getInstance()->replaceScene(TransitionFade::create(0.5, SelectMap::createScene()));
		break;
	default:
		break;
	}
}
void MainScene::MenuCallback(cocos2d::Ref* pSender)
{

	MenuItem* pMenuItem = (MenuItem*)pSender;
	if (pMenuItem->getTag() > MAX_LEVEL) return;
	_currentPage = (pMenuItem->getTag() - 1) / MENU_ITEMS_TOTAL;
	bool hasPan = false;
	if (pMenuItem->getTag() > 2) {
		hasPan = true;
	}
	if (_heart == 0) {
		def->setIntegerForKey("heart", -1);
	}
	else {
		def->setIntegerForKey("heart", _heart);
	}
	def->setIntegerForKey("time", time(0));
	UserDefault::getInstance()->setIntegerForKey("min", _min);
	UserDefault::getInstance()->setIntegerForKey("sec", _sec);
	UserDefault::getInstance()->setIntegerForKey("curPage", _pageView->getCurPageIndex());
	UserDefault::getInstance()->flush();
	if (_heart > 0) {
		experimental::AudioEngine::play2d("click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, HelloWorld::createScene("level" + to_string(pMenuItem->getTag()), _currentPage, hasPan), Color3B(0, 0, 0)));
	}
	else {
		experimental::AudioEngine::play2d("error.mp3");
	}
}