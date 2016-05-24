#include "StageSelectScene.h"
//#include "GameStageScene.h"
#include "SelectStageInfo.h"
#include "Menus.h"

USING_NS_CC;

Scene* StageSelectScene::createScene()
{
	auto scene = Scene::create();
	auto layer = StageSelectScene::create();
	scene->addChild(layer);

	return scene;
}

bool StageSelectScene::init()
{
	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();

	

	auto pScene1 = Menus::createScene();
	auto scenestr1 = new Menus("StageSelectScene");
	scenestr1->autorelease();
	pScene1->addChild(scenestr1);
	this->addChild(pScene1, 3);

	auto MainBackGround = Sprite::create("TileMaps/Main.png");
	MainBackGround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	MainBackGround->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(MainBackGround, 0);

	maxStage = UserDefault::getInstance()->getIntegerForKey("clear_stage");

	auto tableViewBackGround = Sprite::create("Images/tableViewBackGround.png");
	tableViewBackGround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(tableViewBackGround, 2);

	TableView* tableView1 = TableView::create(this,
		Size(tableViewBackGround->getContentSize().width-20,
			tableViewBackGround->getContentSize().height));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(10,0));
	tableView1->setDelegate(this);
	tableView1->setTag(201);
	tableViewBackGround->addChild(tableView1,3);
	tableView1->reloadData();

	char maxStagestrC[100];
	sprintf(maxStagestrC, "최종 클리어 스테이지\nLevel%d", maxStage);

	auto maxStageLabel = LabelTTF::create(maxStagestrC, "Helvetica", 20.0);	//폰트 수정 또는 확인 필수
	maxStageLabel->setPosition(Vec2(tableViewBackGround->getContentSize().width/2,10));
	maxStageLabel->setAnchorPoint(Vec2(0.5, 0));
	maxStageLabel->setColor(Color3B::BLACK);
	tableViewBackGround->addChild(maxStageLabel, 3);

	return true;
}


void StageSelectScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	//log("Tag : %d\nCell touched at index: %ld", table->getTag(), cell->getIdx());
}
Size StageSelectScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
	if (idx+1 == StageSelectScene::numberOfCellsInTableView(table))
	{
		return Size(100, 200);
	}

	return Size(200, 200);
}

void StageSelectScene::doClick(Ref* pSender, int selectStage)
{
	log("%d번째 스테이지가 선택되었습니다.", selectStage);

	
	if (maxStage+1 < selectStage)
	{
		log("이전 스테이지를 클리어 해주세요");
		return;
	}
	auto pScene = SelectStageInfo::createScene();
	auto stagenum = new SelectStageInfo(selectStage);
	stagenum->autorelease();
	pScene->addChild(stagenum);
	addChild(pScene, 4);
	//Director::getInstance()->replaceScene(pScene);
}
TableViewCell* StageSelectScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
	int idxint = idx + 1;
	char str[20];
	sprintf(str, "Level\n%d", idxint);
	auto cell = table->dequeueCell();

	MenuItemImage* pMenuItem;

	if (!cell)
	{
		cell = new CustomTableViewCell();
		cell->autorelease();

		pMenuItem = MenuItemImage::create(
			"Images/box-highres.png",
			"Images/box-highres.png",
			CC_CALLBACK_1(StageSelectScene::doClick, this , idxint));
		pMenuItem->setPosition(Vec2(0, 0));
		pMenuItem->setAnchorPoint(Vec2(0, 0));
		pMenuItem->setTag(211);

		auto label = LabelTTF::create(str, "Helvetica", 20.0);	//폰트 수정 또는 확인 필수
		label->setPosition(Vec2(pMenuItem->getContentSize().width / 2,
			80 + pMenuItem->getContentSize().height / 2));
		label->setAnchorPoint(Vec2(0.5, 0.5));
		label->setTag(212);
		cell->addChild(label,3);

		
	}
	else
	{
		auto label = (LabelTTF*)cell->getChildByTag(212);
		label->setString(str);
		 
		pMenuItem = (MenuItemImage*)cell->getChildByTag(211);
		pMenuItem = MenuItemImage::create(
			"Images/box-highres.png",
			"Images/box-highres.png",
			CC_CALLBACK_1(StageSelectScene::doClick, this, idxint));
		pMenuItem->setPosition(Vec2(0, 0));
		pMenuItem->setAnchorPoint(Vec2(0, 0));
		
	}
	
	pMenu = Menu::create(pMenuItem, NULL);
	pMenu->setPosition(Vec2(0, 80));
	pMenu->setAnchorPoint(Vec2(0, 0.5));

	cell->addChild(pMenu,2);

	return cell;
}

ssize_t StageSelectScene::numberOfCellsInTableView(TableView* table)
{
	return 20;
}