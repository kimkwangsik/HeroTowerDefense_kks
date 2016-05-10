#include "GameEnd.h"
#include "GameStageScene.h"
#include "MainScene.h"
#include "StageSelectScene.h"

USING_NS_CC;

Scene* GameEnd::createScene()
{
	auto scene = Scene::create();
	auto layer = GameEnd::create();
	scene->addChild(layer);

	return scene;
}

GameEnd::GameEnd(int stagelevel, int heartCount)
{
	if (!Layer::init())
	{
		return;
	}

	nowStagelevel = stagelevel;
	nowHeartCount = heartCount;

	winSize = Director::getInstance()->getVisibleSize();

	auto clearPanel = Sprite::create("Images/GameEnd/End_panel.png");
	clearPanel->setScale(0.0f);
	clearPanel->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(clearPanel);

	clearPanel->runAction(ScaleTo::create(1, 1.f));

	auto pMenuItem1 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(GameEnd::doClick, this));
	pMenuItem1->setPosition(Vec2(clearPanel->getContentSize().width / 4, 20));
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));

	auto mainMenu = LabelTTF::create("메인\n화면", "Arial", 15);
	mainMenu->setPosition(Vec2(pMenuItem1->getContentSize().width / 2, 
		pMenuItem1->getContentSize().height / 2));
	//mainMenu->setAnchorPoint(Vec2(1, 0));
	mainMenu->setColor(Color3B::WHITE);
	pMenuItem1->addChild(mainMenu, 2);

	auto pMenuItem2 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
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
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(GameEnd::doClick, this));
	pMenuItem3->setPosition(Vec2(clearPanel->getContentSize().width / 4 * 3, 20));
	pMenuItem3->setAnchorPoint(Vec2(0.5, 0));

	auto stageMenu = LabelTTF::create("스테이지\n선택", "Arial", 15);
	stageMenu->setPosition(Vec2(pMenuItem3->getContentSize().width / 2,
		pMenuItem3->getContentSize().height / 2));
	//mainMenu->setAnchorPoint(Vec2(1, 0));
	stageMenu->setColor(Color3B::WHITE);
	pMenuItem3->addChild(stageMenu, 2);


	//pMenuItem1->setScale(0.2f);

	pMenuItem1->setTag(21);
	pMenuItem2->setTag(22);
	pMenuItem3->setTag(23);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, NULL);
	pMenu->setPosition(Vec2::ZERO);
	clearPanel->addChild(pMenu);

	if (nowHeartCount > 0)
	{
		int getGold = 10;
		star1 = Sprite::create("Images/GameEnd/Stars.png");
		star1->setPosition(Vec2(clearPanel->getContentSize().width / 4, clearPanel->getContentSize().height - 20));
		star1->setAnchorPoint(Vec2(0.5, 1));
		//star1->setVisible(false);
		clearPanel->addChild(star1, 3);
		
		if (nowHeartCount > 3)
		{
			getGold = getGold + 20;
			star2 = Sprite::create("Images/GameEnd/Stars.png");
			star2->setPosition(Vec2(clearPanel->getContentSize().width / 2, clearPanel->getContentSize().height - 20));
			star2->setAnchorPoint(Vec2(0.5, 1));
			//star2->setVisible(false);
			clearPanel->addChild(star2, 3);
		}

		if (nowHeartCount == 5)
		{
			getGold = getGold + 20;
			star3 = Sprite::create("Images/GameEnd/Stars.png");
			star3->setPosition(Vec2(clearPanel->getContentSize().width / 4 * 3, clearPanel->getContentSize().height - 20));
			star3->setAnchorPoint(Vec2(0.5, 1));
			//star3->setVisible(false);
			clearPanel->addChild(star3, 3);
			
		}

		int i = UserDefault::getInstance()->getIntegerForKey("have_gold");
		i = i + getGold;
		UserDefault::getInstance()->setIntegerForKey("have_gold", i);
	}
	else if(nowHeartCount == 0)
	{
		auto stageFall = LabelTTF::create("실패", "Arial", 30);
		stageFall->setColor(Color3B::WHITE);
		stageFall->setPosition(Vec2(clearPanel->getContentSize().width / 2, clearPanel->getContentSize().height - 20));
		stageFall->setAnchorPoint(Vec2(0.5, 1));
	
		clearPanel->addChild(stageFall, 3);
	}
	return;
}
void GameEnd::onEnter() {
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameEnd::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_listener = listener;
}
void GameEnd::onExit() {
	_eventDispatcher->removeEventListener(_listener);
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
	if (i == 21)
	{
		//메인화면
		//Director::getInstance()->popScene();
		//Director::getInstance()->popScene();

		auto pScene = MainScene::createScene();
		Director::getInstance()->replaceScene(pScene);
	}
	else if (i == 22)
	{
		//재도전
		auto pScene = GameStageScene::createScene();
		auto stagenum = new GameStageScene(nowStagelevel);
		stagenum->autorelease();
		pScene->addChild(stagenum);
		Director::getInstance()->replaceScene(pScene);
	}
	else if (i == 23)
	{
		//스테이지 선택화면
		//Director::getInstance()->popScene();
		auto pScene = StageSelectScene::createScene();
		Director::getInstance()->replaceScene(pScene);
	}
	//this->removeFromParentAndCleanup(true);
}