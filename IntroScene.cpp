#include "IntroScene.h"
#include "MainScene.h"
#include "Menus.h"
#include "SimpleAudioEngine.h"

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
	if (!Layer::init())
	{
		return false;
	}

	Size winSize = Director::getInstance()->getVisibleSize();

	auto Back_Castel = Sprite::create("Images/Back_Castel.png");
	Back_Castel->setAnchorPoint(Vec2(0, 0));

	addChild(Back_Castel, 0);

	auto pLabel = LabelTTF::create("touch to start",
		"Arial", 34);

	pLabel->setPosition(Vec2(winSize.width - 20, winSize.height / 2));
	pLabel->setColor(Color3B::BLACK);
	pLabel->setAnchorPoint(Vec2(1, 0.5));
	pLabel->setOpacity(255.0);
	this->addChild(pLabel);

	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Sound/Music/Woodland Fantasy.mp3", true);

	createKnight(1);

	//항상 처음 실행 하게 만드는 이벤트
	UserDefault::getInstance()->setBoolForKey("Start_First", false);

	bool start_First = UserDefault::getInstance()->getBoolForKey("Start_First");
	if (start_First)
	{
		
	}
	else //최초 실행시
	{
		UserDefault::getInstance()->setBoolForKey("Start_First", true);
		UserDefault::getInstance()->setBoolForKey("Hero1", false);
		UserDefault::getInstance()->setIntegerForKey("Hero1_Level", 0);
		UserDefault::getInstance()->setBoolForKey("Hero2", false);
		UserDefault::getInstance()->setIntegerForKey("Hero2_Level", 0);
		UserDefault::getInstance()->setBoolForKey("Hero3", false);
		UserDefault::getInstance()->setIntegerForKey("Hero3_Level", 0);
		UserDefault::getInstance()->setIntegerForKey("have_gold", 2000);
		UserDefault::getInstance()->setIntegerForKey("clear_stage", 0);
		UserDefault::getInstance()->setBoolForKey("sound", true);
		UserDefault::getInstance()->setBoolForKey("vibration", true);
	}

	bool soundon = UserDefault::getInstance()->getBoolForKey("sound");
	if (soundon)
	{

	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	return true;
}
  
void IntroScene::createKnight(float a)
{
	introSprite = Sprite::create("Images/Tower/Knight1/Walking_Horizontal_1.png");
	
	introSprite->setPosition(Vec2(270 + introSprite->getContentSize().width / 2,
		28 + introSprite->getContentSize().height / 2));
	//introSprite->setAnchorPoint(Vec2::ZERO);
	addChild(introSprite);

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.125f);

	animation->addSpriteFrameWithFile("Images/Tower/Knight1/Walking_Horizontal_1.png");
	animation->addSpriteFrameWithFile("Images/Tower/Knight1/Walking_Horizontal_2.png");
	animation->addSpriteFrameWithFile("Images/Tower/Knight1/Walking_Horizontal_3.png");
	animation->addSpriteFrameWithFile("Images/Tower/Knight1/Walking_Horizontal_4.png");
	auto animate = Animate::create(animation);
	auto repAnimate = RepeatForever::create(animate);

	auto replace = Place::create(Vec2(270 + introSprite->getContentSize().width / 2,
		28 + introSprite->getContentSize().height / 2));

	auto seqMove = Sequence::create(replace, MoveBy::create(5, Vec2(250, 0)), nullptr);
	auto repMove = RepeatForever::create(seqMove);

	introSprite->runAction(repAnimate);
	introSprite->runAction(repMove);
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

	auto touchPoint = touch->getLocation();
	log("touch : %f , %f", touchPoint.x, touchPoint.y);

	if (introSprite->getBoundingBox().containsPoint(touchPoint))
	{
		introSprite->setRotation(0.f);
		auto Action = MoveBy::create(1, Vec2(-300, 300));
		auto roAction = RotateBy::create(1, -720.f);

		auto throw_action = Spawn::create(Action, roAction, nullptr);

		introSprite->runAction(throw_action);
		return true;
	}

	auto pScene = MainScene::createScene();
	Director::getInstance()->replaceScene(pScene);

	return true;
}