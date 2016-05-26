#ifndef __EndPopup_H__
#define __EndPopup_H__

#include "cocos2d.h"

class EndPopup : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	
	EndPopup(std::string SceneName);
	void doClick(Ref * pSender);
	void doShow(Ref * pSender);
	void doHide(Ref * pSender);
	std::string nowSceneName;

	//CREATE_FUNC(Menus);
	cocos2d::Size winSize;
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	//void abc();
};

#endif // __HELLOWORLD_SCENE_H__
