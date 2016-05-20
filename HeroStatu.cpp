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
		"Images/Hero/hero1.png",
		"Images/Hero/hero1.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroItem1->setPosition(Vec2(10, heroMenuSprite->getContentSize().height - 10));
	pHeroItem1->setAnchorPoint(Vec2(0, 1));

	pHeroItem2 = MenuItemImage::create(
		"Images/Hero/hero2.png",
		"Images/Hero/hero2.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroItem2->setPosition(Vec2(pHeroItem1->getContentSize().width + 10, heroMenuSprite->getContentSize().height - 10));
	pHeroItem2->setAnchorPoint(Vec2(0, 1));

	pHeroItem3 = MenuItemImage::create(
		"Images/Hero/hero3.png",
		"Images/Hero/hero3.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroItem3->setPosition(Vec2(pHeroItem2->getPositionX() + pHeroItem2->getContentSize().width + 10, heroMenuSprite->getContentSize().height - 10));
	pHeroItem3->setAnchorPoint(Vec2(0, 1));

	pHeroItem1->setTag(411);
	pHeroItem2->setTag(412);
	pHeroItem3->setTag(413);

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
		pHeroItem3->setOpacity(100.f);
	}

	auto pHeroMenu = Menu::create(pHeroItem1, pHeroItem2, pHeroItem3, NULL);
	pHeroMenu->setPosition(Vec2::ZERO);
	heroMenuSprite->addChild(pHeroMenu);



	auto heroSprite = Sprite::create("Images/white.png");
	heroSprite->setPosition(Vec2(0, VisibleWinSize.height / 2));
	heroSprite->setAnchorPoint(Vec2(0, 0.5));
	addChild(heroSprite);

	

	pHeroInfoView = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroInfoView->setPosition(Vec2(heroSprite->getContentSize().width / 3, 10));
	pHeroInfoView->setAnchorPoint(Vec2(0.5, 0));

	auto infoView = LabelTTF::create("상세\n정보", "Arial", 15);
	infoView->setPosition(Vec2(pHeroInfoView->getContentSize().width / 2,
		pHeroInfoView->getContentSize().height / 2));
	infoView->setColor(Color3B::WHITE);
	pHeroInfoView->addChild(infoView, 2);

	pHeroUpgrade = MenuItemImage::create(
		"Images/GameEnd/Game_btn_on.png",
		"Images/GameEnd/Game_btn_down.png",
		CC_CALLBACK_1(HeroStatu::doClick, this));
	pHeroUpgrade->setPosition(Vec2(heroSprite->getContentSize().width / 3 * 2, 10));
	pHeroUpgrade->setAnchorPoint(Vec2(0.5, 0));

	upgrade = LabelTTF::create("강화", "Arial", 15);
	upgrade->setPosition(Vec2(pHeroUpgrade->getContentSize().width / 2,
		pHeroUpgrade->getContentSize().height / 2));
	upgrade->setColor(Color3B::WHITE);
	pHeroUpgrade->addChild(upgrade, 2);

	pHeroInfoView->setTag(421);
	pHeroUpgrade->setTag(422);

	

	auto pHero = Menu::create(pHeroInfoView, pHeroUpgrade, NULL);
	pHero->setPosition(Vec2::ZERO);
	heroSprite->addChild(pHero);

	mainSprite = Sprite::create("Images/Hero/Paladin/Walking/Down_1.png");
	mainSprite->setPosition(Vec2(heroSprite->getContentSize().width / 2, heroSprite->getContentSize().height));
	mainSprite->setAnchorPoint(Vec2(0.5, 1));
	mainSprite->setScale(2.5f);
	mainSprite->setVisible(false);
	heroInfoOn = false;
	heroSprite->addChild(mainSprite);

	spriteLevelView = LabelTTF::create("", "Arial", 10);
	spriteLevelView->setPosition(Vec2(mainSprite->getContentSize().width / 2, 0));
	spriteLevelView->setAnchorPoint(Vec2(0.5, 1));
	spriteLevelView->setColor(Color3B::BLACK);
	mainSprite->addChild(spriteLevelView, 2);

	infoLayer = LayerColor::create(Color4B(100, 100, 100, 100));
	infoLayer->setContentSize(Size(200, 150));
	infoLayer->setPosition(Vec2(25, pHeroInfoView->getPositionY() + pHeroInfoView->getContentSize().height));
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
		mainSprite->setTexture("Images/Hero/hero1.png");
		mainSprite->setVisible(true);
		infoLayer->setVisible(false);
		heroInfoOn = true;
		clickHeroNum = 1;
		if (hero1)
		{
			resetHeroInfo("Hero1", false, 10);
		}
		else
		{
			resetHeroInfo("Hero1", false, 100);
			spriteLevelView->setString("");
		}
	}
	else if (i == 412)
	{
		mainSprite->setTexture("Images/Hero/hero2.png");
		mainSprite->setVisible(true);
		infoLayer->setVisible(false);
		heroInfoOn = true;
		clickHeroNum = 2;
		if (hero2)
		{
			resetHeroInfo("Hero2", false, 20);
		}
		else
		{
			resetHeroInfo("Hero2", false, 200);
			spriteLevelView->setString("");
		}
	}
	else if (i == 413)
	{
		mainSprite->setTexture("Images/Hero/hero3.png");
		mainSprite->setVisible(true);
		infoLayer->setVisible(false);
		heroInfoOn = true;
		clickHeroNum = 3;
		if (hero3)
		{
			resetHeroInfo("Hero3", false, 30);
		}
		else
		{
			resetHeroInfo("Hero3", false, 300);
			spriteLevelView->setString("");
		}
	}
	else if (i == 421)
	{
		log("정보");
		if (clickHeroNum == 1)
		{
			if (hero1)
			{
				resetHeroInfo("Hero1", false, 10);
			}
			else
			{
				infoLabel->setString("Level 1\n공격력 25\n스킬\n배쉬\n5 % \n공격 대상을\n기절 시킴");
			}
		}
		else if (clickHeroNum == 2)
		{
			if (hero2)
			{
				resetHeroInfo("Hero2", false, 20);
			}
			else
			{
				infoLabel->setString("Level 1\n공격력 20\n스킬\n대공 전문\n1.05 배\n비행 몬스터에게\n데미지 증가");
			}
		}
		else if (clickHeroNum == 3)
		{
			if (hero3)
			{
				resetHeroInfo("Hero3", false, 30);
			}
			else
			{
				infoLabel->setString("Level 1\n공격력 30\n스킬\n스플래시 데미지\n0.20 배\n주변 몬스터에게\n스플래시 데미지");
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
					resetHeroInfo("Hero1", true, 10);
				}
				else
				{
					log("강화 금액 부족");
				}
			}
			else if (gold >= 100)
			{
				resetHeroInfo("Hero1", true, 100);
				resetHeroInfo("Hero1", false, 10);
			}
			else
			{
				log("소환 금액 부족");
			}
			
		}
		else if (clickHeroNum == 2)
		{
			if (hero2)
			{
				if (gold >= 20)
				{
					resetHeroInfo("Hero2", true, 20);
				}
				else
				{
					log("강화 금액 부족");
				}
			}
			else if (gold >= 200)
			{
				resetHeroInfo("Hero2", true, 200);
				resetHeroInfo("Hero2", false, 20);
			}
			else
			{
				log("소환 금액 부족");
			}
		}
		else if (clickHeroNum == 3)
		{
			if (hero3)
			{
				if (gold >= 30)
				{
					resetHeroInfo("Hero3", true, 30);
				}
				else
				{
					log("강화 금액 부족");
				}
			}
			else if (gold >= 300)
			{
				resetHeroInfo("Hero3", true, 300);
				resetHeroInfo("Hero3", false, 30);
			}
			else
			{
				log("소환 금액 부족");
			}
		}
	}
}

