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

#include "Store.h"
#include "MainScene.h"
#include "MyButton.h"
#include "json\rapidjson.h"
#include "json\document.h"
#include "AudioEngine.h"

using namespace rapidjson;

#define ARROW_LEFT (-1)
#define ARROW_RIGHT (-2)

#define MENU_ITEMS_ACROSS 5
#define MENU_ITEMS_DOWN 4
#define MENU_ITEMS_PAGE (MENU_ITEMS_ACROSS*MENU_ITEMS_DOWN)
#define MENU_ITEMS_TOTAL 20

#define MENU_PAGES 3
#define MENU_FRACTION (Vec2(0.8,0.8))
#define MENU_ANCHOR (Vec2(0.5,0.5))

#define SLIDE_DURATION 1.0

Label *coinLabel;
Label *labelQuantity;
int _pageIndex;
ui::PageView* _myPage;
vector<int> priceArr;
vector<string> nameArr;


Scene* Store::createScene()
{
	return Store::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool Store::init()
{
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("bgMusic.mp3");

	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Document m_document;
	ssize_t size;

	char* buf = (char*)FileUtils::getInstance()->getFileData("ShopItem.json","r" , &size);
	string content(buf,size);

	m_document.Parse(content.c_str());
	UserDefault* def = UserDefault::getInstance();

	layer = LayerColor::create(Color4B::BLACK);
	layer->setOpacity(225);
	layer->setVisible(false);
	this->addChild(layer, 6);

	// Start: Shop Coin
	shopCoin = Sprite::create("bgShop.png");
	shopCoin->setContentSize(visibleSize * 3 / 4);
	auto scrollView = ui::ScrollView::create();
	scrollView->setContentSize(shopCoin->getContentSize() * 7 / 8);
	scrollView->setInnerContainerSize(Size(shopCoin->getContentSize().width * 2, shopCoin->getContentSize().height));
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
		auto position = Vec2(scrollView->getContentSize().width*i / 3 + shopCoin->getContentSize().width / 6, shopCoin->getContentSize().height / 2);
		MyButton* button = new MyButton(position, "ShopCoin/BuyCoin.png", "ShopCoin/BuyCoin_Pressed.png", scrollView);
		button->setTag(i);
		button->setAnchorPoint(Vec2(0.5, 0.4));
		button->addTouchEventListener(CC_CALLBACK_2(Store::buyCoin, this));
		auto labelDollar = Label::createWithTTF("$" + to_string(dollarArr.at(i)).substr(0, 4), "fonts/zrnic rg.ttf", button->getContentSize().height / 12);
		labelDollar->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height * 1 / 7));
		labelDollar->setColor(Color3B(252, 251, 162));
		//labelDollar->enableBold();

		auto labelCoin = Label::createWithTTF(to_string((int)coinToDollarArr.at(i)) + " Coins", "fonts/zrnic rg.ttf", button->getContentSize().height / 12);
		labelCoin->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height * 7 / 8));
		labelCoin->setColor(Color3B(104, 44, 34));
		labelCoin->enableBold();

		button->addChild(labelCoin, 1);
		button->addChild(labelDollar, 1);

		if (i != 0) {
			auto sprite = Sprite::create("ShopCoin/CoinImage" + to_string(i) + ".png");
			sprite->setPosition(button->getContentSize() / 2);
			button->addChild(sprite, 0);
		}
		//button->setScale(0.9);
	}
	this->addChild(shopCoin, 6);

	_buttonClose = new MyButton(Vec2(shopCoin->getPosition().x + shopCoin->getContentSize().width / 2, shopCoin->getPosition().y + shopCoin->getContentSize().height / 2), "Close.png", "Close_Pressed.png", this, 7);
	_buttonClose->setAnchorPoint(Vec2(1.1, 1));
	_buttonClose->addTouchEventListener(CC_CALLBACK_2(Store::close, this));
	_buttonClose->setVisible(false);
	//End: Shop Coin

	// Start: Shop Item
	auto shopItem = Sprite::create("Border_Named.png");
	shopItem->setContentSize(visibleSize * 4 / 5);
	auto scrollView1 = ui::ScrollView::create();
	scrollView1->setContentSize(Size(shopItem->getContentSize().width * 7 / 8, shopItem->getContentSize().height * 0.7));
	scrollView1->setInnerContainerSize(Size(shopItem->getContentSize().width, shopItem->getContentSize().height*2));
	//scrollView1->setBackGroundImage("bgShop.png");
	//scrollView1->setBounceEnabled(true);
	scrollView1->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView1->setAnchorPoint(Vec2(0.5, 0.55));
	scrollView1->setPosition(shopItem->getContentSize() / 2);
	shopItem->setPosition(visibleSize / 2);
	//shopItem->setVisible(false);
	shopItem->addChild(scrollView1);

	Label* storeLabel = Label::createWithTTF("Store", "fonts/CarnevaleeFreakshow.ttf", 50);
	storeLabel->setColor(Color3B(94, 36, 14));
	storeLabel->setPosition(Vec2(shopItem->getContentSize().width / 2, shopItem->getContentSize().height * 0.92));
	shopItem->addChild(storeLabel);

	for (int i = 0; i < 5; i++) {
		//auto position = CalculatePosition(i, scrollView->getContentSize());
		string s = "item" + to_string(i + 1);
		const char *key = s.c_str();
		priceArr.push_back(m_document[key]["price"].GetInt());
		nameArr.push_back(m_document[key]["name"].GetString());

		auto position = Vec2(scrollView1->getContentSize().width/2, scrollView1->getInnerContainerSize().height - i*(shopItem->getContentSize().height/2.5));
		MyButton* button = new MyButton(position, "Shop/BorderItem.png", "Shop/BorderItem_Pressed.png", scrollView1);
		button->setEnabled(false);
		button->setTag(i);
		button->setAnchorPoint(Vec2(0.5, 1));
		//button->setScale(0.74);
		button->addTouchEventListener(CC_CALLBACK_2(Store::buyItem, this));
		auto labelPrice = Label::createWithTTF(to_string(priceArr.at(i)), "fonts/zrnic rg.ttf", button->getContentSize().height / 6);
		labelPrice->setPosition(Vec2(button->getContentSize().width *0.45, button->getContentSize().height *0.2));
		//labelPrice->setAnchorPoint(Vec2(0, 0.5));
		//labelPrice->setColor(Color3B(252, 251, 162));	
		labelPrice->enableBold();

		auto labelName = Label::createWithTTF(nameArr.at(i), "fonts/zrnic rg.ttf", button->getContentSize().height / 5);
		labelName->setPosition(Vec2(button->getContentSize().width /4, button->getContentSize().height * 5 / 6));
		labelName->setAnchorPoint(Vec2(0, 0.5));
		labelName->setColor(Color3B(67, 177, 93));
		labelName->enableShadow();
		labelName->enableBold();

		auto labelDescription = Label::createWithTTF(m_document[key]["description"].GetString(), "fonts/zrnic rg.ttf", button->getContentSize().height / 7);
		labelDescription->setPosition(Vec2(button->getContentSize().width/4, button->getContentSize().height * 3 / 5));
		labelDescription->setAnchorPoint(Vec2(0, 0.5));
		labelDescription->setColor(Color3B(94, 36, 14));

		labelQuantity = Label::createWithTTF(to_string(def->getIntegerForKey(nameArr.at(i).c_str())),"fonts/zrnic rg.ttf", button->getContentSize().height / 7);
		labelQuantity->setPosition(Vec2(button->getContentSize().width * 0.96,button->getContentSize().height*0.91));
		labelQuantity->enableBold();
		labelQuantity->setName(nameArr.at(i));

		button->addChild(labelQuantity, 1);
		button->addChild(labelPrice, 1);
		button->addChild(labelName, 1);
		button->addChild(labelDescription, 1);

		auto sprite = Sprite::create(m_document[key]["imageName"].GetString());
		sprite->setPosition(Vec2(button->getContentSize().width / 8, button->getContentSize().height / 2));
		button->addChild(sprite, 0);

		//button->setScale(0.9);
	}
	this->addChild(shopItem, 5);
	//End: Shop Item

	MyButton* _backButton = new MyButton(Vec2(visibleSize.width * 1 / 20, visibleSize.height * 14 / 15), "UI/shop/Back.png", "UI/shop/Back_Pressed.png", this);
	_backButton->addTouchEventListener(CC_CALLBACK_2(Store::back, this));
	// Start: Coin 
	auto coinSprite = Sprite::create("Coin.png");
	coinSprite->setPosition(Vec2(visibleSize.width * 9 / 10, visibleSize.height * 14 / 15));
	coinSprite->setScale(0.65);

	auto plusButton = new MyButton(Vec2(coinSprite->getContentSize().width, coinSprite->getContentSize().height / 2), "Plus.png", "Plus_Pressed.png", coinSprite);
	plusButton->addTouchEventListener(CC_CALLBACK_2(Store::plusCoin,this));
	plusButton->setAnchorPoint(Vec2(1, 0.55));

	int coin = def->getIntegerForKey("coin");
	coinLabel = Label::createWithTTF(to_string(coin), "fonts/CarnevaleeFreakshow.ttf", coinSprite->getContentSize().height * 2 / 3);
	coinLabel->setAnchorPoint(Vec2(1, 0.5));
	coinLabel->setPosition(Vec2(coinSprite->getContentSize().width * 4 / 5, coinSprite->getContentSize().height / 2));
	coinSprite->addChild(coinLabel);
	// End: Coin 
	/*_myPage->setEnabled(false);
	_myPage->setCurrentPageIndex(_pageIndex);*/
	this->addChild(coinSprite, 6);
	//this->addChild(background, 0);
	//this->addChild(_myPage, 1);

	return true;
}
void Store::update(float dt) {

}
void Store::back(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type) {
	case ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		Director::getInstance()->replaceScene(MainScene::createScene(UserDefault::getInstance()->getIntegerForKey("curPage")));
		break;
	default:
		break;
	}
}
void Store::buyItem(Ref* sender, ui::Widget::TouchEventType type) {
	MyButton* button = (MyButton*)sender;
	int coin = UserDefault::getInstance()->getIntegerForKey("coin");
	int quantity = UserDefault::getInstance()->getIntegerForKey(nameArr.at(button->getTag()).c_str());
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		if (coin >= priceArr.at(button->getTag())) {
			experimental::AudioEngine::play2d("buy.mp3");
			quantity++;
			coin -= priceArr.at(button->getTag());
			UserDefault::getInstance()->setIntegerForKey("coin", coin);
			UserDefault::getInstance()->setIntegerForKey(nameArr.at(button->getTag()).c_str(), quantity);
			Label* label = (Label*)button->getChildByName(nameArr.at(button->getTag()).c_str());
			label->setString(to_string(quantity));
			coinLabel->setString(to_string(coin));
		}
		else {
			experimental::AudioEngine::play2d("error.mp3");
		}
	default:
		break;
	}
}
void Store::buyCoin(Ref* sender, ui::Widget::TouchEventType type) {
	MyButton* button = (MyButton*)sender;
	int coin = UserDefault::getInstance()->getIntegerForKey("coin");
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("buy.mp3");
		coin += coinToDollarArr.at(button->getTag());
		UserDefault::getInstance()->setIntegerForKey("coin", coin);
		coinLabel->setString(to_string(coin));
		UserDefault::getInstance()->flush();
		break;
	default:
		break;
	}
}
void Store::plusCoin(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		shopCoin->setVisible(true);
		layer->setVisible(true);
		_buttonClose->setVisible(true);
		break;
	default:
		break;
	}
}
void  Store::close(Ref* sender, ui::Widget::TouchEventType type) {
	switch (type)
	{
	case ui::Widget::TouchEventType::ENDED:
		experimental::AudioEngine::play2d("click.mp3");
		_buttonClose->setVisible(false);
		shopCoin->setVisible(false);
		layer->setVisible(false);
		break;
	default:
		break;
	}
}