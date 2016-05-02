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
	winSizePixel = Director::getInstance()->getWinSizeInPixels();
	winSize = Director::getInstance()->getWinSize();
	auto winSize1 = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	char str123[50];
	sprintf(str123, "%f , %f", winSize1.width, winSize1.height);

	auto Label = LabelTTF::create(str123, "Arial", 20);
	Label->setPosition(Vec2(winSize1.width / 2, winSize1.height / 2));
	Label->setAnchorPoint(Vec2(0.5, 0.5));
	Label->setColor(Color3B::BLACK);
	addChild(Label, 2);

	auto pScene = Menus::createScene();
	this->addChild(pScene,2000);

	char str[20];
	sprintf(str, "TileMaps/Level%ld.tmx", stagelevel);

	auto skip = MenuItemImage::create(
		"Images/MenuButton/skip_button.png",
		"Images/MenuButton/skip_button2.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	skip->setPosition(Vec2(winSize.width, winSize.height-31 + origin.y));
	skip->setAnchorPoint(Vec2(1, 1));
	skip->setTag(550);

	auto skipLabel = LabelTTF::create("skip", "Arial", 15);
	skipLabel->setPosition(Vec2(skip->getPositionX() - skip->getContentSize().width/2,
		skip->getPositionY() - skip->getContentSize().height / 2));
	skipLabel->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(skipLabel, 500);

	auto skipMenu = Menu::create(skip, NULL);
	skipMenu->setPosition(Vec2(0, 0));
	addChild(skipMenu, 2);

	char phase[20];
	sprintf(phase, "%d phase", phaseLevel);

	phaseLabel = LabelTTF::create(phase, "Arial", 20);
	phaseLabel->setPosition(Vec2(-10, 0));
	phaseLabel->setAnchorPoint(Vec2(1, 0));
	phaseLabel->setColor(Color3B::WHITE);
	skip->addChild(phaseLabel, 2);


	timerBase = Sprite::create("Images/bar_base.png");
	timerBase->setPosition(Vec2(skip->getContentSize().width, -1));
	timerBase->setAnchorPoint(Vec2(1, 1));
	skip->addChild(timerBase,2);

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
	tmap->setPosition(Vec2(winSize.width / 2, winSize.height / 2 + origin.y));
	tmap->setAnchorPoint(Vec2(0.5, 0.5));
	metainfo = tmap->getLayer("Options");

	objects = tmap->getObjectGroup("ViaPoint");

	ValueMap viapoint = objects->getObject("Start");
	int x = viapoint["x"].asInt() + 15;
	int y = viapoint["y"].asInt() + 15;
	_Vec2Point.push_back(Vec2(x, y));

	for (int n=1;;n++)
	{
		char via[10];
		sprintf(via, "via%d", n);
		ValueMap viapoint = objects->getObject(via);

		int x = viapoint["x"].asInt() + 15;
		int y = viapoint["y"].asInt() + 15;

		if (x == 15 && y == 15)
		{
			break;
		}

		_Vec2Point.push_back(Vec2(x, y));
	}

	viapoint = objects->getObject("End");
	x = viapoint["x"].asInt() + 15;
	y = viapoint["y"].asInt() + 15;
	_Vec2Point.push_back(Vec2(x, y));

	metainfo->setVisible(false);
	this->addChild(tmap, 0, 11);

	_heartCount = 5;

	

	heartCreate(_heartCount, Vec2(0, winSize.height - 31 + origin.y));
	
	_pMonster = &_monster;

	masicMenuCreate();
	towerMenuCreate();
	
	return;
}

Sequence* GameStageScene::SequenceMonsterAdd(int num, int max)
{
	if (num == max)
	{
		auto nullseq = Sequence::create(DelayTime::create(0), nullptr);
		return nullseq;
	}

	auto addAction = Sequence::create(
		CallFunc::create(CC_CALLBACK_0(GameStageScene::addMonster, this)),
		DelayTime::create(1),
		nullptr);

	auto myAction = Sequence::create(
		addAction, SequenceMonsterAdd(++num, max), nullptr);
	return myAction;
}

