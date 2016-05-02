#ifndef __Tower_H__
#define __Tower_H__

#include "cocos2d.h"
#include "Monster.h"


class Tower : public cocos2d::Sprite
{
public:
	Tower(int typenum);

	void setPriorityWithThis(bool usdNodePriority);
	void setpMonster(cocos2d::Vector<Monster*> *_repMonster);
	cocos2d::Sprite* b_Yes;
	cocos2d::Sprite* b_No;

	virtual void onEnter();
	virtual void onExit();

	void setAttackDelay();
	void setAttackPower();

	bool towerSetup;
	bool towerMenuVisible;
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