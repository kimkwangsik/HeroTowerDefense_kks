#include "SelectStageInfo.h"
#include "GameStageScene.h"

USING_NS_CC;

Scene* SelectStageInfo::createScene()
{
	auto scene = Scene::create();
	auto layer = SelectStageInfo::create();
	scene->addChild(layer);

	return scene;
}

SelectStageInfo::SelectStageInfo(int selectStage)
{
	if (!Layer::init())
	{
		return;
	}
	menuItem3 = false;
	menuItem4 = false;
	_selectStage = selectStage;
	winSize = Director::getInstance()->getVisibleSize();

	auto StageInfo = Sprite::create("Images/SelectStageInfoBackGround.png");
	StageInfo->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(StageInfo);

	char level[20];
	sprintf(level, "Level %d", _selectStage);

	if (_selectStage == 0)
	{
		sprintf(level, "Infinity mode");
	}

	auto levelLabel = LabelTTF::create(level, "Arial", 20);
	levelLabel->setPosition(Vec2(10, StageInfo->getContentSize().height-10));
	levelLabel->setAnchorPoint(Vec2(0, 1));
	//levelLabel->setColor(Color3B::BLACK);
	StageInfo->addChild(levelLabel, 1);
	
	//char levelsprite[20];
	//sprintf(levelsprite, "Images/stage/Level_%d.PNG", _selectStage);
	//auto stageSprite = Sprite::create(levelsprite);
	
	auto stageSprite = Sprite::create("Images/stage/Level_1.PNG");
	stageSprite->setPosition(Vec2(0, 0));
	stageSprite->setAnchorPoint(Vec2(0, 1));

	stageSprite->setScaleX(0.9f);
	stageSprite->setScaleY(0.8f);
	//stageSprite->setColor(Color3B::WHITE);
	levelLabel->addChild(stageSprite);

	auto pMenuItem1 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(SelectStageInfo::doClick, this));
	pMenuItem1->setPosition(Vec2(StageInfo->getContentSize().width / 4, 10));
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));

	auto cancelMenu = LabelTTF::create(" 취소 ", "Arial", 15);
	cancelMenu->setPosition(Vec2(pMenuItem1->getContentSize().width / 2,
		pMenuItem1->getContentSize().height / 2));
	//mainMenu->setAnchorPoint(Vec2(1, 0));
	cancelMenu->setColor(Color3B::WHITE);
	pMenuItem1->addChild(cancelMenu, 2);

	auto pMenuItem2 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(SelectStageInfo::doClick, this));
	pMenuItem2->setPosition(Vec2(StageInfo->getContentSize().width / 4 * 3, 10));
	pMenuItem2->setAnchorPoint(Vec2(0.5, 0));

	auto startMenu = LabelTTF::create(" 시작 ", "Arial", 15);
	startMenu->setPosition(Vec2(pMenuItem2->getContentSize().width / 2,
		pMenuItem2->getContentSize().height / 2));
	startMenu->setColor(Color3B::WHITE);
	pMenuItem2->addChild(startMenu, 2);

	pMenuItem3 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(SelectStageInfo::doClick, this));
	pMenuItem3->setPosition(Vec2(StageInfo->getContentSize().width - 10, pMenuItem2->getPositionY() + pMenuItem2->getContentSize().height + 10));
	pMenuItem3->setAnchorPoint(Vec2(1, 0));

	auto goldAdd = LabelTTF::create("골드\n추가", "Arial", 15);
	goldAdd->setPosition(Vec2(pMenuItem3->getContentSize().width / 2,
		pMenuItem3->getContentSize().height / 2));
	goldAdd->setColor(Color3B::WHITE);
	pMenuItem3->addChild(goldAdd, 2);

	pMenuItem4 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(SelectStageInfo::doClick, this));
	pMenuItem4->setPosition(Vec2(StageInfo->getContentSize().width -10, pMenuItem3->getPositionY() + pMenuItem3->getContentSize().height + 10));
	pMenuItem4->setAnchorPoint(Vec2(1, 0));

	auto fullMasic = LabelTTF::create("마법\n충전", "Arial", 15);
	fullMasic->setPosition(Vec2(pMenuItem4->getContentSize().width / 2,
		pMenuItem4->getContentSize().height / 2));
	fullMasic->setColor(Color3B::WHITE);
	pMenuItem4->addChild(fullMasic, 2);

	pMenuItem1->setScaleX(2.5f);
	pMenuItem2->setScaleX(2.5f);

	pMenuItem1->setTag(621);
	pMenuItem2->setTag(622);
	pMenuItem3->setTag(623);
	pMenuItem4->setTag(624);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, pMenuItem4, NULL);
	pMenu->setPosition(Vec2::ZERO);
	StageInfo->addChild(pMenu);

	return;
}
void SelectStageInfo::onEnter() {
	Layer::onEnter();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(SelectStageInfo::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void SelectStageInfo::onExit() {
	Layer::onExit();
}
bool SelectStageInfo::onTouchBegan(Touch* touch, Event* event) {

	return true;
}
void SelectStageInfo::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	int gold = UserDefault::getInstance()->getIntegerForKey("have_gold");

	if (i == 621)
	{
		if (menuItem3)
		{
			gold += 10;
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);
		}
		if (menuItem4)
		{
			gold += 10;
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);
		}

		this->removeFromParentAndCleanup(true);
		
	}
	else if (i == 622)
	{
		auto pScene = GameStageScene::createScene();
		auto stagenum = new GameStageScene(_selectStage);
		stagenum->getOption(menuItem3, menuItem4);
		stagenum->autorelease();
		pScene->addChild(stagenum);
		Director::getInstance()->replaceScene(pScene);
	}
	else if (i == 623)
	{
		
		if (menuItem3)
		{
			
			gold += 10;
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);

			pMenuItem3->setNormalImage(Sprite::create("Images/GameEnd/Game_btn_on.png"));
			pMenuItem3->setSelectedImage(Sprite::create("Images/GameEnd/Game_btn_down.png"));
			menuItem3 = false;
		}
		else if(gold >= 10)
		{
			gold -= 10;
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);

			pMenuItem3->setNormalImage(Sprite::create("Images/GameEnd/Game_btn_on1.png"));
			pMenuItem3->setSelectedImage(Sprite::create("Images/GameEnd/Game_btn_down1.png"));
			menuItem3 = true;
		}
	}

	else if (i == 624)
	{
		if (menuItem4)
		{
			gold += 10;
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);

			pMenuItem4->setNormalImage(Sprite::create("Images/GameEnd/Game_btn_on.png"));
			pMenuItem4->setSelectedImage(Sprite::create("Images/GameEnd/Game_btn_down.png"));
			menuItem4 = false;
		}
		else if (gold >= 10)
		{
			gold -= 10;
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);

			pMenuItem4->setNormalImage(Sprite::create("Images/GameEnd/Game_btn_on1.png"));
			pMenuItem4->setSelectedImage(Sprite::create("Images/GameEnd/Game_btn_down1.png"));
			menuItem4 = true;
		}
	}
}