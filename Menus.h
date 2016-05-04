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


	bool pauseNow;
	char sceneText[20] = {0};
	void doClick(Ref* pSender);
	void doMsgReceived(Ref* obj);
	//void abc();
};

#endif // __HELLOWORLD_SCENE_H__
