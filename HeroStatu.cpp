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

	auto pScene = Menus::createScene();
	auto scenestr = new Menus("HeroStatu");
	scenestr->autorelease();
	pScene->addChild(scenestr);
	this->addChild(pScene, 10);


	VisibleWinSize = Director::getInstance()->getVisibleSize();

	/*auto tmap = TMXTiledMap::create("TileMaps/Main.tmx");

	tmap->setPosition(Vec2(
		(VisibleWinSize.width / 2),
		(VisibleWinSize.height / 2)
		));

	tmap->setAnchorPoint(Vec2(0.5, 0.5));*/

//	this->addChild(tmap, 0, 191);

	return true;
}