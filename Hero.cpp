#include "Hero.h"
#include "GameStageScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Hero::Hero(int typenum)
	: _useNodePriority(false)
	, _heroType(typenum)
	, _attackDelay(1.0f)
	, _attackPower(10.0f)
{
	bool b0k = initWithTexture(nullptr, Rect::ZERO);
	if (b0k)
	{
		this->autorelease();
	}
}

void Hero::setHeroSetting()
{
	if (_heroType == 1)
	{
		_attackDelay = 0.5f;
		int level = UserDefault::getInstance()->getIntegerForKey("Hero1_Level");
		_level = level;
		_attackPower = 25.0f + (level-1) * 2;
		setTexture("Images/Hero/Knight/Walking/Horizontal_1.png");
		setScale(1.5f);
		sprintf(name, "Knight");
	}
	else if (_heroType == 2)
	{
		_attackDelay = 1.0f;
		int level = UserDefault::getInstance()->getIntegerForKey("Hero2_Level");
		_level = level;
		_attackPower = 20.0f + (level-1) * 3;
		setTexture("Images/Tower/Rogue1/Horizontal_3.png");	//Rogue 도적.
		setScale(1.5f);
		sprintf(name, "Rogue");
	}
	else if (_heroType == 3)
	{
		_attackDelay = 2.0f;
		int level = UserDefault::getInstance()->getIntegerForKey("Hero3_Level");
		_level = level;
		_attackPower = 30.0f + (level-1) * 5;
		setTexture("Images/Tower/Magician1/Horizontal_3.png");
		setScale(1.5f);
		sprintf(name, "Magician");
	}

	setAnchorPoint(Vec2(0.5, 0.3));
}
void Hero::setPriorityWithThis(bool useNodePriority)
{
	_useNodePriority = useNodePriority;
}

void Hero::setpMonster(Vector<Monster*> *_repMonster)
{
	_pMonster = _repMonster;
}

void Hero::setpGold(int * _pnowStageGold)
{
	nowStageGold = _pnowStageGold;
}

void Hero::setpMasicGauge(int *_pmasicGauge)
{
	nowMasicGauge = _pmasicGauge;
}

void Hero::onEnter()
{
	srand((int)time(NULL));
	Sprite::onEnter();
	setHeroSetting();


	sprintf(animationStr1, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 1);
	sprintf(animationStr2, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 2);
	sprintf(animationStr3, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 3);


	schedule(schedule_selector(Hero::heroTick), _attackDelay);

	near1 = false;
};

void Hero::onExit() {
	//_eventDispatcher->removeEventListener(_listenter);
	Sprite::onExit();
}

