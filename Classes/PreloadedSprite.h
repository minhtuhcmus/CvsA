#include "BWTP_Func.h"
#include "PointPlusFX.h"
#include "FX.h"
#include "EnemyExplosion.h"
#include "SimpleAudioEngine.h"
#pragma once
class PreloadedSprite
{
public:
	PreloadedSprite() {
		preloadPlayer();
		//preloadEnemies();
		//preloadFX();
	};

	SpriteFrame* playerBody_Sprite;
	SpriteFrame* getPlayerBody() {
		return playerBody_Sprite;
	}
	SpriteFrame* playerHead_Sprite;
	SpriteFrame* getPlayerHead() {
		return playerHead_Sprite;
	}
	SpriteFrame* playerGun_Sprite;
	SpriteFrame* getPlayerGun() {
		return playerGun_Sprite;
	}

	Vector<SpriteFrame*> alienParts_Sprite;
	Vector<SpriteFrame*> alien_Sprite;
	Vector<SpriteFrame*> alienBloodFX_Sprite;
	Vector<SpriteFrame*> explosionFX_Sprite;
	Vector<SpriteFrame*> fireworkFX_Sprite;
	Vector<SpriteFrame*> hitWallFX_Sprite;
	Vector<SpriteFrame*> gunShotFX_Sprite;

	void preloadPlayer();
	void preloadEnemies();
	void preloadFX();
};

