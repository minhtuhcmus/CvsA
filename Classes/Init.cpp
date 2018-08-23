#include "Init.h"
#include "PlayerController.h"
#include "Enemies.h"
#include "TNT.h"
#include "Lava.h"
#include "Rock.h"
#include "Alien.h"
#include "ChickenEater.h"
#include <string>
#include "Player.h"
#include "Pan.h"
#include "AnoyingFly.h"
#include "Canon.h"
#include "MovingGround.h"
#include "ChainSaw.h"
#include "Portal.h"

Sprite* initBackground(string path, Node* parent) {
	Sprite* background = Sprite::create(path);
	background->setTag(WALL);
	background->setOpacity(200);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	background->setPosition(visibleSize / 2);
	//PhysicsBody* backgroundBody = PhysicsBody::createEdgeBox(
	//	Size(visibleSize.width + 30, visibleSize.height + 30),
	//	PhysicsMaterial(0.0f, 1.0f, 0.0f),
	//	20
	//);
	//backgroundBody->setCategoryBitmask(WALL);
	//backgroundBody->setCollisionBitmask(BULLET + ENEMIES_PARTS + ROCK);
	//backgroundBody->setContactTestBitmask(BULLET);
	//backgroundBody->setDynamic(false);
	//backgroundBody->setGravityEnable(false);
	//backgroundBody->setRotationEnable(false);
	//backgroundBody->setContactTestBitmask(1);
	//background->addComponent(backgroundBody);
	parent->addChild(background);

	return background;
}

void initWall(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *wallObj = map->getObjectGroup("Wall");
	if (wallObj == nullptr) return;

	for (int i = 0; i < wallObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = wallObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		auto edgeSp = Sprite::create();
		edgeSp->setTag(WALL);
		edgeSp->setName("Wall" + to_string(i));

		auto boundBody = PhysicsBody::createBox(Size(wi_box, he_box), PhysicsMaterial(1.0f, 1.0f, 1.0f));
		boundBody->setDynamic(false);
		boundBody->setGroup(WALL);
		boundBody->setCategoryBitmask(WALL);
		boundBody->setCollisionBitmask(BULLET + ENEMIES_PARTS + ROCK + FAKE_BULLET);
		boundBody->setContactTestBitmask(BULLET);

		edgeSp->setPhysicsBody(boundBody);
		edgeSp->setPosition(Vec2(x_box, y_box));

		parent->addChild(edgeSp, 0); // Add obj into Layer
	}
}

Player* initPlayer(TMXTiledMap* map, int ammo, float scale, Layer* parent) {
	TMXObjectGroup *playerObj = map->getObjectGroup("Player");
	initTiltedGround(map, parent);
	if (playerObj == nullptr) return nullptr;
	if (playerObj->getObjects().size() == 0) return nullptr;

	cocos2d::Value objectemp = playerObj->getObjects().at(0);
	float wi_box = objectemp.asValueMap().at("width").asFloat();
	float he_box = objectemp.asValueMap().at("height").asFloat();
	float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
	float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;
	Player* player = new Player(Vec2(x_box, y_box), ammo, scale, parent);
	return player;
}

PlayerController* initPlayerController(Player* player, Node* parent) {
	PlayerController* controller = new PlayerController(parent);
	controller->setPlayer(player);
	return controller;
}

Enemies* initEnemies(TMXTiledMap* map, Layer* parent) {
	Enemies* enemiesHandler = new Enemies();
	parent->addChild(enemiesHandler);

	initAliens(map, enemiesHandler, parent);
	initChickenEaters(map, enemiesHandler, parent);
	initAnoyingFlies(map, enemiesHandler, parent);

	return enemiesHandler;
}

void initAliens(TMXTiledMap* map, Enemies* enemiesHandler, Layer* parent) {
	TMXObjectGroup *enemiesObj = map->getObjectGroup("Aliens");
	if (enemiesObj == nullptr) return;

	for (int i = 0; i < enemiesObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = enemiesObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		new Alien(Vec2(x_box, y_box), 0.15f, i, enemiesHandler, parent);
	}
}

void initPans(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *enemiesObj = map->getObjectGroup("Pans");
	if (enemiesObj == nullptr) return;

	for (int i = 0; i < enemiesObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = enemiesObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		new Pan(Vec2(x_box, y_box), i, parent);
	}
}

void initChickenEaters(TMXTiledMap* map, Enemies* enemiesHandler, Layer* parent) {
	TMXObjectGroup *enemiesObj = map->getObjectGroup("ChickenEaters");
	if (enemiesObj == nullptr) return;

	for (int i = 0; i < enemiesObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = enemiesObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		new ChickenEater(Vec2(x_box, y_box), 1.0f, i, enemiesHandler, parent);
	}
}

