#ifndef __MainScene_H__
#define __MainScene_H__

#include "cocos2d.h"

class MainScene : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

	virtual bool init();
    
    CREATE_FUNC(MainScene);

	void doClick(Ref* pSender);
	void doNoClick(Ref* pSender);

	//virtual void onEnter();
};

#endif // __HELLOWORLD_SCENE_H__
