#include "EndPopup.h"
#include "MainScene.h"

USING_NS_CC;

Scene* EndPopup::createScene()
{
	auto scene = Scene::create();
	auto layer = EndPopup::create();
	scene->addChild(layer);

	return scene;
}

EndPopup::EndPopup(std::string SceneName)
{
	nowSceneName = SceneName;

	winSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto endmenu = Sprite::create("Images/g4396.png");
	endmenu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	endmenu->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(endmenu);

	auto endLabel = LabelTTF::create("게임을 종료\n하시겠습니까?", "Helvetica", 20.0);
	endLabel->setPosition(Vec2(endmenu->getContentSize().width/2, endmenu->getContentSize().height - 10));
	endLabel->setAnchorPoint(Vec2(0.5, 1));
	endLabel->setColor(Color3B::BLACK);
	endmenu->addChild(endLabel);

	if (nowSceneName != "MainScene")
	{
		endLabel->setString("메인 화면으로\n돌아갑니다.");
	}

	auto pMenuItem1 = MenuItemImage::create(
		"Images/Button/b_Yes.png",
		"Images/Button/b_Yes.png",
		CC_CALLBACK_1(EndPopup::doClick, this));
	pMenuItem1->setPosition(Vec2(endmenu->getContentSize().width / 2 + 20, 10));
	pMenuItem1->setAnchorPoint(Vec2(0, 0));

	auto pMenuItem2 = MenuItemImage::create(
		"Images/Button/b_No.png",
		"Images/Button/b_No.png",
		CC_CALLBACK_1(EndPopup::doClick, this));
	
	pMenuItem2->setPosition(Vec2(endmenu->getContentSize().width / 2 - 20, 10));
	pMenuItem2->setAnchorPoint(Vec2(1, 0));

	pMenuItem1->setTag(911);
	pMenuItem2->setTag(912);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, NULL);
	pMenu->setPosition(Vec2(0, 0));
	endmenu->addChild(pMenu);

	return;
}

void EndPopup::onEnter() {
	Layer::onEnter();
	Director::getInstance()->pause();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(EndPopup::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void EndPopup::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	Director::getInstance()->resume();
	Layer::onExit();
}
bool EndPopup::onTouchBegan(Touch* touch, Event* event) {

	return true;
}

void EndPopup::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();

	if (i == 911)
	{
		if(nowSceneName == "MainScene")
		{
			Director::sharedDirector()->end();
		}
		else
		{
			auto pScene = MainScene::createScene();
			Director::getInstance()->replaceScene(pScene);
		}
		log("Yes");
	}
	else if (i == 912)
	{
		this->removeFromParentAndCleanup(true);
		log("No");
	}
}
