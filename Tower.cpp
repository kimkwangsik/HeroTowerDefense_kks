#include "tower.h"
#include "GameStageScene.h"

USING_NS_CC;

Tower::Tower(int typenum)
	: _listenter(nullptr)
	, _useNodePriority(false)
	, _towerType(typenum)
	, _attackDelay(1.0f)
	, _attackPower(10.0f)
	, cost(0)
{
	bool b0k = initWithTexture(nullptr, Rect::ZERO);
	if (b0k)
	{
		this->autorelease();
	}
}


void Tower::setTowerSetting()
{
	if (_towerType == 1)
	{
		_attackDelay = 0.5f;
		_attackPower = 25.0f;
		cost = 10;
		setTexture("Images/Tower/Knight1/Horizontal_3.png");
		sprintf(name, "Knight");
	}
	else if (_towerType == 2)
	{
		_attackDelay = 1.0f;
		_attackPower = 20.0f;
		cost = 20;
		setTexture("Images/Tower/Rogue1/Horizontal_3.png");	//Rogue µµÀû.
		sprintf(name, "Rogue");
	}
	else if (_towerType == 3)
	{
		_attackDelay = 2.0f;
		_attackPower = 45.0f;
		cost = 30;
		setTexture("Images/Tower/Magician1/Horizontal_3.png");
		sprintf(name, "Magician");
	}

	setAnchorPoint(Vec2(0.5, 0.3));
}
void Tower::setPriorityWithThis(bool useNodePriority)
{
	_useNodePriority = useNodePriority;
}

void Tower::setpMonster(Vector<Monster*> *_repMonster)
{
	_pMonster = _repMonster;
}

void Tower::setpGold(int * _pnowStageGold)
{
	nowStageGold = _pnowStageGold;
}

void Tower::onEnter()
{
	Sprite::onEnter();
	setTowerSetting();
	
	towerContentSize = this->getContentSize();
	towerUpgradeLevel = 1;

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	char levelViewstr[10];
	sprintf(levelViewstr, "%d", towerUpgradeLevel);

	levelView = Label::create(levelViewstr,"Arial",20);
	levelView->setPosition(Vec2(towerContentSize.width / 2, towerContentSize.height / 2));
	addChild(levelView, 50);

	b_Yes = Sprite::create("Images/Button/b_Yes.png");
	b_Yes->setPosition(Vec2(towerContentSize.width / 2, towerContentSize.height));
	b_Yes->setAnchorPoint(Vec2(0, 0));
	addChild(b_Yes, 50);

	b_No = Sprite::create("Images/Button/b_No.png");
	b_No->setPosition(Vec2(towerContentSize.width / 2, towerContentSize.height));
	b_No->setAnchorPoint(Vec2(1, 0));
	addChild(b_No, 50);

	towerMenuVisible = true;

	towerUpgradeVisible = false;

	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		Vec2 LocationInNode = this->convertToNodeSpace(touch->getLocation());

		bool b_YesTouch = b_Yes->getBoundingBox().containsPoint(LocationInNode);
		if (b_YesTouch && towerMenuVisible)
		{
			towerSetup = true;
			towerMenuVisible = false;
			b_No->setVisible(false);
			b_Yes->setVisible(false);
			this->setOpacity(255.f);
			schedule(schedule_selector(Tower::towerTick), _attackDelay);
			(*nowStageGold) = (*nowStageGold) - cost;
			_eventDispatcher->removeEventListener(_listenter);
			return true;
		}

		bool b_NoTouch = b_No->getBoundingBox().containsPoint(LocationInNode);
		if (b_NoTouch && towerMenuVisible)
		{
			towerSetup = false;
			towerMenuVisible = false;
			removeFromParent();
		}


		return false;
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		
	};

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 30);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_listenter = listener;
};

void Tower::onExit() {
	_eventDispatcher->removeEventListener(_listenter);
	Sprite::onExit();
}

void Tower::towerTick(float a)
{
	Vec2 myPos = this->getPosition();
	for (int i = 0; i < (*_pMonster).size(); i++)
	{
		auto obj = (Monster*)(*_pMonster).at(i);
		Vec2 objVec2 = obj->getPosition();
		Vec2 dis = myPos - objVec2;
		
		Vec2 absdis = Vec2(fabs(dis.x), fabs(dis.y));
		if (_towerType == 1)
		{
			if (absdis.x <= 45 && absdis.y <= 45)
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
				animation->setDelayPerUnit(0.1f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}
		else if (_towerType == 2) {
			if (absdis.x <= 75 && absdis.y <= 75)
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
				animation->setDelayPerUnit(0.1f);

				animation->addSpriteFrameWithFile(animationStr1);
				animation->addSpriteFrameWithFile(animationStr2);
				animation->addSpriteFrameWithFile(animationStr3);
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}
		else if (_towerType == 3) {
			if (absdis.x <= 75 && absdis.y <= 75)
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
		}
	}

}