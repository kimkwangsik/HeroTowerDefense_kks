#include "GameEnd.h"
#include "GameStageScene.h"
#include "MainScene.h"
#include "StageSelectScene.h"

USING_NS_CC;
static GameEnd *g_pHello = NULL;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

void callJavaMethodGameEnd(std::string func)
{
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t
		, "org/cocos2dx/cpp/AppActivity"
		, func.c_str()
		, "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

#ifdef __cplusplus
extern "C" {
#endif // _cplusplus
	void Java_org_cocos2dx_cpp_AppActivity_AddGetGoldInNative(JNIEnv *env, jobject obj)
	{
		g_pHello->addGoldFromJava();
	}
#ifdef __cplusplus
}
#endif // DEBUG

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)



Scene* GameEnd::createScene()
{
	auto scene = Scene::create();
	auto layer = GameEnd::create();
	scene->addChild(layer);

	return scene;
}

GameEnd::GameEnd(int stagelevel, int heartCount, int phase)
{
	if (!Layer::init())
	{
		return;
	}
	goldbonus = true;
	nowStagelevel = stagelevel;
	nowHeartCount = heartCount;
	Endphase = phase;

	winSize = Director::getInstance()->getVisibleSize();

	auto clearPanel = Sprite::create("Images/GameEnd/End_panel.png");
	clearPanel->setScale(0.0f);
	clearPanel->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(clearPanel);

	clearPanel->runAction(ScaleTo::create(1, 1.f));

	auto pMenuItem1 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on130.png",
		"Images/GameEnd/Game_btn_down130.png",
		CC_CALLBACK_1(GameEnd::doClick, this));
	pMenuItem1->setPosition(Vec2(clearPanel->getContentSize().width / 4 - 10, 20));
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));

	auto mainMenu = LabelTTF::create("메인\n화면", "Arial", 15);
	mainMenu->setPosition(Vec2(pMenuItem1->getContentSize().width / 2, 
		pMenuItem1->getContentSize().height / 2));
	//mainMenu->setAnchorPoint(Vec2(1, 0));
	mainMenu->setColor(Color3B::WHITE);
	pMenuItem1->addChild(mainMenu, 2);

	auto pMenuItem2 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on130.png",
		"Images/GameEnd/Game_btn_down130.png",
		CC_CALLBACK_1(GameEnd::doClick, this));
	pMenuItem2->setPosition(Vec2(clearPanel->getContentSize().width / 2, 20));
	pMenuItem2->setAnchorPoint(Vec2(0.5, 0));

	auto restartMenu = LabelTTF::create("재도전", "Arial", 15);
	restartMenu->setPosition(Vec2(pMenuItem2->getContentSize().width / 2,
		pMenuItem2->getContentSize().height / 2));
	//mainMenu->setAnchorPoint(Vec2(1, 0));
	restartMenu->setColor(Color3B::WHITE);
	pMenuItem2->addChild(restartMenu, 2);

	auto pMenuItem3 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on130.png",
		"Images/GameEnd/Game_btn_down130.png",
		CC_CALLBACK_1(GameEnd::doClick, this));
	pMenuItem3->setPosition(Vec2(clearPanel->getContentSize().width / 4 * 3 + 10, 20));
	pMenuItem3->setAnchorPoint(Vec2(0.5, 0));

	auto stageMenu = LabelTTF::create("스테이지\n선택", "Arial", 15);
	stageMenu->setPosition(Vec2(pMenuItem3->getContentSize().width / 2,
		pMenuItem3->getContentSize().height / 2));
	//mainMenu->setAnchorPoint(Vec2(1, 0));
	stageMenu->setColor(Color3B::WHITE);
	pMenuItem3->addChild(stageMenu, 2);

	

	//pMenuItem1->setScale(0.2f);

	

	if (nowHeartCount > 0)
	{
		int getGold = 10;
		star2 = Sprite::create("Images/GameEnd/Stars.png");
		star2->setPosition(Vec2(clearPanel->getContentSize().width / 2, clearPanel->getContentSize().height - 10));
		star2->setAnchorPoint(Vec2(0.5, 1));
		clearPanel->addChild(star2, 3);
		
		
		if (nowHeartCount > 2)
		{
			getGold = getGold + 20;
			star1 = Sprite::create("Images/GameEnd/Stars.png");
			star1->setPosition(Vec2(clearPanel->getContentSize().width / 4, clearPanel->getContentSize().height - 10));
			star1->setAnchorPoint(Vec2(0.5, 1));
			clearPanel->addChild(star1, 3);
		}

		if (nowHeartCount == 5)
		{
			getGold = getGold + 20;
			star3 = Sprite::create("Images/GameEnd/Stars.png");
			star3->setPosition(Vec2(clearPanel->getContentSize().width / 4 * 3, clearPanel->getContentSize().height - 10));
			star3->setAnchorPoint(Vec2(0.5, 1));
			clearPanel->addChild(star3, 3);
			
		}

		getGold *= nowStagelevel;

		int i = UserDefault::getInstance()->getIntegerForKey("have_gold");
		getGoldAD = getGold;
		i = i + getGold;
		UserDefault::getInstance()->setIntegerForKey("have_gold", i);

		int maxStage = UserDefault::getInstance()->getIntegerForKey("clear_stage");
		if (maxStage < stagelevel)
		{
			UserDefault::getInstance()->setIntegerForKey("clear_stage", stagelevel);
		}

		auto reward = LabelTTF::create("보상", "Arial", 20);
		reward->setPosition(Vec2(star2->getContentSize().width / 2, 0));
		reward->setAnchorPoint(Vec2(0.5, 1));
		star2->addChild(reward);

		auto gold_crystal = Sprite::create("Images/Treasure/gold_crystal.png");
		gold_crystal->setPosition(Vec2(reward->getContentSize().width / 2, 0));
		gold_crystal->setAnchorPoint(Vec2(1, 1));
		reward->addChild(gold_crystal);

		char gold[10];
		sprintf(gold, "%d", getGold);

		goldLabel = LabelTTF::create(gold, "Arial", 20);
		goldLabel->setPosition(Vec2(reward->getContentSize().width / 2, -gold_crystal->getContentSize().height / 2));
		goldLabel->setAnchorPoint(Vec2(0, 0.5));
		reward->addChild(goldLabel);

		
	}
	else if(nowHeartCount == 0)
	{
		if (nowStagelevel != 0)
		{
			auto stageFall = LabelTTF::create("실패", "Arial", 30);
			stageFall->setColor(Color3B::WHITE);
			stageFall->setPosition(Vec2(clearPanel->getContentSize().width / 2, clearPanel->getContentSize().height - 20));
			stageFall->setAnchorPoint(Vec2(0.5, 1));

			clearPanel->addChild(stageFall, 3);
		}
		else
		{
			int a = (Endphase / 6);
			
			int getGold = a * 50;
			getGoldAD = getGold;


			auto stageFall = LabelTTF::create("무한 모드 종료", "Arial", 30);
			stageFall->setColor(Color3B::WHITE);
			stageFall->setPosition(Vec2(clearPanel->getContentSize().width / 2, clearPanel->getContentSize().height - 20));
			stageFall->setAnchorPoint(Vec2(0.5, 1));

			clearPanel->addChild(stageFall, 3);

			auto reward = LabelTTF::create("보상", "Arial", 20);
			reward->setPosition(Vec2(stageFall->getContentSize().width / 2, 0));
			reward->setAnchorPoint(Vec2(0.5, 1));
			stageFall->addChild(reward);

			auto gold_crystal = Sprite::create("Images/Treasure/gold_crystal.png");
			gold_crystal->setPosition(Vec2(reward->getContentSize().width / 2, 0));
			gold_crystal->setAnchorPoint(Vec2(1, 1));
			reward->addChild(gold_crystal);

			char gold[10];
			sprintf(gold, "%d", getGold);

			goldLabel = LabelTTF::create(gold, "Arial", 20);
			goldLabel->setPosition(Vec2(reward->getContentSize().width / 2, -gold_crystal->getContentSize().height / 2));
			goldLabel->setAnchorPoint(Vec2(0, 0.5));
			reward->addChild(goldLabel);

			int i = UserDefault::getInstance()->getIntegerForKey("have_gold");
			i = i + getGold;
			UserDefault::getInstance()->setIntegerForKey("have_gold", i);
		}
	}

	pMenuItem1->setTag(631);
	pMenuItem2->setTag(632);
	pMenuItem3->setTag(633);

	Menu* pMenu;
	if (nowHeartCount != 0 || nowStagelevel == 0)
	{
		pMenuItem4 = MenuItemImage::create(
			"Images/GameEnd/Game_btn_on130.png",
			"Images/GameEnd/Game_btn_down130.png",
			CC_CALLBACK_1(GameEnd::doClick, this));
		pMenuItem4->setPosition(Vec2(clearPanel->getContentSize().width / 4 * 3 + 10, 70));
		pMenuItem4->setAnchorPoint(Vec2(0.5, 0));

		auto adView = LabelTTF::create("보상2배\n광고", "Arial", 15);
		adView->setPosition(Vec2(pMenuItem4->getContentSize().width / 2,
			pMenuItem4->getContentSize().height / 2));
		//mainMenu->setAnchorPoint(Vec2(1, 0));
		adView->setColor(Color3B::WHITE);
		pMenuItem4->addChild(adView, 2);


		pMenuItem4->setTag(634);
		pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, pMenuItem4, NULL);

	}
	else {

		pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, NULL);
	}
	pMenu->setPosition(Vec2::ZERO);
	clearPanel->addChild(pMenu);


	g_pHello = this;

	return;
}
void GameEnd::onEnter() {
	Layer::onEnter();
	doShow(this);
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameEnd::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_listener = listener;
}
void GameEnd::onExit() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethodGameEnd("Cancel");

