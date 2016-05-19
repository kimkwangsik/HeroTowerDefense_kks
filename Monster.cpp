#include "Monster.h"

USING_NS_CC;

Monster::Monster(std::string monsterName)
	: _fixedPriority(0)
	, _useNodePriority(false)
	, maxHp(100.0f)
	, myMonsterName(monsterName)
	, dropGold(1)
	, _fly(false)
	, boss(false)
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
	speedDown = true;

	//기본 자세와 걷는 모션
	char monsterFileName[50];
	sprintf(monsterFileName, "Images/Monster/%s_walk/%d.png", myMonsterName.c_str(), 0);
	setTexture(monsterFileName);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.15f);
	for (int i = 0; i < 10; i++)
	{
		char monsterFileName[50];
		sprintf(monsterFileName, "Images/Monster/%s_walk/%d.png", myMonsterName.c_str(), i);
		animation->addSpriteFrameWithFile(monsterFileName);
	}
	auto animate = Animate::create(animation);
	rep = RepeatForever::create(animate);
	runAction(rep);


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
	if (speed->getSpeed() == 0.5f && speedDown)
	{
		unschedule(schedule_selector(Monster::resetSpeed));
		scheduleOnce(schedule_selector(Monster::resetSpeed), 2.0f);
		speedDown = false;
	}

	if (speed->getSpeed() == 0.0f && speedDown)
	{
		unschedule(schedule_selector(Monster::resetSpeed));
		scheduleOnce(schedule_selector(Monster::resetSpeed), 2.0f);
		speedDown = false;
	}

	gauge = (hp / maxHp)*100;
	gaugeBar->setPercentage(gauge);
	if (hp <= 0)
	{
		remove();
	}
	

	//log("%f",speed->getSpeed());
}
void Monster::resetSpeed(float a)
{
	speed->setSpeed(1.0f);
	setColor(Color3B::WHITE);
	speedDown = true;
}
void Monster::sendSpeed(Speed* resspeed)
{
	speed = resspeed;
}
void Monster::onExit() {
	
	Sprite::onExit();
};