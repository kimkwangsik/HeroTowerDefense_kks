#include "HeroStatu.h"
#include "Menus.h"

USING_NS_CC;

Scene* HeroStatu::createScene()
{
	auto scene = Scene::create();
	auto layer = HeroStatu::create();
	scene->addChild(layer);

	return scene;
}

bool HeroStatu::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	clickHeroNum = 0;

	auto pScene = Menus::createScene();
	auto scenestr = new Menus("HeroStatu");
	scenestr->autorelease();
	pScene->addChild(scenestr);
	this->addChild(pScene, 10);


	VisibleWinSize = Director::getInstance()->getVisibleSize();
	
	auto heroMenuSprite = Sprite::create("Images/g43961.png");
	heroMenuSprite->setPosition(Vec2(VisibleWinSize.width, VisibleWinSize.height / 2));
	heroMenuSprite->setAnchorPoint(Vec2(1, 0.5));
	addChild(heroMenuSprite);

	pHeroItem1 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroItem1->setPosition(Vec2(10, heroMenuSprite->getContentSize().height - 10));
	pHeroItem1->setAnchorPoint(Vec2(0, 1));

	pHeroItem2 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroItem2->setPosition(Vec2(pHeroItem1->getContentSize().width + 10, heroMenuSprite->getContentSize().height - 10));
	pHeroItem2->setAnchorPoint(Vec2(0, 1));

	pHeroItem1->setTag(411);
	pHeroItem2->setTag(412);

	bool hero1 = UserDefault::getInstance()->getBoolForKey("Hero1");
	bool hero2 = UserDefault::getInstance()->getBoolForKey("Hero2");
	bool hero3 = UserDefault::getInstance()->getBoolForKey("Hero3");

	if (!hero1)
	{
		pHeroItem1->setOpacity(100.f);
	}
	if (!hero2)
	{
		pHeroItem2->setOpacity(100.f);
	}
	if (!hero3)
	{

	}

	auto pHeroMenu = Menu::create(pHeroItem1, pHeroItem2, NULL);
	pHeroMenu->setPosition(Vec2::ZERO);
	heroMenuSprite->addChild(pHeroMenu);



	auto heroSprite = Sprite::create("Images/white.png");
	heroSprite->setPosition(Vec2(0, VisibleWinSize.height / 2));
	heroSprite->setAnchorPoint(Vec2(0, 0.5));
	addChild(heroSprite);



	pHero1 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHero1->setPosition(Vec2(heroSprite->getContentSize().width / 3, 10));
	pHero1->setAnchorPoint(Vec2(0.5, 0));

	pHero2 = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHero2->setPosition(Vec2(heroSprite->getContentSize().width / 3 * 2, 10));
	pHero2->setAnchorPoint(Vec2(0.5, 0));

	upgrade = LabelTTF::create("강화", "Arial", 15);
	upgrade->setPosition(Vec2(pHero2->getContentSize().width / 2,
		pHero2->getContentSize().height / 2));
	upgrade->setColor(Color3B::WHITE);
	pHero2->addChild(upgrade, 2);

	pHero1->setTag(421);
	pHero2->setTag(422);

	

	auto pHero = Menu::create(pHero1, pHero2, NULL);
	pHero->setPosition(Vec2::ZERO);
	heroSprite->addChild(pHero);

	mainSprite = Sprite::create("Images/Hero/Paladin/Walking/Down_1.png");
	mainSprite->setPosition(Vec2(heroSprite->getContentSize().width / 2, heroSprite->getContentSize().height));
	mainSprite->setAnchorPoint(Vec2(0.5, 1));
	mainSprite->setScale(2.5f);
	mainSprite->setVisible(false);
	heroInfoOn = false;
	heroSprite->addChild(mainSprite);

	infoLayer = LayerColor::create(Color4B(100, 100, 100, 100));
	infoLayer->setContentSize(Size(200, 150));
	infoLayer->setPosition(Vec2(25, pHero1->getPositionY() + pHero1->getContentSize().height));
	infoLayer->setVisible(false);
	heroSprite->addChild(infoLayer, 300);

	infoLabel = LabelTTF::create("", "Arial", 15);
	infoLabel->setPosition(Vec2(infoLayer->getContentSize().width / 2,
		infoLayer->getContentSize().height / 2));
	infoLabel->setColor(Color3B::WHITE);
	infoLayer->addChild(infoLabel, 2);
	
	
	return true;
}

