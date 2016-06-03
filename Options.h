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

	cocos2d::MenuItemFont* pMenuItem2;
	cocos2d::MenuItemFont* pMenuItem3;
	cocos2d::MenuItemFont* pMenuItem4;
	

	void doClick(Ref* pSender);
	void doShow(Ref * pSender);
	void doHide(Ref * pSender);
	void doSendScore(Ref * pSender);
	void doShowLeaderBoard(Ref * pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
