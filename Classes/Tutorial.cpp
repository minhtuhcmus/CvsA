#include "Tutorial.h"
#include "GameplayTutorial.h"
#include "ObjectTutorial.h"

Tutorial::Tutorial(string levelName, Node* parent)
{
	this->autorelease();
	_firstTutorial = nullptr;
	Vec2 visibleSize = Director::getInstance()->getVisibleSize();

	levelName.erase(0, 5);
	int levelIndex = atoi(levelName.c_str());
	switch (levelIndex)
	{
	case 1: {
		GameplayTutorial* aim = new GameplayTutorial(
			Vec2(visibleSize.x*0.5, visibleSize.y*0.75),
			Vec2(visibleSize.x*0.75, visibleSize.y*0.1),
			false,
			nullptr,
			this
		);
		_firstTutorial = aim;
		break;
	}
	case 2: {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/ChickBouncing.plist");
		//ObjectTutorial* bouncing = new ObjectTutorial(
		//	Vec2(visibleSize.x*0.45, visibleSize.y*0.8),
		//	120,
		//	"ChickBouncing",
		//	"The Chick will bounce and keep flying.\nThe White Line is the flying path.",
		//	nullptr,
		//	parent
		//);
		GameplayTutorial* drag = new GameplayTutorial(
			Vec2(visibleSize.x*0.5, visibleSize.y*0.5),
			Vec2(visibleSize.x*0.45, visibleSize.y*0.8),
			true,
			nullptr,
			this
		);
		_firstTutorial = drag;
		break;
	}
	case 3: {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/ChickRoasting.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/PanBouncing.plist");
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/PanRotate.plist");
		GameplayTutorial* shoot = new GameplayTutorial(
			Vec2(visibleSize.x*0.1, visibleSize.y*0.1),
			Vec2(visibleSize.x*0.45, visibleSize.y*0.4),
			false,
			nullptr,
			this
		);
		ObjectTutorial* clickThePan = new ObjectTutorial(
			Vec2(visibleSize.x*0.45, visibleSize.y*0.5),
			180,
			"PanRotate",
			"Touch the pan to rotate it.",
			shoot,
			parent
		);
		ObjectTutorial* pan = new ObjectTutorial(
			Vec2(visibleSize.x*0.45, visibleSize.y*0.5),
			180,
			"PanBouncing",
			"This is The Pan. \nChick will bounce when he touch it, \nlike ground and wall.",
			clickThePan,
			parent
		);
		ObjectTutorial* lava = new ObjectTutorial(
			Vec2(visibleSize.x*0.2, visibleSize.y*0.9),
			80,
			"ChickRoasting",
			"This is Lava. Chick will be roasted when he touch it.",
			pan,
			parent
		);
		_firstTutorial = lava;
		break;
	}
	case 8: {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/TNTExplode.plist");
		ObjectTutorial* tnt = new ObjectTutorial(
			Vec2(visibleSize.x*0.51, visibleSize.y*0.35),
			-90,
			"TNTExplode",
			"This is TNT. Shoot it to kill bounch of enemies.",
			nullptr,
			parent
		);
		_firstTutorial = tnt;
		break;
	}
	case 11: {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/RockTutorial.plist");
		ObjectTutorial* tnt = new ObjectTutorial(
			Vec2(visibleSize.x*0.52, visibleSize.y*0.5),
			-90,
			"RockTutorial",
			"This is TNT. It's only thing can move the rock.",
			nullptr,
			parent
		);
		_firstTutorial = tnt;
		break;
	}
	case 12: {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorial/ChickenEater.plist");
		ObjectTutorial* chickenEater = new ObjectTutorial(
			Vec2(visibleSize.x*0.1, visibleSize.y*0.2),
			-130,
			"ChickenEater",
			"This is Chicken Eater. Don't let Chick fly to them.",
			nullptr,
			parent
		);
		_firstTutorial = chickenEater;
		break;
	}
	default:
		break;
	}

	parent->addChild(this);
}


Tutorial::~Tutorial()
{
}

void Tutorial::setEnable() {
	if (_firstTutorial == nullptr) return;
	_firstTutorial->setEnabled(true);
	_firstTutorial->setVisible(true);
}