void Hero::heroTick(float a)
{
	Vec2 myPos = this->getPosition();
	absMaxDis = Vec2(3000, 3000);
	minDis = Vec2(0, 0);
	float disVec = 0;
	for (int i = 0; i < (*_pMonster).size(); i++)
	{
		
		auto obj = (Monster*)(*_pMonster).at(i);
		Vec2 objVec2 = obj->getPosition();
		Vec2 dis = objVec2 - myPos;

		

		Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));

		if (obj->getPositionX() > 400.0f)
		{
			continue;
		}
		if (_heroType == 1 && obj->_fly)
		{
			continue;
		}
		else if (absMaxDis > absDis)
		{
			absMaxDis = absDis;
			minDis = dis;
			nearMonster = obj;
			//near1 = true;
			disVec = sqrt((fabs(dis.x) * fabs(dis.x)) + (fabs(dis.y) * fabs(dis.y)));
		}
		

		if (_heroType == 1)
		{
			if (absDis.x <= 45 && absDis.y <= 45)
			{
				setFlippedX(false);


				int a = rand() % 100 + 1;
				int level = UserDefault::getInstance()->getIntegerForKey("Hero1_Level");
				if (a < level + 5)
				{
					if (obj->boss == false)
					{
						obj->speed->setSpeed(0.0f);
						obj->setColor(Color3B::GRAY);
						obj->speedDown = true;
						auto cache = SpriteFrameCache::getInstance();
						cache->addSpriteFramesWithFile("Images/Hero/spell/fx_f1_decimate.plist");

						Vector<SpriteFrame*> animFrames;

						char str[100] = { 0 };
						for (int i = 0; i < 3; i++)
						{
							if (i < 10)
							{
								sprintf(str, "fx_f1_decimate_00%d.png", i);
							}
							else
							{
								sprintf(str, "fx_f1_decimate_0%d.png", i);
							}
							SpriteFrame* frame = cache->getSpriteFrameByName(str);
							animFrames.pushBack(frame);
						}
						/////////////
						auto pMan = Sprite::createWithSpriteFrameName("fx_f1_decimate_000.png");
						//pMan->setScale(1.5f);
						pMan->setPosition((dis / 1.5) + Vec2((getContentSize().width / 2), (getContentSize().height / 2)));
						pMan->setAnchorPoint(Vec2(0.5, 0.5));
						this->addChild(pMan);

						auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
						auto animate = Animate::create(animation);
						auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
						pMan->runAction(seq);
					}
				}

				stopAllActions();
				//(*_pMonster).at(i)->hp -= _attackPower;

				attackedMonsterHero = obj;
				scheduleOnce(schedule_selector(Hero::attackDeley), 0.1f);

				setAnimation(absDis, dis);
				/*if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}*/

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.16f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}

		
		else if (_heroType == 2) {
			if (absDis.x <= 75 && absDis.y <= 75)
			{
				setFlippedX(false);
				stopAllActions();
				//(*_pMonster).at(i)->hp -= _attackPower;

				auto ball = Sprite::create("Images/Tower/Green_Dagger.png");
				Vec2 shootVector = obj->getPosition() - getPosition();
				float shootAngle = shootVector.getAngle();
				float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
				ball->setRotation(cocosAngle + 90);

				ball->setPosition(getContentSize().width / 2, getContentSize().height / 2);
				addChild(ball);
				ball->runAction(Sequence::create(MoveBy::create(0.2, dis/1.5), RemoveSelf::create(), nullptr));

				attackedMonsterHero = obj;
				scheduleOnce(schedule_selector(Hero::attackDeley), 0.2f);

				/*if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}*/

				setAnimation(absDis, dis);

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.1f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}
		else if (_heroType == 3) {
			if (absDis.x <= 75 && absDis.y <= 75)
			{
				setFlippedX(false);
				/*(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}
*/
				stopAllActions();
				//(*_pMonster).at(i)->hp -= _attackPower;
				auto ball = Sprite::create("Images/Tower/ice_ball.png");
				ball->setPosition(getContentSize().width / 2, getContentSize().height / 2);
				addChild(ball);
				ball->runAction(Sequence::create(MoveBy::create(0.5, dis / 1.5), RemoveSelf::create(), nullptr));

				attackedMonsterHero = obj;
				splashVec = obj->getPosition();
				scheduleOnce(schedule_selector(Hero::attackDeley), 0.5f);

				setAnimation(absDis, dis);

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.1f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}
	}

	stopAllActions();

	bool moveOK = false;

	if (_heroType == 3)
	{
		for (int i = 0; i < (*_pMonster).size(); i++)
		{
			auto obj = (Monster*)(*_pMonster).at(i);

			if (obj->getPositionX() >= 400.0f)
			{
				moveOK = false;
			}
			else
			{
				moveOK = true;
				break;
			}
		}

		if ((*_pMonster).size() != 0 && moveOK)
		{
			if (nearMonster->getPositionX() >= 400.f)
			{
				return;
			}
			else
			{
				auto cache = SpriteFrameCache::getInstance();
				cache->addSpriteFramesWithFile("Images/Hero/spell/f3_fx_entropicdecay.plist");

				Vector<SpriteFrame*> animFrames;

				char str[100] = { 0 };
				for (int i = 0; i < 30; i++)
				{
					if (i < 10)
					{
						sprintf(str, "f3_fx_entropicdecay_00%d.png", i);
					}
					else
					{
						sprintf(str, "f3_fx_entropicdecay_0%d.png", i);
					}
					//log("%s", str);
					SpriteFrame* frame = cache->getSpriteFrameByName(str);
					animFrames.pushBack(frame);
				}
				/////////////
				/*auto pMan = Sprite::createWithSpriteFrameName("f3_fx_entropicdecay_000.png");
				pMan->setPosition(getPosition());
				pMan->setAnchorPoint(Vec2(0.5, 0.3));
				this->addChild(pMan);
				*/
				auto animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
				auto animate = Animate::create(animation);
				//auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
				auto seq = Sequence::create(animate,
					CallFunc::create(CC_CALLBACK_0(Hero::setDefTexture, this)),
					nullptr);
				runAction(seq);

				//setVisible(false);
				magicainMoveVec = nearMonster->getPosition();
				scheduleOnce(schedule_selector(Hero::magicianMove), 1.0f);
			}
		}
		return;
		
	}

	char MoveAnimationStr1[50];
	char MoveAnimationStr2[50];
	char MoveAnimationStr3[50];
	char MoveAnimationStr4[50];

	setFlippedX(false);
	if ((*_pMonster).size() != 0)
	{
		if (abs(minDis.x) > abs(minDis.y))
		{
			if (minDis.x > 0)
			{
				sprintf(MoveAnimationStr1, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 1);
				sprintf(MoveAnimationStr2, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 2);
				sprintf(MoveAnimationStr3, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 3);
				sprintf(MoveAnimationStr4, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 4);
			}
			else
			{
				sprintf(MoveAnimationStr1, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 1);
				sprintf(MoveAnimationStr2, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 2);
				sprintf(MoveAnimationStr3, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 3);
				sprintf(MoveAnimationStr4, "Images/Hero/%s/Walking/Horizontal_%d.png", name, 4);
				setFlippedX(true);
			}
		}
		else
		{
			if (minDis.y > 0)
			{
				sprintf(MoveAnimationStr1, "Images/Hero/%s/Walking/Up_%d.png", name, 1);
				sprintf(MoveAnimationStr2, "Images/Hero/%s/Walking/Up_%d.png", name, 2);
				sprintf(MoveAnimationStr3, "Images/Hero/%s/Walking/Up_%d.png", name, 3);
				sprintf(MoveAnimationStr4, "Images/Hero/%s/Walking/Up_%d.png", name, 4);
			}
			else
			{
				sprintf(MoveAnimationStr1, "Images/Hero/%s/Walking/Down_%d.png", name, 1);
				sprintf(MoveAnimationStr2, "Images/Hero/%s/Walking/Down_%d.png", name, 2);
				sprintf(MoveAnimationStr3, "Images/Hero/%s/Walking/Down_%d.png", name, 3);
				sprintf(MoveAnimationStr4, "Images/Hero/%s/Walking/Down_%d.png", name, 4);
			}
		}

		auto animation = Animation::create();

		if (_heroType == 1)
		{
			animation->setDelayPerUnit(0.1f);
		}
		if (_heroType == 2)
		{
			animation->setDelayPerUnit(0.12f);
			animation->addSpriteFrameWithFile(MoveAnimationStr1);
			animation->addSpriteFrameWithFile(MoveAnimationStr2);
			animation->addSpriteFrameWithFile(MoveAnimationStr3);
			animation->addSpriteFrameWithFile(MoveAnimationStr4);
		}

		animation->addSpriteFrameWithFile(MoveAnimationStr1);
		animation->addSpriteFrameWithFile(MoveAnimationStr2);
		animation->addSpriteFrameWithFile(MoveAnimationStr3);
		animation->addSpriteFrameWithFile(MoveAnimationStr4);
		auto animate = Animate::create(animation);

		moveOK = true;


		for (int i = 0; i < (*_pMonster).size(); i++)
		{
			auto obj = (Monster*)(*_pMonster).at(i);

			if (_heroType == 1 && obj->_fly)
			{
				moveOK = false;
			}
			else if (obj->getPositionX() >= 400.0f)
			{
				moveOK = false;
			}
			else
			{
				moveOK = true;
				runAction(animate);
				break;
			}
		}
	}
	else
	{
		char def[50];
		sprintf(def, "Images/Hero/%s/Walking/Horizontal_1.png", name);
		setTexture(def);
	}

	if (moveOK)
	{
		if (nearMonster->getPositionX() >= 400.f)
		{
			stopAllActions();
			char def[50];
			sprintf(def, "Images/Hero/%s/Walking/Horizontal_1.png", name);
			setTexture(def);
		}
		else
		{
			auto moveb = MoveBy::create(disVec / 60.f, minDis);
			runAction(moveb);
		}
	}
	else
	{
		stopAllActions();
		char def[50];
		sprintf(def, "Images/Hero/%s/Walking/Horizontal_1.png", name);
		setTexture(def);
	}
}

void Hero::magicianMove(float dt)
{
	setPosition(magicainMoveVec);
}

void Hero::setDefTexture()
{
	stopAllActions();
	setTexture("Images/Hero/hero3.png");
}

void Hero::setAnimation(cocos2d::Vec2 absDis, cocos2d::Vec2 dis)
{
	if (absDis.x > absDis.y)
	{
		if (dis.x > 0)
		{
			sprintf(animationStr1, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 1);
			sprintf(animationStr2, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 2);
			sprintf(animationStr3, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 3);
		}
		else
		{
			sprintf(animationStr1, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 1);
			sprintf(animationStr2, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 2);
			sprintf(animationStr3, "Images/Hero/%s/Attack/Horizontal_%d.png", name, 3);
			setFlippedX(true);
		}
	}
	else
	{
		if (dis.y > 0)
		{
			sprintf(animationStr1, "Images/Hero/%s/Attack/Up_%d.png", name, 1);
			sprintf(animationStr2, "Images/Hero/%s/Attack/Up_%d.png", name, 2);
			sprintf(animationStr3, "Images/Hero/%s/Attack/Up_%d.png", name, 1);
		}
		else
		{
			sprintf(animationStr1, "Images/Hero/%s/Attack/Down_%d.png", name, 1);
			sprintf(animationStr2, "Images/Hero/%s/Attack/Down_%d.png", name, 2);
			sprintf(animationStr3, "Images/Hero/%s/Attack/Down_%d.png", name, 1);
		}
	}
}

void Hero::attackDeley(float dt)
{
	//int inum = _attackedMonster;
	auto obj = attackedMonsterHero;
	bool magicionAttacked = true;
	bool soundon = UserDefault::getInstance()->getBoolForKey("effect_sound");

	
	if (soundon)
	{
		if (_heroType == 1)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/Effect_Sound/Attack_sound_knight.wav");
		}
		else if (_heroType == 2)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/Effect_Sound/Attack_sound.wav");
		}
		else if (_heroType == 3)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sound/Effect_Sound/Wind_effects_5.wav");
		}
	}
	for (int i = 0; i < (*_pMonster).size(); i++)
	{
		auto removeobj = (*_pMonster).at(i);
		if (obj == (*_pMonster).at(i))
		{
			if (_heroType == 2 && obj->_fly)
			{
				obj->hp -= (_attackPower * (1.20 + ((_level - 1) * 0.05)));
			}
			else if (_heroType == 3)
			{
				obj->hp -= _attackPower;
				Vec2 monsterVec2 = obj->getPosition();
				Vec2 dis = monsterVec2 - getPosition();
				Vec2 conSize = Vec2((getContentSize().width / 2), (getContentSize().height / 2));
				//auto draw_node = DrawNode::create();
				//draw_node->setPosition((dis / 1.5)+ conSize);
				//draw_node->setScale(0.66);
				//addChild(draw_node);
				//draw_node->drawCircle(Vec2(0, 0), 60, CC_DEGREES_TO_RADIANS(90), 50, false, Color4F(0, 1, 1, 1));
				//draw_node->runAction(Sequence::create(FadeIn::create(1.0f), RemoveSelf::create(), nullptr));
				auto cache = SpriteFrameCache::getInstance();
				cache->addSpriteFramesWithFile("Images/Hero/spell/fx_f3_fountainofyouth.plist");

				Vector<SpriteFrame*> animFrames;

				char str[100] = { 0 };
				for (int i = 5; i < 11; i++)
				{
					if (i < 10)
					{
						sprintf(str, "fx_f3_fountainofyouth_00%d.png", i);
					}
					else
					{
						sprintf(str, "fx_f3_fountainofyouth_0%d.png", i);
					}
					SpriteFrame* frame = cache->getSpriteFrameByName(str);
					animFrames.pushBack(frame);
				}
				/////////////
				auto pMan = Sprite::createWithSpriteFrameName("fx_f3_fountainofyouth_000.png");
				pMan->setScale(1.5f);
				pMan->setPosition((dis / 1.5) + conSize);
				pMan->setAnchorPoint(Vec2(0.5, 0.5));
				this->addChild(pMan);
				
				auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
				auto animate = Animate::create(animation);
				auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
				pMan->runAction(seq);

				for (int j = 0; j != (*_pMonster).size(); j++)
				{
					auto monsterObj = (Monster*)(*_pMonster).at(j);
					Vec2 monsterVec2 = monsterObj->getPosition();
					Vec2 dis = splashVec - monsterVec2;

					Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));
					if (absDis.x <= 60 && absDis.y <= 60)
					{
						monsterObj->hp -= (_attackPower * (0.50 + ((_level - 1) * 0.05)));
						if (monsterObj->hp <= 0)
						{
							_masicMonster.pushBack(monsterObj);
						}
						if (monsterObj->boss == false)
						{
							monsterObj->speed->setSpeed(0.5f);
							monsterObj->setColor(Color3B::BLUE);
							monsterObj->speedDown = true;
						}
					}
				}
			}
			else
			{
				obj->hp -= _attackPower;
			}

			
			magicionAttacked = false;
		}
		

		/*if (removeobj->hp <= 0)
		{
			(*nowStageGold) = (*nowStageGold) + removeobj->dropGold;
			(*nowMasicGauge) = (*nowMasicGauge) + 5;
			_masicMonster.pushBack(removeobj);
		}*/
	}

	if (magicionAttacked)
	{
		if (_heroType == 3)
		{
			log("마법사 공격 대상X");
			Vec2 dis = splashVec - getPosition();
			Vec2 conSize = Vec2((getContentSize().width / 2), (getContentSize().height / 2));

			auto cache = SpriteFrameCache::getInstance();
			cache->addSpriteFramesWithFile("Images/Hero/spell/fx_f3_fountainofyouth.plist");
			Vector<SpriteFrame*> animFrames;

			char str[100] = { 0 };
			for (int i = 5; i < 11; i++)
			{
				if (i < 10)
				{
					sprintf(str, "fx_f3_fountainofyouth_00%d.png", i);
				}
				else
				{
					sprintf(str, "fx_f3_fountainofyouth_0%d.png", i);
				}
				SpriteFrame* frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			/////////////
			auto pMan = Sprite::createWithSpriteFrameName("fx_f3_fountainofyouth_000.png");
			pMan->setScale(1.5f);
			pMan->setPosition((dis / 1.5) + conSize);
			pMan->setAnchorPoint(Vec2(0.5, 0.5));
			this->addChild(pMan);

			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto seq = Sequence::create(animate, RemoveSelf::create(), nullptr);
			pMan->runAction(seq);

			for (int j = 0; j != (*_pMonster).size(); j++)
			{
				auto monsterObj = (Monster*)(*_pMonster).at(j);
				Vec2 monsterVec2 = monsterObj->getPosition();
				Vec2 dis = splashVec - monsterVec2;

				Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));
				if (absDis.x <= 60 && absDis.y <= 60)
				{
					monsterObj->hp -= (_attackPower * (0.50 + ((_level - 1) * 0.05)));
					if (monsterObj->hp <= 0)
					{
						_masicMonster.pushBack(monsterObj);
					}
					if (monsterObj->boss == false)
					{
						monsterObj->speed->setSpeed(0.5f);
						monsterObj->setColor(Color3B::BLUE);
						monsterObj->speedDown = true;
					}
				}
			}
		}
	}
	for (int i = 0; i < (*_pMonster).size(); i++)
	{
		auto removeobj = (*_pMonster).at(i);
		if (removeobj->hp <= 0)
		{
			(*nowStageGold) = (*nowStageGold) + removeobj->dropGold;
			(*nowMasicGauge) = (*nowMasicGauge) + 5;
			_masicMonster.pushBack(removeobj);
		}
	}
	for (int i = 0; i != _masicMonster.size(); i++)
	{
		(*_pMonster).eraseObject(_masicMonster.at(i));
	}
	_masicMonster.clear();
}