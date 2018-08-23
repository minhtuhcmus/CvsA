#include "cocos2d.h"

class Player;
class PlayerController;
class Enemies;

USING_NS_CC;
using namespace std;

Sprite* initBackground(string path, Node* parent);
void initWall(TMXTiledMap* map, Layer* parent);
Player* initPlayer(cocos2d::TMXTiledMap* map, int ammo, float scale, Layer* parent);
void initPans(TMXTiledMap* map, Layer* parent);
PlayerController* initPlayerController(Player* player, Node* parent);
Enemies* initEnemies(cocos2d::TMXTiledMap* map, Layer* parent);
void initItems(cocos2d::TMXTiledMap* map, Layer* parent);
void initAliens(cocos2d::TMXTiledMap* map, Enemies* enemiesHandler, Layer* parent);
void initChickenEaters(cocos2d::TMXTiledMap* map, Enemies* enemiesHandler, Layer* parent);
void initAnoyingFlies(TMXTiledMap* map, Enemies* enemiesHandler, Layer* parent);
void initTNT(cocos2d::TMXTiledMap* map, Layer* parent);
void initLava(cocos2d::TMXTiledMap* map, Layer* parent);
void initRock(cocos2d::TMXTiledMap* map, Layer* parent);
void initCanon(TMXTiledMap* map, Layer* parent);
void initMovingGround(TMXTiledMap* map, Layer* parent);
void initChainSaw(TMXTiledMap* map, Layer* parent);
void initPortal(TMXTiledMap* map, Layer* parent);
void initTiltedGround(cocos2d::TMXTiledMap* map, Layer* parent);