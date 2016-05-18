#include "GameStageScene.h"
#include "Menus.h"
#include "GameEnd.h"
#include "SimpleAudioEngine.h"

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

	phaseLevel = 0;

	log("stop");
	nowStageLevel = stagelevel;

	nowStageGold = 30;
	_pnowStageGold = &nowStageGold;
	_pmasicGauge = &masicGaugeNum;
	skipTrue = true;
	firstHero = false;
	SecondHero = false;

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
	
	if (stagelevel == 0)
	{
		auto scenestr = new Menus("InfinityStage");
		scenestr->setpGold(_pnowStageGold);
		scenestr->autorelease();
		pScene->addChild(scenestr);
		this->addChild(pScene, 100);
	}
	else
	{
		auto scenestr = new Menus("GameStageScene");
		scenestr->setpGold(_pnowStageGold);
		scenestr->autorelease();
		pScene->addChild(scenestr);
		this->addChild(pScene, 100);
	}
	
	createStage(stagelevel);

	b_Upgrade = Sprite::create("Images/Button/grey_box1.png");
	b_Upgrade->setPosition(Vec2::ZERO);
	b_Upgrade->setAnchorPoint(Vec2(0.5, 1));
	b_Upgrade->setVisible(false);
	tmap->addChild(b_Upgrade, 102);

	auto upgradeCoin = Sprite::create("Images/Treasure/coin.png");
	upgradeCoin->setPosition(Vec2(b_Upgrade->getContentSize().width / 2,
		b_Upgrade->getContentSize().height / 2));
	upgradeCoin->setScale(0.7f);
	upgradeCoin->setAnchorPoint(Vec2(0.5, 0));
	b_Upgrade->addChild(upgradeCoin);

	upgradeCost = LabelTTF::create("0", "Arial", 16);
	upgradeCost->setPosition(Vec2(b_Upgrade->getContentSize().width / 2,
		b_Upgrade->getContentSize().height / 2));
	upgradeCost->setAnchorPoint(Vec2(0.5, 1));
	upgradeCost->setColor(Color3B::BLACK);
	b_Upgrade->addChild(upgradeCost);

	masicSprite = Sprite::create("Images/spell/lighting-sky-3.png");
	masicSprite->setPosition(Vec2::ZERO);
	masicSprite->setVisible(false);
	masicVisible = false;
	tmap->addChild(masicSprite);

	auto draw_node = DrawNode::create();
	draw_node->setPosition(Vec2(masicSprite->getContentSize().width/2, masicSprite->getContentSize().height / 2));
	masicSprite->addChild(draw_node);
	draw_node->drawCircle(Vec2(0, 0), 45, CC_DEGREES_TO_RADIANS(90), 50, false, Color4F(0, 1, 1, 1));
	
	return;
}

