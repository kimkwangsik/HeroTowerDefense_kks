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


	//NotificationCenter::getInstance()->addObserver(this,
	//	callfuncO_selector(Menus::doMsgReceived),
	//	"SceneStatus",
	//	nullptr);

	//log("%f, %f", winSize.width, winSize.height);

	auto menubar = Sprite::create("Images/CyanSquare32.png");// 100 x 100 사이즈 그림
	menubar->setScaleX(10.24);
	menubar->setScaleY(0.66);
	menubar->setPosition(Vec2(winSize.width / 2, winSize.height));
	//menubar->setOpacity(100.0f);
	menubar->setAnchorPoint(Vec2(0.5, 1));
	menubar->setTag(300);
	addChild(menubar);

	auto pMenuItem1 = MenuItemImage::create(
		"Images/Button/scaled-at-50/b_Parameters.png",
		"Images/Button/scaled-at-50/b_Parameters.png",
		CC_CALLBACK_1(Menus::doClick, this));
	pMenuItem1->setPosition(Vec2(winSize.width, winSize.height));
	pMenuItem1->setAnchorPoint(Vec2(1, 1));

	MenuItemImage* pMenuItem2;

	if (1)
	{
		pMenuItem2 = MenuItemImage::create(
			"Images/Button/scaled-at-50/b_No.png",
			"Images/Button/scaled-at-50/b_No.png",
			CC_CALLBACK_1(Menus::doClick, this));
	}
	else
	{
		pMenuItem2 = MenuItemImage::create(
			"Images/Button/scaled-at-50/b_back.png",
			"Images/Button/scaled-at-50/b_back.png",
			CC_CALLBACK_1(Menus::doClick, this));
	}
	

	pMenuItem2->setPosition(Vec2(0, winSize.height));
	pMenuItem2->setAnchorPoint(Vec2(0, 1));

	pMenuItem1->setTag(11);
	pMenuItem2->setTag(12);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, NULL);
	pMenu->setPosition(Vec2::ZERO);
	this->addChild(pMenu);

	return true;
}

void Menus::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	if (i == 11)
	{
		log("메뉴가 선택되었습니다.");
		auto pScene = Options::createScene();
		this->addChild(pScene,3000);
	}
	else if (i == 12)
	{
		log("X를 클릭 하셧습니다.");
		Director::getInstance()->popScene();
	}
	
}

void Menus::doMsgReceived(Ref* obj)
{
	

	/*char *inputStr = (char*)obj;
	
	sprintf(sceneText, "%s", inputStr);

	log("[%s] 메시지 도착", sceneText);*/
	
}