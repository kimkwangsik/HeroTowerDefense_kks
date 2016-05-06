#ifndef __GameEnd_H__
#define __GameEnd_H__

#include "cocos2d.h"

class GameEnd : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	GameEnd(int stagelevel, int heartCount);

	int nowStagelevel;
	int nowHeartCount;

	cocos2d::Sprite* star1;
	cocos2d::Sprite* star2;
	cocos2d::Sprite* star3;

	cocos2d::EventListenerTouchOneByOne* _listener;

	cocos2d::Size winSize;

	void doClick(Ref* pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
