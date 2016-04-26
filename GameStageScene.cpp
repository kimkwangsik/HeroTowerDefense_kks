#include "GameStageScene.h"
#include "Menus.h"

USING_NS_CC;

Scene* GameStageScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameStageScene::create();
	scene->addChild(layer);

	return scene;
}

GameStageScene::GameStageScene(int stagelevel)
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return;
	}
	winSize = Director::getInstance()->getVisibleSize();

	auto pScene = Menus::createScene();
	this->addChild(pScene,2000);

	char str[20];
	sprintf(str, "TileMaps/Level%ld.tmx", stagelevel);

	auto skip = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	skip->setPosition(Vec2(winSize.width, 288));
	skip->setAnchorPoint(Vec2(1, 1));
	skip->setScale(0.5);
	skip->setScaleY(0.2);
	skip->setTag(550);

	auto skipMenu = Menu::create(skip, NULL);
	skipMenu->setPosition(Vec2(0, 0));
	addChild(skipMenu, 2);

	char phase[20];
	sprintf(phase, "%d phase", phaseLevel);

	phaseLabel = LabelTTF::create(phase, "Arial", 20);
	phaseLabel->setPosition(Vec2(winSize.width - 50, 288));
	phaseLabel->setAnchorPoint(Vec2(1, 1));
	phaseLabel->setColor(Color3B::BLACK);
	addChild(phaseLabel, 2);


	timerBase = Sprite::create("Images/bar_base.png");
	timerBase->setPosition(Vec2(winSize.width, 290 - 20));
	timerBase->setAnchorPoint(Vec2(1, 1));
	addChild(timerBase,2);

	timerGauge = Sprite::create("Images/bar_gauge.png");

	gauge = 100;

	gaugeBar = ProgressTimer::create(timerGauge);
	gaugeBar->setType(ProgressTimer::Type::BAR);
	gaugeBar->setMidpoint(Vec2(1, 0));
	gaugeBar->setBarChangeRate(Vec2(1, 0));
	gaugeBar->setPercentage(gauge);

	gaugeBar->setPosition(Vec2(0, 0));
	gaugeBar->setAnchorPoint(Vec2(0, 0));
	timerBase->addChild(gaugeBar,3);

	


	this->schedule(schedule_selector(GameStageScene::myTick), 0.5f);

	tmap = TMXTiledMap::create(str);
	tmap->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	tmap->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(tmap, 0, 11);

	heartCreate(5, Vec2(0, 290));
	
	//Size s = tmap->getContentSize();
	//log("ContentSize: %f, %f", s.width, s.height);

	//게임 스테이지 씬
	//타일맵 추가

	masicMenuCreate();
	towerMenuCreate();

	return;
}

void GameStageScene::myTick(float f)
{
	if (phaseLevel == 0)
	{
		gauge = gauge - 4;
	}
	else
	{
		gauge = gauge - 2;
	}
	this->SpriteProgressToRadial(gauge);
	if (gauge < 0)
	{
		gauge = 100;
		log("%dStage",++phaseLevel);
		char phase[20];
		sprintf(phase, "%d phase", phaseLevel);
		phaseLabel->setString(phase);
	}
}
void GameStageScene::SpriteProgressToRadial(float f)
{
	gaugeBar->setPercentage(f);
}

