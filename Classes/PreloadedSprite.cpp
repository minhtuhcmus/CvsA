#include "PreloadedSprite.h"

void PreloadedSprite::preloadPlayer() {
	std::string path2 = FileUtils::getInstance()->getStringFromFile("player/Cowboy.xml");

	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError eResult = doc->Parse(path2.c_str(), path2.size());

	tinyxml2::XMLElement* root = doc->RootElement();
	tinyxml2::XMLElement* child = root->FirstChildElement();

	child = root->FirstChildElement();
	playerBody_Sprite = loadAnim(child, "Body").at(0);
	child = root->FirstChildElement();
	playerHead_Sprite = loadAnim(child, "Head").at(0);
	child = root->FirstChildElement();
	playerGun_Sprite = loadAnim(child, "Gun").at(0);
}

void PreloadedSprite::preloadEnemies() {
	std::string path2 = FileUtils::getInstance()->getStringFromFile("enemies/Alien.xml");

	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError eResult = doc->Parse(path2.c_str(), path2.size());

	tinyxml2::XMLElement* root = doc->RootElement();
	tinyxml2::XMLElement* child = root->FirstChildElement();

	child = root->FirstChildElement();
	alien_Sprite = loadAnim(child, "Alien_Idle");

	path2 = FileUtils::getInstance()->getStringFromFile("enemies/Alien_Body_Parts.xml");

	doc = new tinyxml2::XMLDocument();
	eResult = doc->Parse(path2.c_str(), path2.size());

	root = doc->RootElement();

	child = root->FirstChildElement();
	alienParts_Sprite = loadAnim(child, "Alien_Body_Parts");
}
void PreloadedSprite::preloadFX() {
	std::string path2 = FileUtils::getInstance()->getStringFromFile("player/Cowboy.xml");

	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
	tinyxml2::XMLError eResult = doc->Parse(path2.c_str(), path2.size());

	tinyxml2::XMLElement* root = doc->RootElement();
	tinyxml2::XMLElement* child = root->FirstChildElement();

	child = root->FirstChildElement();
	playerBody_Sprite = loadAnim(child, "Body").at(0);
	child = root->FirstChildElement();
	playerHead_Sprite = loadAnim(child, "Head").at(0);
	child = root->FirstChildElement();
	playerGun_Sprite = loadAnim(child, "Gun").at(0);
}