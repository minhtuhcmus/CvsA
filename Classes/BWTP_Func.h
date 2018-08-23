#include "cocos2d.h"
#include <string>
#include "ui/CocosGUI.h"
#include <math.h>
#include "GameDefine.h"
#include "../external/tinyxml2/tinyxml2.h"

USING_NS_CC;
using namespace std;

Vector<SpriteFrame*> loadAnim(tinyxml2::XMLElement* child, std::string key);
Vector<SpriteFrame*> getAnimation(const char* format);
SpriteFrame* getSprite(const char* format);

//Player* initPlayer(TMXTiledMap* map, int ammo, float scale, Layer* parent);
//PlayerController* initPlayerController(Player* player, Node* parent);

void addTileMap(TMXTiledMap* map, string objectName, PhysicsMaterial material, int cartegory, int collision, int contactTest, bool isDynamic, Node* parent);