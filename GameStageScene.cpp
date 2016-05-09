#include "GameStageScene.h"
#include "Menus.h"
#include "GameEnd.h"

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

	nowStageLevel = stagelevel;

	gameOver = false;
	towerStop = true;
	towerUpgradeVisible = false;

	winSizePixel = Director::getInstance()->getWinSizeInPixels();
	winSize = Director::getInstance()->getWinSize();
	VisibleWinSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();


	/*
	char winSizeStr[50];
	sprintf(winSizeStr, "%f , %f", winSize.width, winSize.height);
	
	char VisibleWinSizeStr[50];
	sprintf(VisibleWinSizeStr, "%f , %f", VisibleWinSize.width, VisibleWinSize.height);

	auto Label = LabelTTF::create(winSizeStr, "Arial", 20);
	Label->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	Label->setAnchorPoint(Vec2(0.5, 0.5));
	Label->setColor(Color3B::BLACK);
	addChild(Label, 2);

	auto Label1 = LabelTTF::create(VisibleWinSizeStr, "Arial", 20);
	Label1->setPosition(Vec2(VisibleWinSize.width / 2, VisibleWinSize.height / 2));
	Label1->setAnchorPoint(Vec2(0.5, 0.5));
	Label1->setColor(Color3B::RED);
	addChild(Label1, 2);*/

	auto pScene = Menus::createScene();
	
	this->addChild(pScene, 100);


	createStage(stagelevel);

	b_Upgrade = Sprite::create("Images/Button/b_Background.png");
	b_Upgrade->setPosition(Vec2(100, 100));
	b_Upgrade->setAnchorPoint(Vec2(0.5, 1));
	b_Upgrade->setVisible(false);
	tmap->addChild(b_Upgrade, 102);
	
	return;
}

