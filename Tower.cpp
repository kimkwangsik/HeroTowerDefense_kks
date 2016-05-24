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
		_attackArea = 45.0f;
		cost = 10;
		setTexture("Images/Tower/Knight1/Horizontal_3.png");
		sprintf(name, "Knight");
	}
	else if (_towerType == 2)
	{
		_attackDelay = 1.0f;
		_attackPower = 20.0f;
		_attackArea = 75.0f;
		cost = 20;
		setTexture("Images/Tower/Rogue1/Horizontal_3.png");	//Rogue 도적.
		sprintf(name, "Rogue");
	}
	else if (_towerType == 3)
	{
		_attackDelay = 2.0f;
		_attackPower = 45.0f;
		_attackArea = 75.0f;
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

void Tower::setpMasicGauge(int * _pMasicGauge)
{
	nowMasicGauge = _pMasicGauge;
}

void Tower::onEnter()
{
	Sprite::onEnter();
	setTowerSetting();

	towerContentSize = this->getContentSize();
	towerUpgradeLevel = 1;

	sprintf(animationStr1, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 1);
	sprintf(animationStr2, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 2);
	sprintf(animationStr3, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 3);

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

	auto draw_node = DrawNode::create();
	draw_node->setPosition(Vec2(getContentSize().width / 2, getContentSize().height / 2));
	addChild(draw_node);
	draw_node->drawCircle(Vec2(0, 0), _attackArea, CC_DEGREES_TO_RADIANS(90), 50, false, Color4F(0, 1, 1, 1));

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
			draw_node->setVisible(false);
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
		Vec2 dis = objVec2 - myPos;
		
		Vec2 absDis = Vec2(fabs(dis.x), fabs(dis.y));
		if (_towerType == 1 && obj->_fly == false)
		{
			if (absDis.x <= _attackArea && absDis.y <= _attackArea)
			{

				attackedMonster = obj;
				scheduleOnce(schedule_selector(Tower::attackDeley), 0.1f);

				/*(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}*/

				setFlippedX(false);
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
		else if (_towerType == 2) {
			if (absDis.x <= _attackArea && absDis.y <= _attackArea)
			{
				attackedMonster = obj;

				auto ball = Sprite::create("Images/Tower/Green_Dagger.png");
				Vec2 shootVector = obj->getPosition() - getPosition();
				float shootAngle = shootVector.getAngle();
				float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
				ball->setRotation(cocosAngle+90);

				ball->setPosition(getContentSize().width / 2, getContentSize().height / 2);
				addChild(ball);
				ball->runAction(Sequence::create(MoveBy::create(0.2, dis), RemoveSelf::create(), nullptr));

				scheduleOnce(schedule_selector(Tower::attackDeley), 0.2f);

				/*(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}
				setFlippedX(false);
				setAnimation(absDis, dis);*/

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
			if (absDis.x <= _attackArea && absDis.y <= _attackArea)
			{
				//_attackedMonster = i;

				auto ball = Sprite::create("Images/Tower/ice_ball.png");
				ball->setPosition(getContentSize().width / 2, getContentSize().height / 2);
				addChild(ball);
				ball->runAction(Sequence::create(MoveBy::create(0.5, dis), RemoveSelf::create(), nullptr));

				attackedMonster = obj;
				scheduleOnce(schedule_selector(Tower::attackDeley), 0.5f);

				/*(*_pMonster).at(i)->hp -= _attackPower;
				if ((*_pMonster).at(i)->hp <= 0)
				{
					(*nowStageGold) = (*nowStageGold) + obj->dropGold;
					(*nowMasicGauge) = (*nowMasicGauge) + 5;
					(*_pMonster).eraseObject(obj);
				}

				if (obj->boss == false)
				{
					obj->speed->setSpeed(0.5f);
					obj->setColor(Color3B::BLUE);
					obj->speedDown = true;
				}*/

				setFlippedX(false);
				setAnimation(absDis, dis);

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

void Tower::resetSpeed(float a)
{
	//resetMonster->speed->setSpeed(1.0f);
}

//void Tower::animationRename()
//{
//	sprintf(animationStr1, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 1);
//	sprintf(animationStr2, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 2);
//	sprintf(animationStr3, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 3);
//}

void Tower::setAnimation(cocos2d::Vec2 absDis, cocos2d::Vec2 dis)
{
	if (absDis.x > absDis.y)
	{
		if (dis.x > 0)
		{
			sprintf(animationStr1, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 1);
			sprintf(animationStr2, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 2);
			sprintf(animationStr3, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 3);
		}
		else
		{
			sprintf(animationStr1, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 1);
			sprintf(animationStr2, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 2);
			sprintf(animationStr3, "Images/Tower/%s%d/Horizontal_%d.png", name, towerUpgradeLevel, 3);
			setFlippedX(true);
		}
	}
	else
	{
		if (dis.y > 0)
		{
			sprintf(animationStr1, "Images/Tower/%s%d/Up_%d.png", name, towerUpgradeLevel, 1);
			sprintf(animationStr2, "Images/Tower/%s%d/Up_%d.png", name, towerUpgradeLevel, 2);
			sprintf(animationStr3, "Images/Tower/%s%d/Up_%d.png", name, towerUpgradeLevel, 1);
		}
		else
		{
			sprintf(animationStr1, "Images/Tower/%s%d/Down_%d.png", name, towerUpgradeLevel, 1);
			sprintf(animationStr2, "Images/Tower/%s%d/Down_%d.png", name, towerUpgradeLevel, 2);
			sprintf(animationStr3, "Images/Tower/%s%d/Down_%d.png", name, towerUpgradeLevel, 1);
		}
	}
}

void Tower::attackDeley(float dt)
{
	//int inum = _attackedMonster;
	auto obj = attackedMonster;
	for (int i = 0; i < (*_pMonster).size(); i++)
	{
		if (obj == (*_pMonster).at(i))
		{
			if (_towerType == 3)
			{
				if (obj->boss == false)
				{
					obj->speed->setSpeed(0.5f);
					obj->setColor(Color3B::BLUE);
					obj->speedDown = true;
				}
			}
			if (_towerType == 2 && obj->_fly)
			{
				obj->hp -= (_attackPower * 1.5f);
			}
			else
			{
				obj->hp -= _attackPower;
			}
			
			if (obj->hp <= 0)
			{
				(*nowStageGold) = (*nowStageGold) + obj->dropGold;
				(*nowMasicGauge) = (*nowMasicGauge) + 5;
				(*_pMonster).eraseObject(obj);
			}
		}
	}	
}