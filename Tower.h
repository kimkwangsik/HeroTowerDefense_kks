#ifndef __Tower_H__
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

	int towerUpgradeLevel;
	char name[10];

	virtual void onEnter();
	virtual void onExit();

	void setTowerSetting();

	bool towerSetup;
	bool towerMenuVisible;
	bool towerUpgradeVisible;
	void towerTick(float a);
	cocos2d::Vector<Monster*> *_pMonster;

private:
	cocos2d::EventListener* _listenter;
	int _fixedPriority;
	bool _useNodePriority;
	int _towerType;
	bool _setupBegan;
	float _attackDelay;
	float _attackPower;
};

#endif //defined(__SpriteExtendEx__Monster__)//