#include "IntroScene.h"
#include "MainScene.h"
#include "Menus.h"

USING_NS_CC;

Scene* IntroScene::createScene()
{
	auto scene = Scene::create();
	auto layer = IntroScene::create();
	scene->addChild(layer);

	return scene;
}

bool IntroScene::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

	Size winSize = Director::getInstance()->getVisibleSize();

	auto Back_Castel = Sprite::create("Images/Back_Castel.png");
	Back_Castel->setAnchorPoint(Vec2(0, 0));

	addChild(Back_Castel, 0);

	auto pLabel = LabelTTF::create("touch to start",
		"Arial", 34, Size(300, 200), TextHAlignment::CENTER, TextVAlignment::CENTER);

	pLabel->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	pLabel->setColor(Color3B::BLACK);
	pLabel->setOpacity(255.0);
	this->addChild(pLabel);

	return true;
}

void IntroScene::onEnter() {
	Layer::onEnter();

	introListener = EventListenerTouchOneByOne::create();

	introListener->setSwallowTouches(true);

	introListener->onTouchBegan = CC_CALLBACK_2(IntroScene::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(introListener, this);
}
void IntroScene::onExit() {
	_eventDispatcher->removeEventListener(introListener);
	Layer::onExit();
}
bool IntroScene::onTouchBegan(Touch* touch, Event* event) {

	auto pScene = MainScene::createScene();
	Director::getInstance()->replaceScene(pScene);

	return true;
}