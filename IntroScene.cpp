#include "IntroScene.h"
#include "MainScene.h"

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
	auto pLabel = LabelTTF::create("touch to start",
		"Arial", 34, Size(300, 200), TextHAlignment::CENTER, TextVAlignment::CENTER);

	pLabel->setPosition(Vec2(240, 160));
	pLabel->setColor(Color3B::BLACK);
	pLabel->setOpacity(255.0);
	this->addChild(pLabel);

	return true;
}

void IntroScene::onEnter() {
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(IntroScene::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void IntroScene::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Layer::onExit();
}
bool IntroScene::onTouchBegan(Touch* touch, Event* event) {
	auto pScene = MainScene::createScene();
	Director::getInstance()->pushScene(pScene);

	return true;
}