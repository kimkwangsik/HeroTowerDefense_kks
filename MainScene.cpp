#include "MainScene.h"

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


    return true;
}