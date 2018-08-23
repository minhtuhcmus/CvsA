#include "BWTP_Func.h"

class PlayerController;
class RewardPoster;
class FailPoster;
class Player;

#pragma once
class MissionHandler : public Node
{
public:
	MissionHandler(PlayerController* playerController, Player* player, Node* parent);
	~MissionHandler();

	static enum SUBMISSIONS { 
		RUSH_TIME, 
		BULLET_COUNT, 
		BULLET_BOUNCING,
		ONE_SHOT,
		NO_BOUNCING,
		DONT_ROASTED,
		DONT_KNOCKED,
		DONT_HIT_TNT,
		HIT_TNT,
		KILL_ALL,
		EATEN
	};

	int getStars() { return _numOfStars; }
	void addScore(int score);
	void checkout();
	void winScene(int timeRemain, int ammo, int bounces);
	void fail();
	void failScene();
	void setLevelName(string levelname);
	void setIDbgMusic(int id);
	bool isWon();
	bool isFail();
	void setRushtime(int timeToRush);
	void setBulletToCount(int bulletToCount);
	void setBouncesToCount(int bouncesToCount);
	void setOneShot(int bulletToCount);
	void setNoBoucing(int bouncesToCount);
	void setDontRoasted(int dontRoasted);
	void setDontKnocked(int dontKnocked);
	void setDontHitTNT(int dontHitTNT);
	void setHitTNT(int hitTNT);
	void setKillAllEnemies(int killAll);
	void setEaten(int eaten);

private:
	string _levelname;
	PlayerController* _playerController;
	Node* _runningScene;
	Player* _player;
	Label* _messageOut[3] = {nullptr, nullptr , nullptr};


	int _idBgMusic;
	int _missionScore = 0;
	int _fontSize = 30;
	int _numOfStars;
	bool _submissionsEnable[11];
	int _timeToRush;
	int _bulletToCount;
	int _bouncesToCount;
	int _oneShot;
	int _noBouncing;
	int _dontRoasted;
	int _dontKnocked;
	int _dontHitTNT;
	int _hitTNT;

	bool _isWon = false;
	bool _isFailed = false;

	void update(float) override;
	void checkSubmissions(int timeRemain, int ammo, int bounces);
};

