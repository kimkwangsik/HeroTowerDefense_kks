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
	int *nowStageGold;
	void setpGold(int* _pnowStageGold);
	int	cost;

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
	cocos2d::EventListener* _listenter;
	float _attackPower;
	cocos2d::Label* levelView;

private:
	int _fixedPriority;
	bool _useNodePriority;
	int _towerType;
	bool _setupBegan;
	float _attackDelay;
};

#endif //defined(__SpriteExtendEx__Monster__)//