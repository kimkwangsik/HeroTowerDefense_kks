#ifndef __IntroScene_H__
#define __IntroScene_H__

#include "cocos2d.h"

class IntroScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(IntroScene);
};

#endif // __HELLOWORLD_SCENE_H__
