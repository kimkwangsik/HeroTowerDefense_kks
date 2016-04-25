#ifndef __GameStageScene_H__
#define __GameStageScene_H__

#include "cocos2d.h"

class GameStageScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	GameStageScene(int stagelevel);
};

#endif // __HELLOWORLD_SCENE_H__
