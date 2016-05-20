#include "Menus.h"
#include "Options.h"
#include "GameStageScene.h"
#include "StageSelectScene.h"
#include "MainScene.h"

USING_NS_CC;

Scene* Menus::createScene()
{
	auto scene = Scene::create();
	auto layer = Menus::create();
	scene->addChild(layer);

	return scene;
}

void Menus::setpGold(int *_pnowStageGold)
{
	nowStageGold = _pnowStageGold;
}

Menus::Menus(std::string SceneName)
{
	nowSceneName = SceneName;
	pauseNow = false;

	winSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menubar = Sprite::create("Images/MenuButton/menubar.png");// 1024 x 66 사이즈 그림
	menubar->setPosition(Vec2(winSize.width / 2, winSize.height + origin.y));
	menubar->setAnchorPoint(Vec2(0.5, 1));
	menubar->setTag(610);
	addChild(menubar);

	auto nowSceneNameLabel = LabelTTF::create(SceneName, "Arial", 20);
	nowSceneNameLabel->setPosition(Vec2(winSize.width / 2, 0));
	nowSceneNameLabel->setColor(Color3B::BLACK);
	nowSceneNameLabel->setAnchorPoint(Vec2(0.5, 1));
	menubar->addChild(nowSceneNameLabel);

	if (nowSceneName == "GameStageScene" || nowSceneName == "InfinityStage")
	{
		char nowgold[10];
		sprintf(nowgold, "%d", 0);

		auto coin = Sprite::create("Images/Treasure/coin.png");
		coin->setPosition(Vec2(winSize.width / 1.5, menubar->getContentSize().height / 2));
		coin->setAnchorPoint(Vec2(1, 0.5));
		menubar->addChild(coin);

		stageGold = LabelTTF::create(nowgold, "Arial", 20);
		stageGold->setPosition(Vec2(coin->getContentSize().width, coin->getContentSize().height / 2));
		stageGold->setColor(Color3B::BLACK);
		stageGold->setAnchorPoint(Vec2(0, 0.5));
		coin->addChild(stageGold);
	}

	auto pMenuItem1 = MenuItemImage::create(
		"Images/Button/b_Parameters.png",
		"Images/Button/b_Parameters.png",
		CC_CALLBACK_1(Menus::doClick, this));
	pMenuItem1->setPosition(Vec2(winSize.width, 0));
	pMenuItem1->setAnchorPoint(Vec2(1, 0));

	MenuItemImage* pMenuItem2;

	if (nowSceneName == "GameStageScene" || nowSceneName == "MainScene" || nowSceneName == "InfinityStage")
	{
		pMenuItem2 = MenuItemImage::create(
			"Images/Button/b_No.png",
			"Images/Button/b_No.png",
			CC_CALLBACK_1(Menus::doClick, this));
	}
	else
	{
		pMenuItem2 = MenuItemImage::create(
			"Images/Button/b_back.png",
			"Images/Button/b_back.png",
			CC_CALLBACK_1(Menus::doClick, this));
	}


	pMenuItem2->setPosition(Vec2(0, 0));
	pMenuItem2->setAnchorPoint(Vec2(0, 0));


	auto pMenuItem3 = MenuItemImage::create(
		"Images/Button/b_Pause1.png",
		"Images/Button/b_Pause1.png",
		CC_CALLBACK_1(Menus::doClick, this));
	pMenuItem3->setPosition(Vec2(winSize.width - pMenuItem1->getContentSize().width, 0));
	pMenuItem3->setAnchorPoint(Vec2(1, 0));
	if (0)
	{
		pMenuItem3->setVisible(true);
	}
	else
	{
		pMenuItem3->setVisible(false);
	}

	pMenuItem1->setTag(611);
	pMenuItem2->setTag(612);
	pMenuItem3->setTag(613);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, NULL);
	pMenu->setPosition(Vec2(0, 0));
	menubar->addChild(pMenu);

	auto gold_crystal = Sprite::create("Images/Treasure/gold_crystal.png");
	gold_crystal->setPosition(Vec2(winSize.width / 2.5, menubar->getContentSize().height/2));
	gold_crystal->setAnchorPoint(Vec2(1, 0.5));
	menubar->addChild(gold_crystal);


	int i = UserDefault::getInstance()->getIntegerForKey("have_gold");
	log("have_gold is %d", i);

	char gold[10];
	sprintf(gold, "%d", i);

	goldLabel = LabelTTF::create(gold, "fonts/arial.ttf", 20);

	goldLabel->setPosition(Vec2(gold_crystal->getContentSize().width, gold_crystal->getContentSize().height/2));
	goldLabel->setColor(Color3B::BLACK);
	goldLabel->setAnchorPoint(Vec2(0, 0.5));
	gold_crystal->addChild(goldLabel);


	this->schedule(schedule_selector(Menus::nowGold));

	return;
}

void Menus::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 611)
	{
		log("메뉴가 선택되었습니다.");
		auto pScene = Options::createScene();
		this->addChild(pScene, 3000);
	}
	else if (i == 612)
	{
		if (nowSceneName == "GameStageScene")
		{
			auto pScene = StageSelectScene::createScene();
			Director::getInstance()->replaceScene(pScene);
		}
		else if (nowSceneName == "InfinityStage")
		{
			auto pScene = MainScene::createScene();
			Director::getInstance()->replaceScene(pScene);
		}
		else if (nowSceneName == "StageSelectScene")
		{
			auto pScene = MainScene::createScene();
			Director::getInstance()->replaceScene(pScene);
		}
		else if (nowSceneName == "HeroStatu")
		{
			auto pScene = MainScene::createScene();
			Director::getInstance()->replaceScene(pScene);
		}
	}
	else if (i == 613 && 0)
	{
		if (pauseNow)
		{
			Director::getInstance()->resume();
			pauseNow = false;
		}
		else
		{
			Director::getInstance()->pause();
			pauseNow = true;
		}
	}
	
}

void Menus::nowGold(float dt)
{
	int i = UserDefault::getInstance()->getIntegerForKey("have_gold");
	char gold[10];
	sprintf(gold, "%d", i);
	goldLabel->setString(gold);

	if (nowSceneName == "GameStageScene" || nowSceneName == "InfinityStage")
	{
		char nowgold[10];
		sprintf(nowgold, "%d", *nowStageGold);

		stageGold->setString(nowgold);
	}
}
