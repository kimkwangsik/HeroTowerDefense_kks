#include "Menus.h"
#include "Options.h"

USING_NS_CC;

Scene* Menus::createScene()
{
	auto scene = Scene::create();
	auto layer = Menus::create();
	scene->addChild(layer);

	return scene;
}

bool Menus::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menubar = Sprite::create("Images/MenuButton/menubar.png");// 1024 x 66 ������ �׸�
	menubar->setPosition(Vec2(winSize.width / 2, winSize.height + origin.y));
	menubar->setAnchorPoint(Vec2(0.5, 1));
	menubar->setTag(300);
	addChild(menubar);

	log("%f", menubar->getContentSize().height);

	auto pMenuItem1 = MenuItemImage::create(
		"Images/Button/b_Parameters.png",
		"Images/Button/b_Parameters.png",
		CC_CALLBACK_1(Menus::doClick, this));
	pMenuItem1->setPosition(Vec2(winSize.width, 0));
	pMenuItem1->setAnchorPoint(Vec2(1, 0));

	MenuItemImage* pMenuItem2;

	if (1)
	{
		pMenuItem2 = MenuItemImage::create(
			"Images/Button/b_No.png",
			"Images/Button/b_No.png",
			CC_CALLBACK_1(Menus::doClick, this));
	}
	else
	{
		pMenuItem2 = MenuItemImage::create(
			"Images/Button/b_back.png",
			"Images/Button/b_back.png",
			CC_CALLBACK_1(Menus::doClick, this));
	}
	

	pMenuItem2->setPosition(Vec2(0, 0));
	pMenuItem2->setAnchorPoint(Vec2(0, 0));

	pMenuItem1->setTag(11);
	pMenuItem2->setTag(12);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, NULL);
	pMenu->setPosition(Vec2(0, 0));
	menubar->addChild(pMenu);

	return true;
}

void Menus::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 11)
	{
		log("�޴��� ���õǾ����ϴ�.");
		auto pScene = Options::createScene();
		this->addChild(pScene,3000);
	}
	else if (i == 12)
	{
		log("X�� Ŭ�� �ϼ˽��ϴ�.");
		Director::getInstance()->popScene();
	}
	
}

void Menus::doMsgReceived(Ref* obj)
{
	

	/*char *inputStr = (char*)obj;
	
	sprintf(sceneText, "%s", inputStr);

	log("[%s] �޽��� ����", sceneText);*/
	
}