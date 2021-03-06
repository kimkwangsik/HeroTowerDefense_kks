﻿#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	virtual bool init();
    
    CREATE_FUNC(MainScene);

	void doClick(Ref* pSender);
	void doNoClick(Ref* pSender);


	cocos2d::Size VisibleWinSize;

	//cocos2d::TMXTiledMap* tmap;
	//cocos2d::TMXObjectGroup* objects;

	//virtual void onEnter();
};

#endif // __HELLOWORLD_SCENE_H__
