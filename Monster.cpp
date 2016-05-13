#include "Monster.h"

USING_NS_CC;

Monster::Monster(std::string monsterName)
	: _fixedPriority(0)
	, _useNodePriority(false)
	, maxHp(100.0f)
	, myMonsterName(monsterName)
	, dropGold(1)
	, _fly(false)
{
	bool b0k = initWithTexture(nullptr, Rect::ZERO);
	if (b0k)
	{
		this->autorelease();
	}
}

void Monster::remove()
{
	removeFromParent();
	//onExit();
}

void Monster::onEnter()
{
	Sprite::onEnter();
	hp = maxHp;
	if (myMonsterName == "slime")
	{
		runAction(RepeatForever::create(JumpBy::create(0.5, Vec2::ZERO, 5.0f, 1)));
	}
	else if (myMonsterName == "minotaur")
	{
		auto sprite = Sprite::create("Images/Monster/minotaur.png");
		auto texture1 = sprite->getTexture();

		auto animation = Animation::create();
		animation->setDelayPerUnit(0.15f);

		for (int i = 20; i < 30; i++)
		{
			log("%d", i);
			int column = i % 10;
			int row = i / 10;

			animation->addSpriteFrameWithTexture(
				texture1,
				Rect(column * 30, row * 30, 30, 30));
		}

		auto animate = Animate::create(animation);
		auto rep = RepeatForever::create(animate);
		runAction(rep);
	}
	
	hpBar = Sprite::create("Images/bar_base.png");
	hpBar->setPosition(Vec2(getContentSize().width/2, getContentSize().height));
	hpBar->setAnchorPoint(Vec2(0.5, 0));
	hpBar->setScaleX(0.2f);
	addChild(hpBar, 2);

	hpGauge = Sprite::create("Images/bar_gauge.png");

	gauge = 100;

	gaugeBar = ProgressTimer::create(hpGauge);
	gaugeBar->setType(ProgressTimer::Type::BAR);
	gaugeBar->setMidpoint(Vec2(1, 0));
	gaugeBar->setBarChangeRate(Vec2(1, 0));
	gaugeBar->setPercentage(gauge);

	gaugeBar->setPosition(Vec2(0, 0));
	gaugeBar->setAnchorPoint(Vec2(0, 0));
	hpBar->addChild(gaugeBar, 3);

	schedule(schedule_selector(Monster::monsterTick));
};

void Monster::monsterTick(float a)
{
	gauge = (hp / maxHp)*100;
	gaugeBar->setPercentage(gauge);
	if (hp <= 0)
	{
		remove();
	}
}
void Monster::sendSpeed(Speed* resspeed)
{
	speed = resspeed;
}
void Monster::onExit() {
	
	Sprite::onExit();
};