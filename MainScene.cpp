#include "MainScene.h"
#include "StageSelectScene.h"
#include "Menus.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);

    return scene;
}

bool MainScene::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    {
        return false;
    }
	
	auto pScene = Menus::createScene();
	this->addChild(pScene,1);


	VisibleWinSize = Director::getInstance()->getVisibleSize();

	auto tmap = TMXTiledMap::create("TileMaps/Main.tmx");

	tmap->setPosition(Vec2(
		(VisibleWinSize.width / 2),
		(VisibleWinSize.height / 2)
		));

	tmap->setAnchorPoint(Vec2(0.5, 0.5));

	objects = tmap->getObjectGroup("ViaPoint");

	ValueMap StageMode = objects->getObject("Stage");
	ValueMap InfiniteMode = objects->getObject("Infinite");
	ValueMap HeroMode = objects->getObject("Hero");

	int StageX = StageMode["x"].asInt();
	int StageY = StageMode["y"].asInt();

	

	/*auto fortress1 = Sprite::create("Images/Main/fortress.png");
	fortress1->setPosition(Vec2(StageX, StageY));
	fortress1->setScale(2.f);
	tmap->addChild(fortress1);*/

	int InfiniteX = InfiniteMode["x"].asInt();
	int InfiniteY = InfiniteMode["y"].asInt();

	/*auto magic_stones = Sprite::create("Images/Main/magic_stones.png");
	magic_stones->setPosition(Vec2(InfiniteX, InfiniteY));
	magic_stones->setScale(2.f);
	tmap->addChild(magic_stones);*/

	int HeroX = HeroMode["x"].asInt();
	int HeroY = HeroMode["y"].asInt();

	/*auto statue = Sprite::create("Images/Main/statue.png");
	statue->setPosition(Vec2(HeroX, HeroY));
	statue->setScale(2.f);
	tmap->addChild(statue);*/

	this->addChild(tmap, 0, 11);

	auto fortress = MenuItemImage::create(
		"Images/Main/fortress.png",
		"Images/Main/fortress.png",
		CC_CALLBACK_1(MainScene::doClick, this));
	fortress->setScale(2.f);
	fortress->setPosition(Vec2(StageX, StageY));

	auto magic_stones = MenuItemImage::create(
		"Images/Main/magic_stones.png",
		"Images/Main/magic_stones.png",
		CC_CALLBACK_1(MainScene::doClick, this));
	magic_stones->setScale(2.f);
	magic_stones->setPosition(Vec2(InfiniteX, InfiniteY));

	auto statue = MenuItemImage::create(
		"Images/Main/statue.png",
		"Images/Main/statue.png",
		CC_CALLBACK_1(MainScene::doClick, this));
	statue->setScale(2.f);
	statue->setPosition(Vec2(HeroX, HeroY));

	fortress->setTag(1);
	magic_stones->setTag(2);
	statue->setTag(3);

	auto pMenu = Menu::create(fortress, magic_stones, statue, NULL);
	pMenu->setPosition(Vec2::ZERO);
	//pMenu->alignItemsVertically();

	addChild(pMenu, 3);

	/*auto pMenuItem1 = MenuItemFont::create(
		" 스테이지 모드 ",
		CC_CALLBACK_1(MainScene::doClick, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));

	auto pMenuItem2 = MenuItemFont::create(
		" 무한모드(미구현) ",
		CC_CALLBACK_1(MainScene::doClick, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));

	auto pMenuItem3 = MenuItemFont::create(
		" 영웅 관리(미구현) ",
		CC_CALLBACK_1(MainScene::doClick, this));
	pMenuItem3->setColor(Color3B(0, 0, 0));

	pMenuItem1->setTag(1);
	pMenuItem2->setTag(2);
	pMenuItem3->setTag(3);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, NULL);

	pMenu->alignItemsVertically();

	tmap->addChild(pMenu,3);*/

    return true;
}

void MainScene::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	log("%d번째 메뉴가 선택되었습니다.", i);
	if (i == 1)
	{
		auto pScene = StageSelectScene::createScene();
		Director::getInstance()->pushScene(pScene);
	}

}


void MainScene::doNoClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	log("%d번째 메뉴가 선택되었습니다.", i);

}