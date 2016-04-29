#ifndef __Tower_H__
#define __Tower_H__

#include "cocos2d.h"


class Tower : public cocos2d::Sprite
{
public:
	Tower(int typenum);

	void setPriorityWithThis(bool usdNodePriority);
	cocos2d::Sprite* b_Yes;
	cocos2d::Sprite* b_No;

	virtual void onEnter();
	virtual void onExit();


	bool towerSetup;
	bool towerMenuVisible;
private:
	cocos2d::EventListener* _listenter;
	int _fixedPriority;
	bool _useNodePriority;
	int _towerType;
	bool _setupBegan;
};

#endif //defined(__SpriteExtendEx__Monster__)//