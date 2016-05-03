#include "tower.h"
#include "GameStageScene.h"

USING_NS_CC;

Tower::Tower(int typenum)
	: _listenter(nullptr)
	, _useNodePriority(false)
	, _towerType(typenum)
	, _attackDelay(1.0f)
	, _attackPower(10.0f)
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
		_attackPower = 30.0f;
		setTexture("Images/Tower/Knight1/Horizontal_3.png");
	}
	else if (_towerType == 2)
	{
		_attackDelay = 1.0f;
		_attackPower = 20.0f;
		setTexture("Images/Tower/Rogue1/Horizontal_3.png");	//Rogue 도적.
	}
	else if (_towerType == 3)
	{
		_attackDelay = 2.0f;
		_attackPower = 30.0f;
		setTexture("Images/Tower/Magician1/Horizontal_3.png");
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

void Tower::onEnter()
{
	Sprite::onEnter();
	setTowerSetting();
	
	towerContentSize = this->getContentSize();

	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);

	b_Yes = Sprite::create("Images/Button/b_Yes.png");
	b_Yes->setPosition(Vec2(towerContentSize.width / 2, towerContentSize.height));
	b_Yes->setAnchorPoint(Vec2(0, 0));
	addChild(b_Yes, 50);

	b_No = Sprite::create("Images/Button/b_No.png");
	b_No->setPosition(Vec2(towerContentSize.width / 2, towerContentSize.height));
	b_No->setAnchorPoint(Vec2(1, 0));
	addChild(b_No, 50);

	towerMenuVisible = true;


	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		log("TowerBegan");
		Vec2 LocationInNode = this->convertToNodeSpace(touch->getLocation());

			bool b_YesTouch = b_Yes->getBoundingBox().containsPoint(LocationInNode);
			if (b_YesTouch && towerMenuVisible)
			{
				//log("Yes!");
				towerSetup = true;
				towerMenuVisible = false;
				b_No->setVisible(false);
				b_Yes->setVisible(false);
				this->setOpacity(255.f);
				schedule(schedule_selector(Tower::towerTick), _attackDelay);
				return true;
			}

			bool b_NoTouch = b_No->getBoundingBox().containsPoint(LocationInNode);
			if (b_NoTouch && towerMenuVisible)
			{
				//log("No..");
				towerSetup = false;
				towerMenuVisible = false;
				removeFromParent();
			}

		Rect rect = Rect(0, 0, towerContentSize.width, towerContentSize.height);
		
		if (rect.containsPoint(LocationInNode))
		{
			//log("%d번 타워", _towerType);
			//_attackPower *= 1.2;
			
			return true;
		}

		return true;
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		
	};

	//_eventDispatcher->addEventListenerWithFixedPriority(listener, 30);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
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
				}

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.1f);

				animation->addSpriteFrameWithFile("Images/Tower/Knight1/Horizontal_1.png");
				animation->addSpriteFrameWithFile("Images/Tower/Knight1/Horizontal_2.png");
				animation->addSpriteFrameWithFile("Images/Tower/Knight1/Horizontal_3.png");
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
				}

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.1f);

				animation->addSpriteFrameWithFile("Images/Tower/Rogue1/Horizontal_1.png");
				animation->addSpriteFrameWithFile("Images/Tower/Rogue1/Horizontal_2.png");
				animation->addSpriteFrameWithFile("Images/Tower/Rogue1/Horizontal_3.png");
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
				}

				auto animation = Animation::create();
				animation->setDelayPerUnit(0.2f);

				animation->addSpriteFrameWithFile("Images/Tower/Magician1/Horizontal_1.png");
				animation->addSpriteFrameWithFile("Images/Tower/Magician1/Horizontal_2.png");
				animation->addSpriteFrameWithFile("Images/Tower/Magician1/Horizontal_3.png");
				auto animate = Animate::create(animation);

				runAction(animate);
				return;
			}
		}
	}

}