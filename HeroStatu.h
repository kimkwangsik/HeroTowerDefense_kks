#ifndef __HeroStatu_H__
#define __HeroStatu_H__

#include "cocos2d.h"

class HeroStatu : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(HeroStatu);



	cocos2d::Size VisibleWinSize;

	cocos2d::TMXTiledMap* tmap;
	cocos2d::TMXObjectGroup* objects;

	//virtual void onEnter();
};

#endif // __HELLOWORLD_SCENE_H__
