#include "SelectStageInfo.h"
#include "GameStageScene.h"

USING_NS_CC;

Scene* SelectStageInfo::createScene()
{
	auto scene = Scene::create();
	auto layer = SelectStageInfo::create();
	scene->addChild(layer);

	return scene;
}

SelectStageInfo::SelectStageInfo(int selectStage)
{
	if (!Layer::init())
	{
		return;
	}

	_selectStage = selectStage;
	winSize = Director::getInstance()->getVisibleSize();

	auto optionbar = Sprite::create("Images/SelectStageInfoBackGround.png");
	//optionbar->setScale(5.0f);
	optionbar->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	addChild(optionbar);

	auto pMenuItem1 = MenuItemFont::create(
		" Ã¢ ´Ý±â ",
		CC_CALLBACK_1(SelectStageInfo::doClick, this));
	pMenuItem1->setPosition(Vec2(optionbar->getContentSize().width / 2, 0));
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	//pMenuItem1->setScale(0.2f);

	auto pMenuItem2 = MenuItemFont::create(
		" ½ÃÀÛ ",
		CC_CALLBACK_1(SelectStageInfo::doClick, this));
	pMenuItem2->setPosition(Vec2(optionbar->getContentSize().width / 2, optionbar->getContentSize().height));
	pMenuItem2->setAnchorPoint(Vec2(0.5, 1));
	//pMenuItem2->setScale(0.2f);

	pMenuItem1->setTag(621);
	pMenuItem2->setTag(622);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, NULL);
	pMenu->setPosition(Vec2::ZERO);
	optionbar->addChild(pMenu);

	return;
}
void SelectStageInfo::onEnter() {
	Layer::onEnter();
	Director::getInstance()->pause();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(SelectStageInfo::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void SelectStageInfo::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->resume();
	Layer::onExit();
}
bool SelectStageInfo::onTouchBegan(Touch* touch, Event* event) {

	return true;
}
void SelectStageInfo::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();

	if (i == 621)
	{
		this->removeFromParentAndCleanup(true);
	}
	if (i == 622)
	{
		auto pScene = GameStageScene::createScene();
		auto stagenum = new GameStageScene(_selectStage);
		stagenum->autorelease();
		pScene->addChild(stagenum);
		Director::getInstance()->replaceScene(pScene);
	}
}