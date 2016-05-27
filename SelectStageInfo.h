#ifndef __SelectStageInfo_H__
#define __SelectStageInfo_H__

#include "cocos2d.h"

class SelectStageInfo : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	SelectStageInfo(int selectStage);
	int _selectStage;

	bool soundon;

	cocos2d::Size winSize;
	bool menuItem3;
	bool menuItem4;
	cocos2d::MenuItemImage* pMenuItem3;
	cocos2d::MenuItemImage* pMenuItem4;

	void doClick(Ref* pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __HELLOWORLD_SCENE_H__