void GameStageScene::createStage(int stagelevel)
{
	char str[20];
	sprintf(str, "TileMaps/Level%ld.tmx", stagelevel);

	if (stagelevel == 0)
	{
		sprintf(str, "TileMaps/Level%ld.tmx", 1);
	}

	auto skip = MenuItemImage::create(
		"Images/MenuButton/skip_button.png",
		"Images/MenuButton/skip_button2.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	skip->setPosition(Vec2(winSize.width, winSize.height - 31 - origin.y));
	skip->setAnchorPoint(Vec2(1, 1));
	skip->setTag(502);

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

	this->addChild(tmap, 0, 501);

	_heartCount = 5;

	heartCreate(_heartCount, Vec2(0, winSize.height - 31 - origin.y));

	_pMonster = &_monster;

	masicMenuCreate();
	towerMenuCreate();
	heroMenuCreate();

}

void GameStageScene::getOption(bool gold, bool masic)
{
	if (gold)
	{
		nowStageGold = nowStageGold + 30;
	}
	if(masic)
	{
		log("충전");
		masicGaugeNum = 100;
	}

}

Sequence* GameStageScene::SequenceMonsterAdd(int num, int max)
{
	if (num == max)
	{
		auto nullseq = Sequence::create(DelayTime::create(0), nullptr);
		return nullseq;
	}

	auto addAction = Sequence::create(
		CallFunc::create(CC_CALLBACK_0(GameStageScene::addMonster, this, phaseLevel)),
		DelayTime::create(1),
		nullptr);

	auto myAction = Sequence::create(
		addAction, SequenceMonsterAdd(++num, max), nullptr);

	return myAction;
}

void GameStageScene::addMonster(int monNum)
{
	Vec2 startVec2 = _Vec2Point.at(0);
	Monster* monster;

	
	if (monNum == 1)
	{
		monster = new Monster("greenslime");
		monster->maxHp = 100.f;
		monster->dropGold = 1;
	}
	else if (monNum == 2)
	{
		monster = new Monster("blueslime");
		monster->maxHp = 120.f;
		monster->dropGold = 1;
	}
	else if (monNum == 3)
	{
		monster = new Monster("yellowslime");
		monster->maxHp = 150.f;
		monster->dropGold = 1;
	}
	else if (monNum == 4)
	{
		monster = new Monster("redslime");
		monster->maxHp = 180.f;
		monster->dropGold = 2;
	}
	else if (monNum == 5)
	{
		monster = new Monster("minotaur");
		monster->maxHp = 200.f;
		monster->dropGold = 2;
	}
	else
	{
		monster = new Monster("minotaur");
		monster->setScale(2.f);
		monster->maxHp = 1000.f;
		monster->boss = true;
		monster->dropGold = 0;
	}
	
	monster->setAnchorPoint(Vec2(0.5, 0));
	monster->setPosition(startVec2);
	auto speed = Speed::create(MoveAction(monster), 1.0f);
	monster->sendSpeed(speed);
	monster->runAction(speed);

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

		float dis = sqrt((disX*disX) + (disY*disY));

		auto action = MoveTo::create(dis / 30.0f, Vec2(afterVec2.x, afterVec2.y));

		_Action.pushBack(action);
	}

	auto myAction = Sequence::create(SequenceMoveAction(monster, 0, (_Vec2Point.size() - 2) + 1),
		nullptr);

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
		if (monster->boss == true)
		{
			auto nullseq = Sequence::create(
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonsterStop, this, monster)),
				DelayTime::create(1),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonster, this, monster)),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonsterStop, this, monster)),
				DelayTime::create(1),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonster, this, monster)),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonsterStop, this, monster)),
				DelayTime::create(1),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonster, this, monster)),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonsterStop, this, monster)),
				DelayTime::create(1),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonster, this, monster)),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonsterStop, this, monster)),
				DelayTime::create(1),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::attackBossMonster, this, monster)),
				nullptr);
			return nullseq;
		}
		else
		{
			auto nullseq = Sequence::create(DelayTime::create(0),
				CallFunc::create(CC_CALLBACK_0(GameStageScene::removeMonster, this, monster)),
				nullptr);
			return nullseq;
		}
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

void GameStageScene::attackBossMonsterStop(Monster* monster)
{
	monster->stopAction(monster->rep);

	char monsterFileName[50];
	sprintf(monsterFileName, "Images/Monster/%s_attack/%d.png", monster->myMonsterName.c_str(), 0);
	monster->setTexture(monsterFileName);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);
	for (int i = 0; i < 10; i++)
	{
		char monsterFileName[50];
		sprintf(monsterFileName, "Images/Monster/%s_attack/%d.png", monster->myMonsterName.c_str(), i);
		animation->addSpriteFrameWithFile(monsterFileName);
	}
	auto animate = Animate::create(animation);
	monster->runAction(animate);
}

void GameStageScene::attackBossMonster(Monster* monster)
{
	if (_heart.size() > 0)
	{
		auto hrartObj = (Sprite*)_heart.at(_heart.size() - 1);
		hrartObj->removeFromParent();
		_heart.popBack();
	}
	if (_heart.size() == 0)
	{
		for (int i = 0; i < _monster.size(); i++)
		{
			auto monsterObj = (Monster*)_monster.at(i);

			monsterObj->stopAllActions();
		}
		if (firstHero)
		{
			hero1->stopAllActions();
			hero1->unscheduleAllSelectors();
		}
		if (SecondHero)
		{
			hero2->stopAllActions();
			hero2->unscheduleAllSelectors();
		}

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

		gameOver = true;
		log("GameOver");

		unscheduleAllSelectors();

		auto pScene = GameEnd::createScene();
		auto stageResult = new GameEnd(nowStageLevel, _heart.size());
		stageResult->autorelease();
		pScene->addChild(stageResult);

		this->addChild(pScene, 3000);
	}

	
}

