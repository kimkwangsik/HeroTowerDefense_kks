#ifndef __Options_H__
#define __Options_H__

#include "cocos2d.h"

class Options : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Options);
	cocos2d::Size winSize;

	void doClick(Ref* pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
