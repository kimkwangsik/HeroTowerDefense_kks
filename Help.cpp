#include "Help.h"

USING_NS_CC;

Scene* Help::createScene()
{
	auto scene = Scene::create();
	auto layer = Help::create();
	scene->addChild(layer);

	return scene;
}

bool Help::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();

	auto helpbar = Sprite::create("Images/GameEnd/End_panel.png");
	helpbar->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(helpbar);

	auto helplabel = LabelTTF::create("Ÿ�� ��ġ ���\n\n�ϴ� �߾��� ��ġ��\nŸ���� ��ġ�� ��ġ��\n��Ҹ� ��ġ���ּ���.\n������ ���� �������\n��� �����մϴ�.", "", 20);
	helplabel->setPosition(Vec2(helpbar->getContentSize().width / 2, helpbar->getContentSize().height - 10));
	helplabel->setColor(Color3B::BLACK);
	helplabel->setAnchorPoint(Vec2(0.5, 1));
	helpbar->addChild(helplabel);

	auto pMenuItem1 = MenuItemFont::create(
		" â �ݱ� ",
		CC_CALLBACK_1(Help::doClick, this));
	pMenuItem1->setPosition(Vec2(helpbar->getContentSize().width / 2, 10));
	pMenuItem1->setColor(Color3B::BLACK);
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));

	pMenuItem1->setTag(941);

	auto pMenu = Menu::create(pMenuItem1, NULL);
	pMenu->setPosition(Vec2::ZERO);
	helpbar->addChild(pMenu);

	return true;
}
void Help::onEnter() {
	Layer::onEnter();
	Director::getInstance()->pause();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Help::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void Help::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->resume();
	Layer::onExit();
}
bool Help::onTouchBegan(Touch* touch, Event* event) {

	return true;
}
void Help::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();

	if (i == 941)
	{
		this->removeFromParentAndCleanup(true);
	}
}