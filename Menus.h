#ifndef __Menus_H__
#define __Menus_H__

#include "cocos2d.h"

class Menus : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	void setpGold(int* _pnowStageGold);
	int *nowStageGold;
	//virtual bool init();

	Menus(std::string SceneName);

	std::string nowSceneName;

	//CREATE_FUNC(Menus);
	cocos2d::Size winSize;
	cocos2d::LabelTTF* goldLabel;
	cocos2d::LabelTTF* stageGold;

	bool pauseNow;
	char sceneText[20] = {0};
	void doClick(Ref* pSender);
	void nowGold(float dt);
	//void abc();
};

#endif // __HELLOWORLD_SCENE_H__
