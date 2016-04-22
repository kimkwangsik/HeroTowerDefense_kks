#include "GameStageScene.h"

USING_NS_CC;

Scene* GameStageScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameStageScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameStageScene::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

	auto tmap = TMXTiledMap::create("TileMaps/Level1.tmx");
	this->addChild(tmap, 0, 11);

	Size s = tmap->getContentSize();
	log("ContentSize: %f, %f", s.width, s.height);

	//게임 스테이지 씬
	//타일맵 추가

	return true;
}