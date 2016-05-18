#include "Hero.h"
#include "GameStageScene.h"

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
		_attackPower = 25.0f + level * 2;

		log("%d , %f", level, _attackPower);
		setTexture("Images/Hero/Paladin/Walking/Horizontal_1.png");
		setScale(1.5f);
		sprintf(name, "Paladin");
	}
	else if (_heroType == 2)
	{
		_attackDelay = 1.0f;
		int level = UserDefault::getInstance()->getIntegerForKey("Hero2_Level");
		_attackPower = 20.0f + level * 3;
		setTexture("Images/Tower/Rogue1/Horizontal_3.png");	//Rogue µµÀû.
		setScale(1.5f);
		sprintf(name, "Rogue");
	}
	//else if (_heroType == 3)
	//{
	//	_attackDelay = 2.0f;
	//	_attackPower = 45.0f;
	//	setTexture("Images/Tower/Magician1/Horizontal_3.png");
	//	sprintf(name, "Magician");
	//}

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
	Sprite::onEnter();
	setHeroSetting();

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
		if (absMaxDis > absDis)
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

				char animationStr1[50];
				char animationStr2[50];
				char animationStr3[50];
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
						sprintf(animationStr2, "Images/Hero/%s/Attack/Up_%d.png", name, 1);
						sprintf(animationStr3, "Images/Hero/%s/Attack/Up_%d.png", name, 2);
					}
					else
					{
						sprintf(animationStr1, "Images/Hero/%s/Attack/Down_%d.png", name, 1);
						sprintf(animationStr2, "Images/Hero/%s/Attack/Down_%d.png", name, 1);
						sprintf(animationStr3, "Images/Hero/%s/Attack/Down_%d.png", name, 2);
					}
				}

				stopAllActions();
				(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}

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
				stopAllActions();
				(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}

				char animationStr1[50];
				sprintf(animationStr1, "Images/Tower/%s1/Horizontal_%d.png", name, 1);
				char animationStr2[50];
				sprintf(animationStr2, "Images/Tower/%s1/Horizontal_%d.png", name, 2);
				char animationStr3[50];
				sprintf(animationStr3, "Images/Tower/%s1/Horizontal_%d.png", name, 3);

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.2f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}
		/*else if (_heroType == 3) {
			if (absDis.x <= 75 && absDis.y <= 75)
			{
				(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*_pMonster).eraseObject(obj);
					(*nowStageGold)++;
				}

				char animationStr1[50];
				sprintf(animationStr1, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 1);
				char animationStr2[50];
				sprintf(animationStr2, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 2);
				char animationStr3[50];
				sprintf(animationStr3, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 3);

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.2f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}*/
	}

	stopAllActions();

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

		runAction(animate);
	}
	else
	{
		char def[50];
		sprintf(def, "Images/Hero/%s/Walking/Horizontal_1.png", name);
		setTexture(def);
	}
	auto moveb = MoveBy::create(disVec/60.f, minDis);
	runAction(moveb);
}