void GameStageScene::createStage(int stagelevel)
{
	char str[20];
	sprintf(str, "TileMaps/Level%ld.tmx", stagelevel);

	auto skip = MenuItemImage::create(
		"Images/MenuButton/skip_button.png",
		"Images/MenuButton/skip_button2.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	skip->setPosition(Vec2(winSize.width, winSize.height - 31 - origin.y));
	skip->setAnchorPoint(Vec2(1, 1));
	skip->setTag(550);

	auto skipLabel = LabelTTF::create("skip", "Arial", 15);
	skipLabel->setPosition(Vec2(skip->getPositionX() - skip->getContentSize().width / 2,
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
	skip->addChild(timerBase, 2);

	timerGauge = Sprite::create("Images/bar_gauge.png");

	gauge = 100;

	gaugeBar = ProgressTimer::create(timerGauge);
	gaugeBar->setType(ProgressTimer::Type::BAR);
	gaugeBar->setMidpoint(Vec2(1, 0));
	gaugeBar->setBarChangeRate(Vec2(1, 0));
	gaugeBar->setPercentage(gauge);

	gaugeBar->setPosition(Vec2(0, 0));
	gaugeBar->setAnchorPoint(Vec2(0, 0));
	timerBase->addChild(gaugeBar, 3);

	this->schedule(schedule_selector(GameStageScene::myTick), 0.5f);

	tmap = TMXTiledMap::create(str);

	tmap->setPosition(Vec2(
		(VisibleWinSize.width / 2),
		(VisibleWinSize.height / 2)
		));

	tmap->setAnchorPoint(Vec2(0.5, 0.5));
	metainfo = tmap->getLayer("Options");

	objects = tmap->getObjectGroup("ViaPoint");

	ValueMap viapoint = objects->getObject("Start");
	int x = viapoint["x"].asInt() + 15;
	int y = viapoint["y"].asInt() + 15;
	_Vec2Point.push_back(Vec2(x, y));

	for (int n = 1;; n++)
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

	heartCreate(_heartCount, Vec2(0, winSize.height - 31 - origin.y));

	_pMonster = &_monster;

	masicMenuCreate();
	towerMenuCreate();

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
	for (int i = 0; i < _monster.size(); )
	{
		auto monsterObj = (Monster*)_monster.at(i);

		

		if (gameOver)
		{
			monsterObj->stopAllActions();
			if (towerStop)
			{
				
				stopAllActions();

				for (int j = 0; j < _setupTower.size(); j++)
				{
					auto towerObj = (Tower*)_setupTower.at(j);
					towerObj->unscheduleAllSelectors();
				}
				towerStop = false;
			}
		}
		
		if (monster == monsterObj)
		{
			
			if (_heart.size() > 0)
			{
				_monster.at(i)->remove();
				_monster.eraseObject(monsterObj);
				auto hrartObj = (Sprite*)_heart.at(_heart.size() - 1);
				hrartObj->removeFromParent();
				_heart.popBack();

			}
			if (_heart.size() == 0 && !gameOver)
			{
				i--;
				gameOver = true;
				log("GameOver");
				
				unscheduleAllSelectors();

				auto pScene = GameEnd::createScene();
				auto stageResult = new GameEnd(nowStageLevel, _heart.size());
				stageResult->autorelease();
				pScene->addChild(stageResult);

				this->addChild(pScene, 3000);
			}
			
			
			//return;
		}
		i++;
	}
}

void GameStageScene::myTick(float f)
{
	if (phaseLevel == 0)
	{
		gauge = gauge - 4;
	}
	else if (phaseLevel <= 5)
	{
		gauge = gauge - 2;
	}
	gaugeBar->setPercentage(f);
	//this->SpriteProgressToRadial(gauge);
	if (gauge < 0 && phaseLevel < 5)
	{
		gauge = 100;
		++phaseLevel;
		char phase[20];
		sprintf(phase, "%d phase", phaseLevel);
		phaseLabel->setString(phase);
		runAction(SequenceMonsterAdd(0, 10));
	}
	if (gauge == 0 && 5 == phaseLevel)
	{
		phaseLabel->setString("BOSS phase");
		runAction(SequenceMonsterAdd(0, 1));
		this->unschedule(schedule_selector(GameStageScene::myTick));
		this->schedule(schedule_selector(GameStageScene::bossTick));
	}
}

void GameStageScene::bossTick(float f)
{
	if (_monster.size() == 0)
	{
		log("clear!!");

		auto pScene = GameEnd::createScene();
		auto stageResult = new GameEnd(nowStageLevel, _heart.size());
		stageResult->autorelease();
		pScene->addChild(stageResult);

		this->addChild(pScene, 3000);

		this->unschedule(schedule_selector(GameStageScene::bossTick));
	}

}

void GameStageScene::onEnter() {
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(GameStageScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameStageScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameStageScene::onTouchEnded, this);
	
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_listenter = listener;
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameStageScene::onExit() {
	_eventDispatcher->removeEventListener(_listenter);
	Layer::onExit();
}

bool GameStageScene::onTouchBegan(Touch* touch, Event* event) {
	auto touchPoint = touch->getLocation();

	Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);

	for (int i = 0; i != _setupTower.size(); i++)
	{
		//타워 설치중이면 바로 리턴
		auto obj = (Tower*)_setupTower.at(i);
		auto bol = obj->towerMenuVisible;
		if (bol)
		{
			return false;
		}
	}

	if (towerTouch)
	{
		clickTower = new Tower(towerTpye);
		clickTower->setPosition(tmapConvertPoint);
		clickTower->setOpacity(100.f);
		clickTower->setpMonster(_pMonster);

		tmap->addChild(clickTower, 101);
		return true;
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
	auto touchPoint = touch->getLocation();
	Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);

	if (towerTouch)
	{
		//Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);

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
				//log("설치 불가");
				clickTower->removeFromParent();
				return;
			}
		}

		Vec2 tileCoord = this->tileCoordForPosition(Vec2(xPoint * 30 + 15, yPoint * 30 + 15));

		int tileGid = this->metainfo->getTileGIDAt(tileCoord);

		if (tileGid)
		{
			//log("설치 불가");
			clickTower->removeFromParent();
		}
		else if (1)
		{
			//clickTower->setAnchorPoint(Vec2(0.5, 0.5));
			clickTower->setPosition(Vec2(xPoint * 30 + 15, yPoint * 30 + 15));
			if (clickTower->getPositionY() >= 280.f)
			{
				clickTower->b_Yes->setPosition(Vec2(clickTower->towerContentSize.width / 2, 0));
				clickTower->b_Yes->setAnchorPoint(Vec2(0, 1));
				clickTower->b_No->setPosition(Vec2(clickTower->towerContentSize.width / 2, 0));
				clickTower->b_No->setAnchorPoint(Vec2(1, 1));
			}

			_setupTower.pushBack(clickTower);
			towerTouch = false;
		}
	}
	else
	{
		if (towerUpgradeVisible)
		{
			towerUpgradeVisible = false;
			b_Upgrade->setVisible(false);
			auto obj = (Tower*)_setupTower.at(upgradeTowerNum);
			bool b_UpgradeTouch = b_Upgrade->getBoundingBox().containsPoint(tmapConvertPoint);
			if (b_UpgradeTouch)
			{
				obj->towerUpgradeLevel++;
				char str[50];
				sprintf(str, "Images/Tower/%s%d/Horizontal_3.png", obj->name, obj->towerUpgradeLevel);

				char levelViewstr[10];
				sprintf(levelViewstr, "%d", obj->towerUpgradeLevel);
				obj->levelView->setString(levelViewstr);
				obj->setTexture(str);

				obj->_attackPower *= 1.5;
			}
		}

		for (int i = 0; i != _setupTower.size(); i++)
		{
			auto obj = (Tower*)_setupTower.at(i);
			Rect rect = Rect(obj->getPositionX() - 15,
				obj->getPositionY() - 15,
				30,	30);
			//Vec2 LocationInNode = this->convertToNodeSpace(touchPoint);
			obj->setOpacity(255.f);

			if (rect.containsPoint(tmapConvertPoint) && obj->towerUpgradeLevel < 3)
			{
				int xPoint = obj->getPositionX() / 30;
				int yPoint = obj->getPositionY() / 30;

				b_Upgrade->setPosition(Vec2(xPoint * 30 + 15, yPoint * 30));

				upgradeTowerNum = i;
				towerUpgradeVisible = true;
				b_Upgrade->setVisible(true);
				obj->setOpacity(150.f);
			}
		}
	}	
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
		"Images/spell/lighting-sky-2.png",
		"Images/spell/lighting-sky-2.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	masicMenuItem1->setPosition(Vec2(0, 0));
	masicMenuItem1->setAnchorPoint(Vec2(0, 0));
	masicMenuItem1->setTag(350);

	auto masicMenuItem2 = MenuItemImage::create(
		"Images/spell/ice-blue-2.png",
		"Images/spell/ice-blue-2.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	masicMenuItem2->setPosition(Vec2(0, masicMenuItem1->getPositionY() +
		masicMenuItem1->getContentSize().height));
	masicMenuItem2->setAnchorPoint(Vec2(0, 0));
	masicMenuItem2->setTag(351);

	auto masicMenu = Menu::create(masicMenuItem1, masicMenuItem2, NULL);
	
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	masicMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

	addChild(masicMenu, 2);
}