void HeroStatu::resetHeroInfo(std::string name, bool upgradeinfo, int cost)
{
	//const char * namec = name.c_str();
	char namestr[20];
	if (name == "Hero1")
	{
		sprintf(namestr, "Hero1");
	}
	else if (name == "Hero2")
	{
		sprintf(namestr, "Hero2");
	}
	else if (name == "Hero3")
	{
		sprintf(namestr, "Hero3");
	}


	char levelstr[20];
	sprintf(levelstr, "%s_Level", namestr);
	bool hero = UserDefault::getInstance()->getBoolForKey(namestr);

	int have_gold = UserDefault::getInstance()->getIntegerForKey("have_gold");

	int level = UserDefault::getInstance()->getIntegerForKey(levelstr);
	if (upgradeinfo)
	{
		level++;
		have_gold = have_gold - cost;
		UserDefault::getInstance()->setIntegerForKey(levelstr, level);
		if (!hero)
		{
			UserDefault::getInstance()->setBoolForKey(namestr, true);
		}
	}


	hero = UserDefault::getInstance()->getBoolForKey(namestr);

	if (!hero)
	{
		char coststr[20];
		sprintf(coststr, "소환\n%d", cost);
		upgrade->setString(coststr);
	}
	else
	{
		char coststr[20];
		sprintf(coststr, "강화\n%d", cost);
		upgrade->setString(coststr);
	}

	char heroInfostr[200];
	if (name == "Hero1")
	{
		int attackPoint = 25 + ((level - 1) * 2);
		int skillPoint = 5 + ((level - 1) * 1);
		sprintf(heroInfostr, "Level %d\n공격력 %d(+2)\n스킬\n배쉬\n%d 퍼 (+1 퍼 )\n공격 대상을\n기절 시킴", level, attackPoint, skillPoint);
		if (hero)
		{
			pHeroItem1->setOpacity(255.f);
			char levelLabelstr[20];
			sprintf(levelLabelstr, "Level %d", level);
			spriteLevelView->setString(levelLabelstr);
		}
	}
	else if (name == "Hero2")
	{
		int attackPoint = 20 + ((level - 1) * 3);
		float skillPoint = 1.05 + ((level - 1) * 0.01);
		sprintf(heroInfostr, "Level %d\n공격력 %d(+3)\n스킬\n대공 전문\n%1.2f 배 (+0.01 배 )\n비행 몬스터에게\n데미지 증가", level, attackPoint, skillPoint);
		if (hero)
		{
			pHeroItem2->setOpacity(255.f);
			char levelLabelstr[20];
			sprintf(levelLabelstr, "Level %d", level);
			spriteLevelView->setString(levelLabelstr);
		}
	}
	else if (name == "Hero3")
	{
		int attackPoint = 30 + ((level - 1) * 5);
		float skillPoint = 0.20 + ((level - 1) * 0.02);
		sprintf(heroInfostr, "Level %d\n공격력 %d(+5)\n스킬\n스플래시 데미지\n%1.2f 배 (+0.02 배 )\n주변 몬스터에게\n스플래시 데미지", level, attackPoint, skillPoint);
		if (hero)
		{
			pHeroItem3->setOpacity(255.f);
			char levelLabelstr[20];
			sprintf(levelLabelstr, "Level %d", level);
			spriteLevelView->setString(levelLabelstr);
		}
	}
	infoLabel->setString(heroInfostr);


	UserDefault::getInstance()->setIntegerForKey("have_gold", have_gold);
}