void GameStageScene::removeMonster(Monster* monster)
{
	for (int i = 0; i < _monster.size(); )
	{
		auto monsterObj = (Monster*)_monster.at(i);

		if (gameOver)
		{
			monsterObj->stopAllActions();
			if (firstHero)
			{
				hero1->stopAllActions();
				hero1->unscheduleAllSelectors();
			}
			if (SecondHero)
			{
				hero2->stopAllActions();
				hero2->unscheduleAllSelectors();
			}

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
	gaugeBar->setPercentage(gauge);
	masicGaugeBar->setPercentage(masicGaugeNum);
	//this->SpriteProgressToRadial(gauge);
	if (gauge < 0 && phaseLevel < 5)
	{
		gauge = 100;
		++phaseLevel;
		char phase[20];
		sprintf(phase, "%d phase", phaseLevel);
		phaseLabel->setString(phase);
		runAction(SequenceMonsterAdd(0, 1));
	}
	if (gauge < 0 && 5 == phaseLevel)
	{
		phaseLevel++;
		phaseLabel->setString("BOSS phase");
		runAction(SequenceMonsterAdd(0, 1));
		this->unschedule(schedule_selector(GameStageScene::myTick));
		this->schedule(schedule_selector(GameStageScene::bossTick));
	}
}

void GameStageScene::bossTick(float f)
{
	masicGaugeBar->setPercentage(masicGaugeNum);
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
	log("onEnter()");
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/Music/Hit Them Harder.mp3", true);
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
	log("onExit()");
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/Music/Woodland Fantasy.mp3", true);
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
		clickTower->setpGold(_pnowStageGold);
		clickTower->setpMasicGauge(_pmasicGauge);
		tmap->addChild(clickTower, 101);
		return true;
	}

	if (masicTouch)
	{
		if (masicTpye == 1)
		{
			masicSprite->setTexture("Images/spell/lighting-sky-3.png");
		}
		else if (masicTpye == 2)
		{
			masicSprite->setTexture("Images/spell/ice-blue-3.png");
		}
		masicSprite->setPosition(tmapConvertPoint);
		masicSprite->setOpacity(100.f);
		masicSprite->setVisible(true);
		masicVisible = true;
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

	if (masicTouch)
	{
		auto touchPoint = touch->getLocation();
		Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);
		masicSprite->setPosition(tmapConvertPoint);
	}

}

void GameStageScene::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	Vec2 tmapConvertPoint = tmap->convertToNodeSpace(touchPoint);

	if (masicTouch && masicVisible)
	{
		if (masicTpye == 1)
		{
			_magictouchPoint = tmapConvertPoint;

			auto cache = SpriteFrameCache::getInstance();
			cache->addSpriteFramesWithFile("fx_f3_starsfury.plist");

			Vector<SpriteFrame*> animFrames;

			char str[100] = { 0 };
			for (int i = 10; i < 45; i++)
			{
				if (i < 10)
				{
					sprintf(str, "fx_f3_starsfury_00%d.png", i);
				}
				else
				{
					sprintf(str, "fx_f3_starsfury_0%d.png", i);
				}

				if (i == 20)
				{
					i = 35;
				}

				log("%s", str);
				SpriteFrame* frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}

			auto pMan = Sprite::createWithSpriteFrameName("fx_f3_starsfury_000.png");
			pMan->setPosition(touchPoint);
			pMan->setAnchorPoint(Vec2(0.5, 0));
			this->addChild(pMan);

			auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
			auto animate = Animate::create(animation);
			auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
			pMan->runAction(seq);

			scheduleOnce(schedule_selector(GameStageScene::magicDeley), 0.5f);

			/*for (int i = 0; i != _monster.size(); i++)
			{
				auto monsterObj = (Monster*)_monster.at(i);
				Vec2 monsterVec2 = monsterObj->getPosition();
				Vec2 dis = tmapConvertPoint - monsterVec2;

				Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));
				if (absDis.x <= 45 && absDis.y <= 45)
				{
					monsterObj->hp -= 100;
					if (monsterObj->hp <= 0)
					{
						_masicMonster.pushBack(monsterObj);
					}
				}
			}
			for (int i = 0; i != _masicMonster.size(); i++)
			{
				_monster.eraseObject(_masicMonster.at(i));
			}
			_masicMonster.clear();
*/
			if (masicGaugeNum > 100)
			{
				masicGaugeNum = 100;
			}

			masicGaugeNum = masicGaugeNum - 80;
			masicGaugeBar->setPercentage(masicGaugeNum);

		}
		else if (masicTpye == 2)
		{
			auto cache = SpriteFrameCache::getInstance();
			cache->addSpriteFramesWithFile("fx_f6_chromaticcold.plist");

			Vector<SpriteFrame*> animFrames;

			char str[100] = { 0 };
			for (int i = 10; i < 20; i++)
			{
				if (i < 10)
				{
					sprintf(str, "fx_f6_chromaticcold_00%d.png", i);
				}
				else
				{
					sprintf(str, "fx_f6_chromaticcold_0%d.png", i);
				}
				log("%s", str);
				SpriteFrame* frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			/////////////
			auto pMan = Sprite::createWithSpriteFrameName("fx_f6_chromaticcold_000.png");
			pMan->setPosition(touchPoint);
			pMan->setAnchorPoint(Vec2(0.5, 0.3));
			this->addChild(pMan);

			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
			pMan->runAction(seq);
			/////////


			for (int i = 0; i != _monster.size(); i++)
			{
				auto monsterObj = (Monster*)_monster.at(i);
				Vec2 monsterVec2 = monsterObj->getPosition();
				Vec2 dis = tmapConvertPoint - monsterVec2;

				Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));
				if (absDis.x <= 45 && absDis.y <= 45)
				{
					monsterObj->hp -= 5;
					monsterObj->speedDown = true;
					monsterObj->speed->setSpeed(0.5f);
					monsterObj->setColor(Color3B::YELLOW);
					if (monsterObj->hp <= 0)
					{
						_masicMonster.pushBack(monsterObj);
					}
				}
			}
			for (int i = 0; i != _masicMonster.size(); i++)
			{
				_monster.eraseObject(_masicMonster.at(i));
			}
			_masicMonster.clear();

			if (masicGaugeNum > 100)
			{
				masicGaugeNum = 100;
			}

			masicGaugeNum = masicGaugeNum - 50;
			masicGaugeBar->setPercentage(masicGaugeNum);
		}


		masicSprite->setVisible(false);
		masicVisible = false;
		masicTouch = false;
		return;
	}

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
			if (b_UpgradeTouch && nowStageGold >= obj->cost)
			{
				nowStageGold = nowStageGold - obj->cost;
				obj->towerUpgradeLevel++;
				char str[50];
				sprintf(str, "Images/Tower/%s%d/Horizontal_3.png", obj->name, obj->towerUpgradeLevel);

				//obj->animationRename();

				char levelViewstr[10];
				sprintf(levelViewstr, "%d", obj->towerUpgradeLevel);
				obj->levelView->setString(levelViewstr);
				obj->setTexture(str);

				obj->_attackPower *= 1.5;
			}
			if (b_UpgradeTouch)
			{
				obj->setOpacity(255.f);
				return;
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

				char upgradeCoststr[50];
				sprintf(upgradeCoststr, "%d", obj->cost);
				upgradeCost->setString(upgradeCoststr);
				if (nowStageGold >= obj->cost)
				{
					upgradeCost->setColor(Color3B::BLACK);
				}
				else
				{
					upgradeCost->setColor(Color3B::RED);
				}
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
	masicMenuItem1->setPosition(Vec2(winSize.width, 0));
	masicMenuItem1->setAnchorPoint(Vec2(1, 0));
	masicMenuItem1->setTag(521);

	auto masicMenuItem2 = MenuItemImage::create(
		"Images/spell/cold.png",
		"Images/spell/cold.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	masicMenuItem2->setPosition(Vec2(winSize.width, masicMenuItem1->getPositionY() +
		masicMenuItem1->getContentSize().height));
	masicMenuItem2->setAnchorPoint(Vec2(1, 0));
	masicMenuItem2->setTag(522);

	auto masicMenu = Menu::create(masicMenuItem1, masicMenuItem2, NULL);
	
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	masicMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

	addChild(masicMenu, 2);


	masicBase = Sprite::create("Images/bar_base.png");
	masicBase->setPosition(Vec2(0, 0));
	masicBase->setAnchorPoint(Vec2(1, 1));
	masicBase->setScaleX(0.6f);
	masicBase->setRotation(90.f);
	masicMenuItem1->addChild(masicBase, 2);

	masicGauge = Sprite::create("Images/bar_gauge.png");

	masicGaugeNum = 0;

	masicGaugeBar = ProgressTimer::create(masicGauge);
	masicGaugeBar->setType(ProgressTimer::Type::BAR);
	masicGaugeBar->setMidpoint(Vec2(1, 0));
	masicGaugeBar->setBarChangeRate(Vec2(1, 0));
	masicGaugeBar->setPercentage(masicGaugeNum);

	masicGaugeBar->setPosition(Vec2(0, 0));
	masicGaugeBar->setAnchorPoint(Vec2(0, 0));
	masicBase->addChild(masicGaugeBar, 3);
}

