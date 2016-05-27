#include "Options.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

void callJavaMethodOption(std::string func)
{
	JniMethodInfo t;

	if (JniHelper::getStaticMethodInfo(t
		, "org/cocos2dx/cpp/AppActivity"
		, func.c_str()
		, "()V"))
	{
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
#else
//#include "Util/dmob/LayerAdmob.h"
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
Scene* Options::createScene()
{
	auto scene = Scene::create();
	auto layer = Options::create();
	scene->addChild(layer);

	return scene;
}

bool Options::init()
{
	if (!Layer::init())
	{
		return false;
	}
	winSize = Director::getInstance()->getVisibleSize();

	auto optionbar = Sprite::create("Images/g4396.png");
	optionbar->setPosition(Vec2(winSize.width / 2, winSize.height /2));
	addChild(optionbar);

	MenuItemFont::setFontSize(20);
	auto pMenuItem1 = MenuItemFont::create(
		" 창 닫기 ",
		CC_CALLBACK_1(Options::doClick, this));
	pMenuItem1->setPosition(Vec2(optionbar->getContentSize().width / 2, 10));
	pMenuItem1->setColor(Color3B::BLACK);
	pMenuItem1->setAnchorPoint(Vec2(0.5, 0));
	//pMenuItem1->setScale(0.25f);

	/*auto soundSprite = Sprite::create("Images/Button/b_Sound1.png");
	soundSprite->setPosition(Vec2(optionbar->getContentSize().width / 4, optionbar->getContentSize().height - 10));
	soundSprite->setAnchorPoint(Vec2(0.5, 1));
	optionbar->addChild(soundSprite);*/

	auto soundSprite = LabelTTF::create("소리","",20);
	soundSprite->setPosition(Vec2(optionbar->getContentSize().width / 4, optionbar->getContentSize().height - 10));
	soundSprite->setAnchorPoint(Vec2(0.5, 1));
	soundSprite->setColor(Color3B::BLACK);
	optionbar->addChild(soundSprite);

	pMenuItem2 = MenuItemFont::create(
		" On ",
		CC_CALLBACK_1(Options::doClick, this));
	pMenuItem2->setPosition(Vec2(optionbar->getContentSize().width / 4 * 3, optionbar->getContentSize().height - 10));
	pMenuItem2->setColor(Color3B::BLACK);
	pMenuItem2->setAnchorPoint(Vec2(0.5, 1));

	/*auto vibSprite = Sprite::create("Images/Button/b_Restart.png");
	vibSprite->setPosition(Vec2(optionbar->getContentSize().width / 4, optionbar->getContentSize().height - 20 - pMenuItem2->getContentSize().height));
	vibSprite->setAnchorPoint(Vec2(0.5, 1));
	optionbar->addChild(vibSprite);*/

	auto vibSprite = LabelTTF::create("진동", "", 20);
	vibSprite->setPosition(Vec2(optionbar->getContentSize().width / 4, optionbar->getContentSize().height - 20 - pMenuItem2->getContentSize().height));
	vibSprite->setColor(Color3B::BLACK);
	vibSprite->setAnchorPoint(Vec2(0.5, 1));
	optionbar->addChild(vibSprite);

	pMenuItem3 = MenuItemFont::create(
		" On ",
		CC_CALLBACK_1(Options::doClick, this));
	pMenuItem3->setPosition(Vec2(optionbar->getContentSize().width / 4 * 3, optionbar->getContentSize().height - 20 - pMenuItem2->getContentSize().height));

	pMenuItem3->setColor(Color3B::BLACK);
	pMenuItem3->setAnchorPoint(Vec2(0.5, 1));
	//pMenuItem1->setScale(0.25f);

	bool soundon = UserDefault::getInstance()->getBoolForKey("sound");
	if (soundon)
	{
		pMenuItem2->setString(" On ");
	}
	else
	{
		pMenuItem2->setString(" Off ");
	}

	bool vibon = UserDefault::getInstance()->getBoolForKey("vibration");
	if (vibon)
	{
		pMenuItem3->setString(" On ");
	}
	else
	{
		pMenuItem3->setString(" Off ");
	}

	pMenuItem1->setTag(621);
	pMenuItem2->setTag(622);
	pMenuItem3->setTag(623);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, NULL);
	pMenu->setPosition(Vec2::ZERO);
	optionbar->addChild(pMenu);

	return true;
}
void Options::onEnter() {
	Layer::onEnter();
	doShow(this);
	Director::getInstance()->pause();
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Options::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
void Options::onExit() {
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	doHide(this);
	Director::getInstance()->resume();
	Layer::onExit();
}
bool Options::onTouchBegan(Touch* touch, Event* event) {

	return true;
}
void Options::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();

	if (i == 621)
	{
		this->removeFromParentAndCleanup(true);
	}
	else if (i == 622)
	{
		bool soundon = UserDefault::getInstance()->getBoolForKey("sound");
		if (soundon)
		{
			log("sound off");
			pMenuItem2->setString(" Off ");
			UserDefault::getInstance()->setBoolForKey("sound", false);
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		else
		{
			log("sound on");
			pMenuItem2->setString(" On ");
			UserDefault::getInstance()->setBoolForKey("sound", true);
			CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
	}
	else if (i == 623)
	{
		bool vibon = UserDefault::getInstance()->getBoolForKey("vibration");
		if (vibon)
		{
			log("vib off");
			pMenuItem3->setString(" Off ");
			UserDefault::getInstance()->setBoolForKey("vibration", false);
		}
		else
		{
			log("vib on");
			pMenuItem3->setString(" On ");
			UserDefault::getInstance()->setBoolForKey("vibration", true);
		}
	}
	
}

void Options::doShow(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethodOption("ShowAdPopup");

#endif

}

void Options::doHide(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	callJavaMethodOption("HideAdPopup");

#endif

}