void initItems(TMXTiledMap* map, Layer* parent) {
	initTNT(map, parent);
	initLava(map, parent);
	initRock(map, parent);
	initCanon(map, parent);
	initMovingGround(map, parent);
	initChainSaw(map, parent);
	initPortal(map, parent);
}

void initTNT(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *itemsObj = map->getObjectGroup("TNT");
	if (itemsObj == nullptr) return;

	for (int i = 0; i < itemsObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = itemsObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;
		new TNT(Vec2(x_box, y_box), 0.3, i, parent);
	}
}

void initChainSaw(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *itemsObj = map->getObjectGroup("ChainSaws");
	if (itemsObj == nullptr) return;

	for (int i = 0; i < itemsObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = itemsObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;
		new ChainSaw(Vec2(x_box, y_box), 1.0f, i, parent);
	}
}

void initLava(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *itemsObj = map->getObjectGroup("Lava");
	if (itemsObj == nullptr) return;

	for (int i = 0; i < itemsObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = itemsObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		Lava* lava = new Lava(Vec2(x_box, y_box), 1, i, parent);

		DrawNode* draw = DrawNode::create();
		draw->drawSolidRect(Vec2(-wi_box / 2, -he_box / 2), Vec2(wi_box / 2, he_box / 2), Color4F(255,0,0,150));
		auto lavaFX = Sequence::create(FadeIn::create(0.5f), FadeOut::create(0.5f), nullptr);
		draw->runAction(RepeatForever::create(lavaFX));
		lava->addChild(draw);

		PhysicsBody* lavaBody = PhysicsBody::createBox(Size(wi_box, he_box));
		lavaBody->setGroup(LAVA);
		lavaBody->setCategoryBitmask(LAVA);
		lavaBody->setCollisionBitmask(BULLET);
		lavaBody->setContactTestBitmask(BULLET + FAKE_BULLET);
		lavaBody->setDynamic(false);

		lava->addComponent(lavaBody);
	}
}

void initRock(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *itemsObj = map->getObjectGroup("Rock");
	if (itemsObj == nullptr) return;

	for (int i = 0; i < itemsObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = itemsObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		new Rock(Vec2(x_box, y_box), 1, i, parent);
	}
}

void initCanon(TMXTiledMap* map, Layer* parent) {
	TMXObjectGroup *itemsObj = map->getObjectGroup("Canons");
	if (itemsObj == nullptr) return;

	for (int i = 0; i < itemsObj->getObjects().size(); i++)
	{

		cocos2d::Value objectemp = itemsObj->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		new Canon(Vec2(x_box, y_box), 1, i, parent);
	}
}

void initPortal(TMXTiledMap* map, Layer* parent) {
	auto objectGroup = map->getObjectGroup("Portals");
	if (objectGroup == nullptr) return;
	Portal* tempPortal;
	auto objects = objectGroup->getObjects();
	for (int i = 0; i < objects.size(); i++)
	{

		auto dic = objects.at(i).asValueMap();
		float objectX = dic.at("x").asFloat();
		float objectY = dic.at("y").asFloat();


		auto pointsVector = dic.at("polylinePoints").asValueVector();
		auto size = pointsVector.size();
		// Get Point 
		if (size > 0)
		{
			Vec2* points = new Vec2[size];
			int j = 0;
			for (auto pointValue : pointsVector)
			{
				auto dicp = pointValue.asValueMap();
				auto x = dicp.at("x").asFloat();
				auto y = -dicp.at("y").asFloat(); // y takes a negative value 
				points[j] = Vec2(x, y);
				j++;
			}

			cocos2d::Value objectemp = objectGroup->getObjects().at(i);

			float wi_box = objectemp.asValueMap().at("width").asFloat();
			float he_box = objectemp.asValueMap().at("height").asFloat();
			float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
			float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

			Portal* newPortal = new Portal(points[0] + Vec2(x_box, y_box), points[1] + Vec2(x_box, y_box), 1, i, parent);
			if (i % 2 == 0) {
				tempPortal = newPortal;
			}
			else {
				tempPortal->setOuputPortal(newPortal);
				newPortal->setOuputPortal(tempPortal);
			}

			delete[] points;

		}
	}
}