void GameStageScene::towerMenuCreate()
{
	auto towerMenuItem2 = MenuItemImage::create(
		"Images/Tower/Rogue1/Horizontal_3.png",
		"Images/Tower/Rogue1/Horizontal_1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem2->setPosition(Vec2(winSize.width / 2, 0));
	towerMenuItem2->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem2->setTag(512);

	auto towerMenuItem1 = MenuItemImage::create(
		"Images/Tower/Knight1/Horizontal_3.png",
		"Images/Tower/Knight1/Horizontal_1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem1->setPosition(Vec2(towerMenuItem2->getPositionX() -
			towerMenuItem2->getContentSize().width, 0));
	towerMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem1->setTag(511);

	auto towerMenuItem3 = MenuItemImage::create(
		"Images/Tower/Magician1/Horizontal_3.png",
		"Images/Tower/Magician1/Horizontal_1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuItem3->setPosition(Vec2(towerMenuItem2->getPositionX() +
		towerMenuItem2->getContentSize().width, 0));
	towerMenuItem3->setAnchorPoint(Vec2(0.5, 0));
	towerMenuItem3->setTag(513);

	towerMenuOnOff = MenuItemImage::create(
		"Images/MenuButton/b_menuOn.png",
		"Images/MenuButton/b_menuOn.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	towerMenuOnOff->setPosition(Vec2(winSize.width / 2, towerMenuItem2->getPositionY() + 
		towerMenuItem2->getContentSize().height));
	towerMenuOnOff->setAnchorPoint(Vec2(0.5, 0));
	towerMenuOnOff->setTag(510);

	towerMenu = Menu::create(towerMenuItem1, towerMenuItem2, towerMenuItem3, towerMenuOnOff, NULL);

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	towerMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

	towerMenuSize = towerMenuItem2->getContentSize().height;

	addChild(towerMenu, 2);
}

