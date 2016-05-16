#include "InfinityInfo.h"

USING_NS_CC;

Scene* InfinityInfo::createScene()
{
	auto scene = Scene::create();
	auto layer = InfinityInfo::create();
	scene->addChild(layer);

	return scene;
}

bool InfinityInfo::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();

	auto optionbar = Sprite::create("Images/CyanSquare.png");
	optionbar->setScale(5.0f);
	optionbar->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(optionbar);

	auto pMenuItem1 = MenuItemFont::create(
		" Ã¢ ´Ý±â ",
		CC_CALLBACK_1(InfinityInfo::doClick, this));
	pMenuItem1->setPosition(Vec2(optionbar->getContentSize().width / 2, 0));
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	pMenuItem1->setScale(0.2f);

	pMenuItem1->setTag(621);

	auto pMenu = Menu::create(pMenuItem1, NULL);
	pMenu->setPosition(Vec2::ZERO);
	optionbar->addChild(pMenu);

	return true;
}
void InfinityInfo::onEnter() {
	Layer::onEnter();
	Director::getInstance()->pause();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(InfinityInfo::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void InfinityInfo::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->resume();
	Layer::onExit();
}
bool InfinityInfo::onTouchBegan(Touch* touch, Event* event) {

	return true;
}
void InfinityInfo::doClick(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}