void initMovingGround(TMXTiledMap* map, Layer* parent) {
	auto objectGroup = map->getObjectGroup("MovingGrounds");
	if (objectGroup == nullptr) return;

	auto objects = objectGroup->getObjects();
	for (int i = 0; i < objects.size(); i++)
	{

		auto dic = objects.at(i).asValueMap();
		float objectX = dic.at("x").asFloat();
		float objectY = dic.at("y").asFloat();


		auto pointsVector = dic.at("polylinePoints").asValueVector();
		auto size = pointsVector.size();
		// Get Point 
		if (size > 0)
		{
			Vec2* points = new Vec2[size];
			int j = 0;
			for (auto pointValue : pointsVector)
			{
				auto dicp = pointValue.asValueMap();
				auto x = dicp.at("x").asFloat();
				auto y = -dicp.at("y").asFloat(); // y takes a negative value 
				points[j] = Vec2(x, y);
				j++;
			}

			cocos2d::Value objectemp = objectGroup->getObjects().at(i);

			float wi_box = objectemp.asValueMap().at("width").asFloat();
			float he_box = objectemp.asValueMap().at("height").asFloat();
			float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
			float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

			MovingGround* anoyingfly = new MovingGround(points[0] + Vec2(x_box, y_box), 1.0f, i, parent);
			anoyingfly->setMoveToPos(points[1] + Vec2(x_box, y_box));
			anoyingfly->setFaceTo(points[2] + Vec2(x_box, y_box));

			delete[] points;

		}
	}
}

void initTiltedGround(TMXTiledMap* map, Layer* parent) {
	auto objectGroup = map->getObjectGroup("TiltedGround");
	if (objectGroup == nullptr) return;

	auto objects = objectGroup->getObjects();
	for (int i = 0; i < objects.size(); i++)
	{

	 auto dic = objects.at(i).asValueMap();
	 float objectX = dic.at("x").asFloat();
	 float objectY = dic.at("y").asFloat();


	 auto drawNode = DrawNode::create();
	 auto pointsVector = dic.at("polylinePoints").asValueVector();
	 auto size = pointsVector.size();
	 // Get Point 
	 if (size > 0)
	 {
	  Vec2* points = new Vec2[size];
	  int i = 0;
	  for (auto pointValue : pointsVector)
	  {
	   auto dicp = pointValue.asValueMap();
	   auto x = dicp.at("x").asFloat();
	   auto y = -dicp.at("y").asFloat(); // y takes a negative value 
	   points[i] = Vec2(x, y);
	   i++;
	  }
	  // Draw the polyline 
	  //  drawNode->drawPoly(points, size, false, Color4F::RED);
	  auto sprite = Sprite::create();
	  sprite->setTag(WALL);
	  sprite->setName("TiltedGround" + to_string(i));
	  auto box = PhysicsBody::createPolygon(points, size, PhysicsMaterial(0.0f, 1.0f, 0.0f));
	  box->setDynamic(false);
	  box->setGroup(WALL);
	  box->setCategoryBitmask(WALL);
	  box->setCollisionBitmask(BULLET + ENEMIES_PARTS + ROCK + FAKE_BULLET);
	  box->setContactTestBitmask(BULLET + FAKE_BULLET);
	  sprite->setPhysicsBody(box);
	  sprite->setPosition(objectX, objectY);
	  parent->addChild(sprite);
	  delete[] points;
	  //drawNode->setPosition(objectX, objectY);
	  //this->addChild(drawNode, 10);
	 }
	}
}

void initAnoyingFlies(TMXTiledMap* map, Enemies* enemiesHandler, Layer* parent) {
	auto objectGroup = map->getObjectGroup("AnoyingFlies");
	if (objectGroup == nullptr) return;

	auto objects = objectGroup->getObjects();
	for (int i = 0; i < objects.size(); i++)
	{

		auto dic = objects.at(i).asValueMap();
		float objectX = dic.at("x").asFloat();
		float objectY = dic.at("y").asFloat();


		auto pointsVector = dic.at("polylinePoints").asValueVector();
		auto size = pointsVector.size();
		// Get Point 
		if (size > 0)
		{
			Vec2* points = new Vec2[size];
			int j = 0;
			for (auto pointValue : pointsVector)
			{
				auto dicp = pointValue.asValueMap();
				auto x = dicp.at("x").asFloat();
				auto y = -dicp.at("y").asFloat(); // y takes a negative value 
				points[j] = Vec2(x, y);
				j++;
			}

			cocos2d::Value objectemp = objectGroup->getObjects().at(i);

			float wi_box = objectemp.asValueMap().at("width").asFloat();
			float he_box = objectemp.asValueMap().at("height").asFloat();
			float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
			float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

			AnoyingFly* anoyingfly = new AnoyingFly(points[0] + Vec2(x_box, y_box), 1.0f, i, enemiesHandler, parent);
			anoyingfly->setMoveToPos(points[1] + Vec2(x_box, y_box));

			delete[] points;

		}
	}
}