void GameStageScene::towerMenuCreate()
{
	auto towerMenuItem2 = MenuItemImage::create(
		"Images/Tower/Rogue1/Horizontal_3.png",
		"Images/Tower/Rogue1/Horizontal_1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem2->setPosition(Vec2(winSize.width / 2, 0));
	towerMenuItem2->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem2->setTag(451);

	auto towerMenuItem1 = MenuItemImage::create(
		"Images/Tower/Knight1/Horizontal_3.png",
		"Images/Tower/Knight1/Horizontal_1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem1->setPosition(Vec2(towerMenuItem2->getPositionX() -
			towerMenuItem2->getContentSize().width, 0));
	towerMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem1->setTag(450);

	auto towerMenuItem3 = MenuItemImage::create(
		"Images/Tower/Magician1/Horizontal_3.png",
		"Images/Tower/Magician1/Horizontal_1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem3->setPosition(Vec2(towerMenuItem2->getPositionX() +
		towerMenuItem2->getContentSize().width, 0));
	towerMenuItem3->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem3->setTag(452);

	towerMenuOnOff = MenuItemImage::create(
		"Images/MenuButton/b_menuOn.png",
		"Images/MenuButton/b_menuOn.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuOnOff->setPosition(Vec2(winSize.width / 2, towerMenuItem2->getPositionY() + 
		towerMenuItem2->getContentSize().height));
	towerMenuOnOff->setAnchorPoint(Vec2(0.5, 0));
	towerMenuOnOff->setTag(453);

	towerMenu = Menu::create(towerMenuItem1, towerMenuItem2, towerMenuItem3, towerMenuOnOff, NULL);

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	towerMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

	towerMenuSize = towerMenuItem2->getContentSize().height;

	addChild(towerMenu, 2);
}

void GameStageScene::doClick(Ref* pSender)
{

	//메뉴 클릭시 타워 업그레이드 중지
	towerUpgradeVisible = false;
	b_Upgrade->setVisible(false);
	for (int i = 0; i != _setupTower.size(); i++)
	{
		auto obj = (Tower*)_setupTower.at(i);
		obj->setOpacity(255.f);
	}

	//타워 설치중이면 메뉴 클릭 불가능
	for (int i = 0; i != _setupTower.size(); i++)
	{
		auto obj = (Tower*)_setupTower.at(i);
		auto bol = obj->towerMenuVisible;
		if (bol)
		{
			return;
		}
	}

	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 453 && towerMenu->numberOfRunningActions() == 0)
	{
		if (towerMenustatus)
		{
			log("OK");
			auto myAction = MoveBy::create(1, Vec2(0, -1 * towerMenuSize));
			towerMenu->runAction(myAction);
			towerMenuOnOff->setRotation(180.f);
			towerMenuOnOff->setAnchorPoint(Vec2(0.5, 1));
			towerMenustatus = false;
		}
		else
		{
			log("OK1");
			auto myAction = MoveBy::create(1, Vec2(0, towerMenuSize));
			towerMenu->runAction(myAction);
			towerMenuOnOff->setRotation(0.f);
			towerMenuOnOff->setAnchorPoint(Vec2(0.5, 0));
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
		//번개 마법
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
	else if (i == 351)
	{
		//얼음
	}
}

