#include "MainScene.h"
#include "StageSelectScene.h"
#include "Menus.h"

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



	/*auto layer1 = LayerGradient::create(Color4B(255, 0, 0, 255), Color4B(255, 0, 255, 255));
	layer1->setContentSize(Size(80, 80));
	layer1->setPosition(Vec2(50, 50));
	addChild(layer1);*/

	auto pMenuItem1 = MenuItemFont::create(
		" StageSelect ",
		CC_CALLBACK_1(MainScene::doClick, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));

	auto pMenuItem2 = MenuItemFont::create(
		" 무한모드 ",
		CC_CALLBACK_1(MainScene::doClick, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));

	auto pMenuItem3 = MenuItemFont::create(
		" 영웅 관리 ",
		CC_CALLBACK_1(MainScene::doClick, this));
	pMenuItem3->setColor(Color3B(0, 0, 0));

	pMenuItem1->setTag(1);
	pMenuItem2->setTag(2);
	pMenuItem3->setTag(3);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, NULL);

	pMenu->alignItemsVertically();

	this->addChild(pMenu);
	auto pScene = Menus::createScene();
	this->addChild(pScene);

    return true;
}
//void MainScene::onEnter()
//{
	//Layer::onEnter();
	//Director::sharedDirector()->
//}

void MainScene::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	log("%d번째 메뉴가 선택되었습니다.", i);
	if (i == 1)
	{
		auto pScene = StageSelectScene::createScene();
		Director::getInstance()->pushScene(pScene);
	}

}


void MainScene::doNoClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	log("%d번째 메뉴가 선택되었습니다.", i);

}