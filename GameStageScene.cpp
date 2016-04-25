#include "GameStageScene.h"
#include "Menus.h"

USING_NS_CC;

Scene* GameStageScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameStageScene::create();
	scene->addChild(layer);

	return scene;
}

GameStageScene::GameStageScene(int stagelevel)
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return;
	}
	auto winSize = Director::getInstance()->getVisibleSize();

	auto pScene = Menus::createScene();
	this->addChild(pScene,2000);

	char str[20];
	sprintf(str, "TileMaps/Level%ld.tmx", stagelevel);

	auto tmap = TMXTiledMap::create(str);
	tmap->setPosition(Vec2(winSize.width/2, winSize.height / 2));
	tmap->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(tmap, 0, 11);

	Size s = tmap->getContentSize();
	log("ContentSize: %f, %f", s.width, s.height);

	//게임 스테이지 씬
	//타일맵 추가

	return;
}