#endif
	_eventDispatcher->removeEventListener(_listener);
	doHide(this); 

	//Director::getInstance()->resume();
	Layer::onExit();
}
bool GameEnd::onTouchBegan(Touch* touch, Event* event) {

	return true;
}
void GameEnd::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 631)
	{
		auto pScene = MainScene::createScene();
		Director::getInstance()->replaceScene(pScene);
	}
	else if (i == 632)
	{
		//재도전
		auto pScene = GameStageScene::createScene();
		auto stagenum = new GameStageScene(nowStagelevel);
		stagenum->autorelease();
		pScene->addChild(stagenum);
		Director::getInstance()->replaceScene(pScene);
	}
	else if (i == 633)
	{
		//스테이지 선택화면
		//Director::getInstance()->popScene();
		auto pScene = StageSelectScene::createScene();
		Director::getInstance()->replaceScene(pScene);
	}
	else if (i == 634 && goldbonus)
	{
		doFullShow(this);
		goldbonus = false;
		pMenuItem4->setOpacity(100.0f);
	}
	//this->removeFromParentAndCleanup(true);
}


void GameEnd::addGoldFromJava()
{
	char gold[10];
	sprintf(gold, "%d", getGoldAD * 2);
	goldLabel->setString(gold);
	int i = UserDefault::getInstance()->getIntegerForKey("have_gold");
	i = i + getGoldAD;
	UserDefault::getInstance()->setIntegerForKey("have_gold", i);
}


void GameEnd::doShow(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethodGameEnd("ShowAdPopup");

#endif

}

void GameEnd::doHide(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethodGameEnd("HideAdPopup");

#endif

}



void GameEnd::doFullShow(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethodGameEnd("ShowAdFull");

#endif

}