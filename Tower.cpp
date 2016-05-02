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

	//towerSetup(typenum);
}


void Tower::setAttackDelay()
{
	if (_towerType == 1)
	{
		_attackDelay = 0.5f;
	}
	else if (_towerType == 2)
	{
		_attackDelay = 1.0f;
	}
	else if (_towerType == 3)
	{
		_attackDelay = 2.0f;
	}
}

void Tower::setAttackPower()
{
	if (_towerType == 1)
	{
		_attackPower = 30.0f;
	}
	else if (_towerType == 2)
	{
		_attackPower = 20.0f;
	}
	else if (_towerType == 3)
	{
		_attackPower = 30.0f;
	}
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
	setAttackDelay();
	setAttackPower();

	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);

	b_Yes = Sprite::create("Images/Button/scaled-at-25/b_Yes.png");
	b_Yes->setPosition(Vec2(30, 30));
	b_Yes->setAnchorPoint(Vec2(1, 0));
	addChild(b_Yes, 50);

	b_No = Sprite::create("Images/Button/scaled-at-25/b_No.png");
	b_No->setPosition(Vec2(0, 30));
	b_No->setAnchorPoint(Vec2(0, 0));
	addChild(b_No, 50);

	towerMenuVisible = true;

	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		Vec2 LocationInNode = this->convertToNodeSpace(touch->getLocation());

			bool b_YesTouch = b_Yes->getBoundingBox().containsPoint(LocationInNode);
			if (b_YesTouch && towerMenuVisible)
			{
				log("Yes!");
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
				towerSetup = false;
				towerMenuVisible = false;
				removeFromParent();
				log("No..");
			}

		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		
		if (rect.containsPoint(LocationInNode))
		{
			log("%d번 타워", _towerType);
			//_attackPower *= 1.2;

			/*auto abc = Sprite::create("Images/bar_base.png");
			abc->setPosition(Vec2(0, -20));
			abc->setAnchorPoint(Vec2(0.5, 0));
			addChild(abc, 50);*/

			return true;
		}

		return true;
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
	
	};

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 30);
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
				runAction(Sequence::create(
					FadeOut::create(0.1f), FadeIn::create(0.1f),
					nullptr));
				
				log("검사는 %f 의 피해를 입혔다!", _attackPower);
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
				runAction(Sequence::create(
					FadeOut::create(0.2f), FadeIn::create(0.2f),
					nullptr));
				log("궁수는 %f 의 피해를 입혔다!", _attackPower);
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
				runAction(Sequence::create(
					FadeOut::create(0.3f), FadeIn::create(0.3f),
					nullptr));
				log("마법사는 %f 의 피해를 입혔다!", _attackPower);
				return;
			}
		}
	}

}