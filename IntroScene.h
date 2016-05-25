#ifndef __IntroScene_H__
#define __IntroScene_H__

#include "cocos2d.h"
#include "sqlite3.h"

class IntroScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(IntroScene);

	cocos2d::EventListenerTouchOneByOne* introListener;

	cocos2d::Layer* layer1;

	cocos2d::Sprite* introSprite;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	void createKnight(float a);
};

#endif // __HELLOWORLD_SCENE_H__