void GameStageScene::onEnter() {
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameStageScene::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(GameStageScene::onTouchMoved, this);
	//listener->onTouchEnded = CC_CALLBACK_2(GameStageScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void GameStageScene::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Layer::onExit();
}
bool GameStageScene::onTouchBegan(Touch* touch, Event* event) {
	auto touchPoint = touch->getLocation();

	log("onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touchPoint.x, touchPoint.y);

	return true;
}

void GameStageScene::onTouchMoved(Touch* touch, Event* event)
{

}

void GameStageScene::onTouchEnded(Touch* touch, Event* event)
{
	
}

void GameStageScene::heartCreate(int num,Vec2 position)
{

	if (num == 0)
	{
		return;
	}
	auto heart = Sprite::create("Images/heart.png");

	heart->setPosition(position);
	heart->setScale(0.4f);
	heart->setAnchorPoint(Vec2(0, 1));

	addChild(heart, 10);

	_heart.pushBack(heart);

	num--;
	position.y = position.y - heart->getContentSize().height / 2.5;
	heartCreate(num, position);
}
Vec2 GameStageScene::positionForTileCoord(Vec2 position)
{
	int x = position.x * tmap->getTileSize().width + tmap->getTileSize().width / 2;
	int y = (tmap->getMapSize().height * tmap->getTileSize().height) -
		(position.y * tmap->getTileSize().height + tmap->getTileSize().height / 2);

	return Vec2(x, y);
}

Vec2 GameStageScene::tileCoordForPosition(Vec2 position)
{
	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y)
		/ tmap->getTileSize().height;
	return Vec2(x, y);
}

void GameStageScene::masicMenuCreate()
{
	auto masicMenuItem1 = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	masicMenuItem1->setPosition(Vec2(0, 0));
	masicMenuItem1->setAnchorPoint(Vec2(0, 0));
	masicMenuItem1->setScale(0.5);
	masicMenuItem1->setTag(350);

	auto masicMenuItem2 = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	masicMenuItem2->setPosition(Vec2(0, masicMenuItem1->getPositionY() +
		masicMenuItem1->getContentSize().height / 2));
	masicMenuItem2->setAnchorPoint(Vec2(0, 0));
	masicMenuItem2->setScale(0.5);
	masicMenuItem2->setTag(351);

	auto masicMenu = Menu::create(masicMenuItem1, masicMenuItem2, NULL);

	masicMenu->setPosition(Vec2(0, 0));
	//pMenu->setAnchorPoint(Vec2(0, 0.5));
	addChild(masicMenu, 2);
}

void GameStageScene::towerMenuCreate()
{
	auto towerMenuItem2 = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem2->setPosition(Vec2(winSize.width / 2, 0));
	towerMenuItem2->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem2->setScale(0.5);
	towerMenuItem2->setTag(450);

	auto towerMenuItem1 = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem1->setPosition(Vec2(towerMenuItem2->getPositionX() -
			towerMenuItem2->getContentSize().width / 2, 0));
	towerMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem1->setScale(0.5);
	towerMenuItem1->setTag(451);

	auto towerMenuItem3 = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem3->setPosition(Vec2(towerMenuItem2->getPositionX() +
		towerMenuItem2->getContentSize().width / 2, 0));
	towerMenuItem3->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem3->setScale(0.5);
	towerMenuItem3->setTag(452);

	auto towerMenuOnOff = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuOnOff->setPosition(Vec2(winSize.width / 2, towerMenuItem2->getPositionY() + 
		towerMenuItem2->getContentSize().height / 2));
	towerMenuOnOff->setAnchorPoint(Vec2(0.5, 0));
	towerMenuOnOff->setScale(0.5);
	towerMenuOnOff->setScaleY(0.2);
	towerMenuOnOff->setTag(453);

	towerMenu = Menu::create(towerMenuItem1, towerMenuItem2, towerMenuItem3, towerMenuOnOff, NULL);

	towerMenu->setPosition(Vec2(0, 0));

	towerMenuSize = towerMenuItem2->getContentSize().height / 2;

	addChild(towerMenu, 2);
}


void GameStageScene::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 453 && towerMenu->numberOfRunningActions() == 0)
	{
		log("OK");
		if (towerMenustatus)
		{
			auto myAction = MoveTo::create(1, Vec2(0, -1 * towerMenuSize));
			towerMenu->runAction(myAction);
			towerMenustatus = false;
		}
		else
		{
			auto myAction = MoveTo::create(1, Vec2(0, 0));
			towerMenu->runAction(myAction);
			towerMenustatus = true;
		}
	}
	else if (i == 550)
	{
		gauge = 0;
	}
	else
	{
		log("%d 메뉴 선택", i);
	}
}