void GameStageScene::addMonster()
{
	Vec2 startVec2 = _Vec2Point.at(0);

	auto monster = new Monster();

	monster->setTexture("Images/Monster/slime.png");
	monster->setPosition(startVec2);
	monster->runAction(MoveAction(monster));
	
	_monster.pushBack(monster);

	tmap->addChild(monster);
}

Sequence* GameStageScene::MoveAction(Monster* monster)
{
	for (int i = 1; i < _Vec2Point.size(); i++)
	{
		Vec2 beforeVec2 = _Vec2Point.at(i - 1);
		Vec2 afterVec2 = _Vec2Point.at(i);

		float disX = (beforeVec2.x - afterVec2.x);
		float disY = (beforeVec2.y - afterVec2.y);

		float dis;
		if (disX == 0)
		{
			dis = disY;
		}
		else
		{
			dis = disX;
		}

		if (dis < 0)
		{
			dis = -1 * dis;
		}

		auto action = MoveTo::create(dis / 30.0f, Vec2(afterVec2.x, afterVec2.y));

		_Action.pushBack(action);

	}

	auto myAction = Sequence::create(SequenceMoveAction(monster, 0, (_Vec2Point.size() - 2) + 1),
		nullptr);

	myAction->setTag(999);

	_Action.clear();	// 액션 적용 하기 전에 _Action에 저장된 액션들을 지운다.

	return myAction;
}

Sequence* GameStageScene::SequenceMoveAction(Monster* monster, int num , int max)
{
	if (num == max)
	{	
		//auto removeAction = CallFunc::create(re,CC_CALLFUNC_SELECTOR(Node::removeFromParent));
		//_Action의 다음액션이 없을때 호출되어 함수의 종료를 가능하게 해줌
		//DelayTime::create(0)에는 CallFunc를 이용해
		//몬스터의 제거 또는 행동을 추가
		auto nullseq = Sequence::create(DelayTime::create(0),
			CallFunc::create(CC_CALLBACK_0(GameStageScene::removeMonster, this, monster)),
			nullptr);
		return nullseq;
	}

	//현재 _Action의 액션을 순차적으로 가져온다 
	auto actionBefore = _Action.at(num);

	//위의 액션을 Sequence 에 추가후 다른_Action을 추가해줄 SequenceMoveAction() 함수를 재귀
	auto myAction = Sequence::create(
		actionBefore, SequenceMoveAction(monster, ++num , max), nullptr);

	//재귀 함수 호출이 완료되서 함수의 맨위에 있는 nullseq가 반환되면 재귀가 종료되며
	//추가되어있는 Sequence 액션을 반환하여 몬스터에게 적용 시키게 된다.
	return myAction;
}

void GameStageScene::removeMonster(Monster* monster)
{
	for (int i = 0; i < _monster.size(); i++)
	{
		log("_monster.size() : %d", _monster.size());
		auto obj = (Monster*)_monster.at(i);
		if (monster == obj)
		{
			_monster.at(i)->remove();
			_monster.eraseObject(obj);
			/*auto hrartObj = (Sprite*)_heart.at(_heart.size() - 1);
			hrartObj->removeFromParent();
			_heart.popBack();

			if (_heart.size() == 0)
			{
			log("GameOver");
			}*/

			return;
		}
	}
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
		runAction(SequenceMonsterAdd(0, 10));
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
	listener->onTouchMoved = CC_CALLBACK_2(GameStageScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameStageScene::onTouchEnded, this);
	
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 50);
	_listenter = listener;
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameStageScene::onExit() {
	_eventDispatcher->removeEventListener(_listenter);
	Layer::onExit();
}

