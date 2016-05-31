#ifndef __GameEnd_H__
#define __GameEnd_H__

#include "cocos2d.h"

class GameEnd : public cocos2d::Layer
{
public:
	void addGoldFromJava();
	static cocos2d::Scene* createScene();

	GameEnd(int stagelevel, int heartCount, int phase);

	int nowStagelevel;
	int nowHeartCount;
	int Endphase;

	int getGoldAD;

	cocos2d::Sprite* star1;
	cocos2d::Sprite* star2;
	cocos2d::Sprite* star3;

	cocos2d::MenuItemImage* pMenuItem4;

	cocos2d::LabelTTF* goldLabel;
	bool goldbonus;

	cocos2d::EventListenerTouchOneByOne* _listener;

	cocos2d::Size winSize;

	void doClick(Ref* pSender);
	void doShow(Ref * pSender);
	void doHide(Ref * pSender);
	void doFullShow(Ref * pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