void HeroStatu::doClick(Ref * pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	int gold = UserDefault::getInstance()->getIntegerForKey("have_gold");
	bool hero1 = UserDefault::getInstance()->getBoolForKey("Hero1");
	bool hero2 = UserDefault::getInstance()->getBoolForKey("Hero2");
	bool hero3 = UserDefault::getInstance()->getBoolForKey("Hero3");
	if (i == 411)
	{
		mainSprite->setTexture("Images/Hero/Paladin/Walking/Down_1.png");
		mainSprite->setVisible(true);
		infoLayer->setVisible(false);
		heroInfoOn = true;
		clickHeroNum = 1;
		if (hero1)
		{
			upgrade->setString("강화");
		}
		else
		{
			upgrade->setString("소환");
		}
	}
	else if (i == 412)
	{
		mainSprite->setTexture("Images/Hero/Rogue/Walking/Down_1.png");
		mainSprite->setVisible(true);
		infoLayer->setVisible(false);
		heroInfoOn = true;
		clickHeroNum = 2;
		if (hero2)
		{
			upgrade->setString("강화");
		}
		else
		{
			upgrade->setString("소환");
		}
	}
	else if (i == 421)
	{
		log("정보");
		if (clickHeroNum == 1)
		{
			if (hero1)
			{
				resetHero1Info();
			}
			else
			{
				infoLabel->setString("Level 1\n공격력 25\n스킬\n배쉬 5 % \n공격 대상을\n기절 시킴");
			}
		}
		else if(clickHeroNum == 2)
		{
			if (hero2)
			{
				resetHero2Info();
			}
			else
			{
				infoLabel->setString("Level 1\n공격력 20\n스킬\n대공 전문 1.05 배\n비행 몬스터에게\n데미지 증가");
			}
		}

		if (heroInfoOn)
		{
			mainSprite->setVisible(false);
			infoLayer->setVisible(true);
			heroInfoOn = false;
		}
		else
		{
			mainSprite->setVisible(true);
			infoLayer->setVisible(false);
			heroInfoOn = true;
		}
	}
	else if (i == 422)
	{
		if (clickHeroNum == 1)
		{
			if (hero1)
			{
				if (gold >= 10)
				{
					int level = UserDefault::getInstance()->getIntegerForKey("Hero1_Level");
					level++;
					gold = gold - 10;
					UserDefault::getInstance()->setIntegerForKey("Hero1_Level", level);

					resetHero1Info();
				}
				else
				{
					log("강화 금액 부족");
				}
			}
			else if (gold >= 100)
			{
				int level = UserDefault::getInstance()->getIntegerForKey("Hero1_Level");
				level++;
				UserDefault::getInstance()->setIntegerForKey("Hero1_Level", level);
				gold = gold - 100;
				UserDefault::getInstance()->setBoolForKey("Hero1", true);
				pHeroItem1->setOpacity(255.f);
				upgrade->setString("강화");

				resetHero1Info();
			}
			else
			{
				log("소환 금액 부족");
			}
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);
		}
		else if (clickHeroNum == 2)
		{
			if (hero2)
			{
				if (gold >= 20)
				{
					int level = UserDefault::getInstance()->getIntegerForKey("Hero2_Level");
					level++;
					gold = gold - 20;
					UserDefault::getInstance()->setIntegerForKey("Hero2_Level", level);

					resetHero2Info();
				}
				else
				{
					log("강화 금액 부족");
				}
			}
			else if (gold >= 200)
			{
				int level = UserDefault::getInstance()->getIntegerForKey("Hero2_Level");
				level++;
				UserDefault::getInstance()->setIntegerForKey("Hero2_Level", level);
				gold = gold - 200;
				UserDefault::getInstance()->setBoolForKey("Hero2", true);
				pHeroItem2->setOpacity(255.f);
				upgrade->setString("강화");

				resetHero2Info();
			}
			else
			{
				log("소환 금액 부족");
			}
			UserDefault::getInstance()->setIntegerForKey("have_gold", gold);
		}
	}
}

void HeroStatu::resetHero1Info()
{
	int level = UserDefault::getInstance()->getIntegerForKey("Hero1_Level");
	int attackPoint = 25 + ((level - 1) * 2);
	int skillPoint = 5 + ((level - 1) * 1);
	char heroInfostr[100];
	sprintf(heroInfostr, "Level %d\n공격력 %d(+2)\n스킬\n배쉬 %d 퍼 (+1 퍼 )\n공격 대상을\n기절 시킴", level, attackPoint, skillPoint);
	infoLabel->setString(heroInfostr);
}

void HeroStatu::resetHero2Info()
{
	int level = UserDefault::getInstance()->getIntegerForKey("Hero2_Level");
	int attackPoint = 20 + ((level - 1) * 3);
	float skillPoint = 1.05 + ((level - 1) * 0.01);
	char heroInfostr[100];
	sprintf(heroInfostr, "Level %d\n공격력 %d(+3)\n스킬\n대공 전문 %1.2f 배 (+0.01 배 )\n비행 몬스터에게\n데미지 증가", level, attackPoint, skillPoint);
	infoLabel->setString(heroInfostr);
}