bool GameStageScene::onTouchBegan(Touch* touch, Event* event) {

	log("onTouchBegan");
	auto touchPoint = touch->getLocation();

	Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);

	if (towerTouch)
	{
		if (towerTpye == 1)
		{
			clickTower = new Tower(1);
			clickTower->setTexture("Images/red.png");
		}
		else if (towerTpye == 2)
		{
			clickTower = new Tower(2);
			clickTower->setTexture("Images/blue.png");
		}
		else
		{
			clickTower = new Tower(3);
			clickTower->setTexture("Images/yellow.png");
		}

		clickTower->setAnchorPoint(Vec2(0.5, 0.5));
		clickTower->setPosition(tmapConvertPoint);
		clickTower->setOpacity(100.f);
		clickTower->setpMonster(_pMonster);

		tmap->addChild(clickTower, 10);
	}

	return true;
}

void GameStageScene::onTouchMoved(Touch* touch, Event* event)
{
	if (towerTouch)
	{
		auto touchPoint = touch->getLocation();
		Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);
		clickTower->setPosition(tmapConvertPoint);
	}
}

void GameStageScene::onTouchEnded(Touch* touch, Event* event)
{

	log("onTouchEnded");
	auto touchPoint = touch->getLocation();

	if (towerTouch)
	{
		Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);

		int xPoint = tmapConvertPoint.x / 30;
		int yPoint = tmapConvertPoint.y / 30;

		for (int i = 0; i != _setupTower.size(); i++)
		{
			//현재 타워가 없는 위치 검색해서 삭제하는 구문
			auto obj = (Tower*)_setupTower.at(i);
			auto bol = obj->towerSetup;
			if (bol)
			{

			}
			else
			{
				_setupTower.eraseObject(obj);
				break;
			}
		}

		for (int i = 0; i != _setupTower.size(); i++)
		{
			auto obj = (Tower*)_setupTower.at(i);

			if (obj->getPosition() == Vec2(xPoint * 30 + 15, yPoint * 30 + 15))
			{
				log("설치 불가");
				clickTower->removeFromParent();
				return;
			}
		}

		Vec2 tileCoord = this->tileCoordForPosition(Vec2(xPoint * 30 + 15, yPoint * 30 + 15));

		int tileGid = this->metainfo->getTileGIDAt(tileCoord);

		if (tileGid)
		{
			log("설치 불가");
			clickTower->removeFromParent();
		}
		else if (1)
		{
			clickTower->setAnchorPoint(Vec2(0.5, 0.5));
			clickTower->setPosition(Vec2(xPoint * 30 + 15, yPoint * 30 + 15));

			_setupTower.pushBack(clickTower);
			towerTouch = false;
		}
	}
}

void GameStageScene::setupTower(Tower* clickTower)
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
	int x = position.x / 30;
	int y = ((tmap->getMapSize().height * 30) - position.y)
		/ 30;
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
	
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	masicMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

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
	towerMenuItem2->setTag(451);

	auto towerMenuItem1 = MenuItemImage::create(
		"Images/box-highres.png",
		"Images/box-highres.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem1->setPosition(Vec2(towerMenuItem2->getPositionX() -
			towerMenuItem2->getContentSize().width / 2, 0));
	towerMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem1->setScale(0.5);
	towerMenuItem1->setTag(450);

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

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	towerMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

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
			auto myAction = MoveBy::create(1, Vec2(0, -1 * towerMenuSize));
			towerMenu->runAction(myAction);
			towerMenustatus = false;
		}
		else
		{
			auto myAction = MoveBy::create(1, Vec2(0, towerMenuSize));
			towerMenu->runAction(myAction);
			towerMenustatus = true;
		}
	}
	else if (i == 550)
	{
		gauge = 0;
	}
	else if(i == 450)
	{
		towerTpye = 1;
		towerTouch = true;
	}
	else if(i == 451)
	{
		towerTpye = 2;
		towerTouch = true;
	}
	else if (i == 452)
	{
		towerTpye = 3;
		towerTouch = true;
	}
	else if (i == 350)
	{
		for (int i = 0; i < _monster.size(); i++)
		{
			auto obj = (Monster*)_monster.at(i);
			obj->hp = obj->hp - 20;
			if (obj->hp <= 0)
			{
				_monster.eraseObject(obj);
			}
		}
	}
}

