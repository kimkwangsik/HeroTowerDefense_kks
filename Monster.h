#ifndef __Monster_H__
#define __Monster_H__

#include "cocos2d.h"

class Monster : public cocos2d::Sprite
{
public:
	Monster(std::string monsterName);
	std::string myMonsterName;
	int hp;

	void setPriority(int fixwdPriority);
	void setPriorityWithThis(bool usdNodePriority);

	virtual void onEnter();
	virtual void onExit();
	void remove();
	void monsterTick(float a);

	cocos2d::Sprite* hpBar;
	cocos2d::Sprite* hpGauge;
	int gauge;
	cocos2d::ProgressTimer *gaugeBar;

private:
	cocos2d::EventListener* _listenter;
	int _fixedPriority;
	bool _useNodePriority;
};

#endif //defined(__SpriteExtendEx__Monster__)//