void GameStageScene::heroMenuCreate()
{

	heroMenuItem1 = MenuItemImage::create(
		"Images/Hero/hero1.png",
		"Images/Hero/hero1.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	heroMenuItem1->setPosition(Vec2(0, 0));
	heroMenuItem1->setAnchorPoint(Vec2(0, 0));
	heroMenuItem1->setTag(531);


	heroMenuItem2 = MenuItemImage::create(
		"Images/Hero/hero2.png",
		"Images/Hero/hero2.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	heroMenuItem2->setPosition(Vec2(heroMenuItem1->getContentSize().width, 0));
	heroMenuItem2->setAnchorPoint(Vec2(0, 0));
	heroMenuItem2->setTag(532);

	

	auto heroMenuItem3 = MenuItemImage::create(
		"Images/Hero/hero3.png",
		"Images/Hero/hero3.png",
		CC_CALLBACK_1(GameStageScene::doClick, this));
	heroMenuItem3->setPosition(Vec2(heroMenuItem1->getContentSize().width + heroMenuItem2->getContentSize().width, 0));
	heroMenuItem3->setAnchorPoint(Vec2(0, 0));
	heroMenuItem3->setTag(533);

	bool hero1_have = UserDefault::getInstance()->getBoolForKey("Hero1");
	bool hero2_have = UserDefault::getInstance()->getBoolForKey("Hero2");
	bool hero3_have = UserDefault::getInstance()->getBoolForKey("Hero3");

	heroMenuItem1->setOpacity(100.f);
	heroMenuItem2->setOpacity(100.f);
	heroMenuItem3->setOpacity(100.f);

	if(hero1_have)
	heroMenuItem1->setOpacity(255.f);
	if(hero2_have)
	heroMenuItem2->setOpacity(255.f);
	if(hero3_have)
	heroMenuItem3->setOpacity(255.f);

	auto heroMenu = Menu::create(heroMenuItem1, heroMenuItem2, heroMenuItem3, NULL);

	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	heroMenu->setPosition(Vec2(0 + origin.x, 0 + origin.y));

//	auto heroMenuItem3 = heroMenuItem3->getContentSize().height;

	addChild(heroMenu, 2);
}

void GameStageScene::doClick(Ref* pSender)
{
	towerTouch = false;
	masicTouch = false;
	bool hero1_have = UserDefault::getInstance()->getBoolForKey("Hero1");
	bool hero2_have = UserDefault::getInstance()->getBoolForKey("Hero2");
	bool hero3_have = UserDefault::getInstance()->getBoolForKey("Hero3");
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

	//메뉴 클릭시 타워 업그레이드 중지
	towerUpgradeVisible = false;
	b_Upgrade->setVisible(false);
	for (int i = 0; i != _setupTower.size(); i++)
	{
		auto obj = (Tower*)_setupTower.at(i);
		obj->setOpacity(255.f);
	}

	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 510 && towerMenu->numberOfRunningActions() == 0)
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
	else if (i == 502 && skipTrue)
	{
		gauge = 0;
		skipTrue = false;
		scheduleOnce(schedule_selector(GameStageScene::trueFalse), 5.0f);
	}
	else if(i == 511 && (*_pnowStageGold) >= 10)
	{
		towerTpye = 1;
		towerTouch = true;
	}
	else if(i == 512 && (*_pnowStageGold) >= 20)
	{
		towerTpye = 2;
		towerTouch = true;
	}
	else if (i == 513 && (*_pnowStageGold) >= 30)
	{
		towerTpye = 3;
		towerTouch = true;
	}
	else if (i == 521 && masicGaugeNum >= 80)
	{
		

		masicTpye = 1;
		masicTouch = true;
		////번개 마법
		//for (int i = 0; i < _monster.size(); i++)
		//{
		//	auto obj = (Monster*)_monster.at(i);
		//	obj->hp = obj->hp - 20;
		//	if (obj->hp <= 0)
		//	{
		//		_monster.eraseObject(obj);
		//	}
		//}
	}
	else if (i == 522 && masicGaugeNum >= 50)
	{
		masicTpye = 2;
		masicTouch = true;
		//nowStageGold = nowStageGold + 50;
		//얼음
	}
	else if (i == 531 && !firstHero && hero1_have)
	{
		log("Hero1");
		hero1 = new Hero(1);
		hero1->setPosition(_Vec2Point.at(_Vec2Point.size() - 1));
		hero1->setOpacity(240.f);
		hero1->setpMonster(_pMonster);
		hero1->setpGold(_pnowStageGold);
		hero1->setpMasicGauge(_pmasicGauge);

		tmap->addChild(hero1, 200);
		firstHero = true;
		heroMenuItem1->setOpacity(100.f);
		//heroMenuItem1->setNormalImage(Sprite::create("Images/Hero/hero3.png"));
		//heroMenuItem1->setSelectedImage(Sprite::create("Images/Hero/hero2.png"));
	}
	else if (i == 532 && !SecondHero && hero2_have)
	{
		log("Hero2");
		hero2 = new Hero(2);
		hero2->setPosition(_Vec2Point.at(_Vec2Point.size() - 1));
		hero2->setOpacity(240.f);
		hero2->setpMonster(_pMonster);
		hero2->setpGold(_pnowStageGold);
		hero2->setpMasicGauge(_pmasicGauge);

		tmap->addChild(hero2, 200);
		SecondHero = true;
		heroMenuItem2->setOpacity(100.f);
	}
//	else if (i == 533 && hero3_have)
	else if (i == 533)
	{
		log("Hero3");
		masicGaugeNum = 100;
	}
}

void GameStageScene::trueFalse(float f)
{
	if (skipTrue == true)
	{
		skipTrue = false;
	}
	else if (skipTrue == false)
	{
		skipTrue = true;
	}
}

void GameStageScene::magicDeley(float dt)
{
	auto tmapConvertPoint = _magictouchPoint;
	for (int i = 0; i != _monster.size(); i++)
	{
		auto monsterObj = (Monster*)_monster.at(i);
		Vec2 monsterVec2 = monsterObj->getPosition();
		Vec2 dis = tmapConvertPoint - monsterVec2;

		Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));
		if (absDis.x <= 45 && absDis.y <= 45)
		{
			monsterObj->hp -= 100;
			if (monsterObj->hp <= 0)
			{
				_masicMonster.pushBack(monsterObj);
			}
		}
	}
	for (int i = 0; i != _masicMonster.size(); i++)
	{
		_monster.eraseObject(_masicMonster.at(i));
	}
	_masicMonster.clear();

}