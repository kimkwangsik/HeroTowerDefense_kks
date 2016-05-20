#ifndef __InfinityInfo_H__
#define __InfinityInfo_H__

#include "cocos2d.h"

class InfinityInfo : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(InfinityInfo);
	cocos2d::Size winSize;

	void doClick(Ref* pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
