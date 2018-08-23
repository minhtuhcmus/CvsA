#include "BWTP_Func.h"
//#include "PlayerController.h"

Vector<SpriteFrame*> loadAnim(tinyxml2::XMLElement* child, std::string key)
{

	Vector<SpriteFrame*> list;

	while (child)
	{
		if (child->Attribute("name") == key)
		{
			tinyxml2::XMLElement* ele = child->FirstChildElement();
			while (ele)
			{
				float x;
				ele->QueryFloatAttribute("x", &x);
				float y;
				ele->QueryFloatAttribute("y", &y);
				float w;
				ele->QueryFloatAttribute("w", &w);
				float h;
				ele->QueryFloatAttribute("h", &h);


				list.pushBack(SpriteFrame::create(child->Attribute("imagePath"), Rect(x, y, w, h)));

				ele = ele->NextSiblingElement();
			}
			break;
		}
		child = child->NextSiblingElement();
	}
	return list;
}

void addTileMap(TMXTiledMap* map, string objectName, PhysicsMaterial material, int cartegory, int collision, int contactTest, bool isDynamic, Node* parent) {
	TMXObjectGroup *objectGroup_land_row = map->getObjectGroup(objectName);

	for (unsigned int i = 0; i < objectGroup_land_row->getObjects().size(); i++)
	{
		Value objectemp = objectGroup_land_row->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat();
		float he_box = objectemp.asValueMap().at("height").asFloat();
		float x_box = objectemp.asValueMap().at("x").asFloat() + wi_box / 2;
		float y_box = objectemp.asValueMap().at("y").asFloat() + he_box / 2;

		auto edgeSp = Sprite::create();
		edgeSp->setTag(cartegory);
		edgeSp->setName("Wall" + to_string(i));

		auto boundBody = PhysicsBody::createBox(Size(wi_box, he_box), material);
		boundBody->setDynamic(isDynamic);
		boundBody->setCategoryBitmask(cartegory);
		boundBody->setCollisionBitmask(collision);
		boundBody->setContactTestBitmask(contactTest);

		edgeSp->setPhysicsBody(boundBody);
		edgeSp->setPosition(Vec2(x_box, y_box));

		parent->addChild(edgeSp); // Add obj into Layer
	}
}

Vector<SpriteFrame*> getAnimation(const char* format)
{
	auto spriteCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames;
	char str[100];
	int i = 1;
	SpriteFrame* frame;
	do {
		sprintf(str, format, i);
		frame = spriteCache->getSpriteFrameByName(str);
		if (frame) animFrames.pushBack(frame);
		i++;
	} while (frame != nullptr);

	return animFrames;
}

SpriteFrame* getSprite(const char* format)
{
	auto spriteCache = SpriteFrameCache::getInstance();
	SpriteFrame* frame;
	frame = spriteCache->getSpriteFrameByName(format);
	return frame;
}