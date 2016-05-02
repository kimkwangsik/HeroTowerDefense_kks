#include "StageSelectScene.h"
#include "GameStageScene.h"
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

	auto tableViewBackGround = Sprite::create("Images/tableViewBackGround1.png");
	tableViewBackGround->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(tableViewBackGround, 2);

	TableView* tableView1 = TableView::create(this,
		Size(tableViewBackGround->getContentSize().width-20,
			tableViewBackGround->getContentSize().height));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(10,0));
	tableView1->setDelegate(this);
	tableView1->setTag(100);
	tableViewBackGround->addChild(tableView1,3);
	tableView1->reloadData();



	auto pScene = Menus::createScene();
	this->addChild(pScene, 2000);
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

void StageSelectScene::doClick(Ref* pSender, int a)
{
	log("%d��° ���������� ���õǾ����ϴ�.", a);

	auto pScene = GameStageScene::createScene();
	auto stagenum = new GameStageScene(a);
	stagenum->autorelease();
	pScene->addChild(stagenum);
	Director::getInstance()->pushScene(pScene);
}
TableViewCell* StageSelectScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
	auto string = String::createWithFormat("%ld", idx);
	char str[20];
	sprintf(str, "Level\n%ld", idx+1);
	int idxint = idx + 1;
	cell = table->dequeueCell();
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
		pMenuItem->setTag(150);

		auto label = LabelTTF::create(str, "Helvetica", 20.0);	//��Ʈ ���� �Ǵ� Ȯ�� �ʼ�
		label->setPosition(Vec2(pMenuItem->getContentSize().width / 2,
			80 + pMenuItem->getContentSize().height / 2));
		label->setAnchorPoint(Vec2(0.5, 0.5));
		label->setTag(123);
		cell->addChild(label,3);
	}
	else
	{
		auto label = (LabelTTF*)cell->getChildByTag(123);
		label->setString(str);
		 
		pMenuItem = (MenuItemImage*)cell->getChildByTag(150);
		pMenuItem = MenuItemImage::create(
			"Images/box-highres.png",
			"Images/box-highres.png",
			CC_CALLBACK_1(StageSelectScene::doClick, this, idxint));
		pMenuItem->setPosition(Vec2(0, 0));
		pMenuItem->setAnchorPoint(Vec2(0, 0));
	}
	
	auto pMenu = Menu::create(pMenuItem, NULL);

	pMenu->setPosition(Vec2(0, 80));
	pMenu->setAnchorPoint(Vec2(0, 0.5));
	cell->addChild(pMenu,2);

	return cell;
}

ssize_t StageSelectScene::numberOfCellsInTableView(TableView* table)
{
	return 20;
}