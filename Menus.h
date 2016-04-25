#ifndef __Menus_H__
#define __Menus_H__

#include "cocos2d.h"

class Menus : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Menus);
	cocos2d::Size winSize;

	void doClick(Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
