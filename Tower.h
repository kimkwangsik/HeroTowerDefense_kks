#ifndef __Tower_H__
#define __Tower_H__

#include "cocos2d.h"

class Tower : public cocos2d::Sprite
{
public:
	Tower(int typenum);

	void setPriority(int fixwdPriority);
	void setPriorityWithThis(bool usdNodePriority);

	virtual void onEnter();
	virtual void onExit();

	void towerSetup(int typenum);

private:
	cocos2d::EventListener* _listenter;
	int _fixedPriority;
	bool _useNodePriority;
	int _towerType;
};

#endif //defined(__SpriteExtendEx__Monster__)//