﻿#ifndef __Tower_H__
#define __Tower_H__

#include "cocos2d.h"
#include "Monster.h"


class Tower : public cocos2d::Sprite
{
public:
	Tower(int typenum);


	cocos2d::Size towerContentSize;

	void setPriorityWithThis(bool usdNodePriority);
	void setpMonster(cocos2d::Vector<Monster*> *_repMonster);

	cocos2d::Sprite* b_Yes;
	cocos2d::Sprite* b_No;
	cocos2d::Sprite* b_Upgrade;
	int *nowStageGold;
	int *nowMasicGauge;
	void setpGold(int* _pnowStageGold);
	void setpMasicGauge(int * _pMasicGauge);
	int	cost;

	void setAnimation(cocos2d::Vec2 absDis, cocos2d::Vec2 dis);

	void attackDeley(float dt);
	int _attackedMonster;
	Monster* attackedMonster;

	int towerUpgradeLevel;
	char name[10];
	char animationStr1[50];
	char animationStr2[50];
	char animationStr3[50];

	virtual void onEnter();
	virtual void onExit();

	void setTowerSetting();
	//void resetSpeed(float a, Monster* speedDownMonster);
	void resetSpeed(float a);
	bool towerSetup;
	bool towerMenuVisible;
	bool towerUpgradeVisible;
	void towerTick(float a);
	//void animationRename();
	cocos2d::Vector<Monster*> *_pMonster;
	cocos2d::EventListener* _listenter;
	float _attackPower;
	cocos2d::Label* levelView;

private:
	int _fixedPriority;
	bool _useNodePriority;
	int _towerType;
	bool _setupBegan;
	float _attackDelay;
	float _attackArea;
};

#endif //defined(__SpriteExtendEx__Monster__)//