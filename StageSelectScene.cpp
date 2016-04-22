#include "StageSelectScene.h"
#include "GameStageScene.h"

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
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
	{
		return false;
	}

	TableView* tableView1 = TableView::create(this, Size(440, 280));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(20, 20));
	tableView1->setDelegate(this);
	tableView1->setTag(100);
	this->addChild(tableView1);
	tableView1->reloadData();

	return true;
}


void StageSelectScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
	
	
	log("Tag : %d\nCell touched at index: %ld", table->getTag(), cell->getIdx());
	
	auto sprite = (Sprite*)cell->getChildByTag(150);

	auto pScene = GameStageScene::createScene();
	Director::getInstance()->pushScene(pScene);

}
Size StageSelectScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
	if (idx+1 == StageSelectScene::numberOfCellsInTableView(table))
	{
		return Size(100, 200);
	}

	return Size(200, 200);
}
TableViewCell* StageSelectScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
	auto string = String::createWithFormat("%ld", idx);
	
	char str[20];
	sprintf(str, "Level\n%ld", idx+1);
	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CustomTableViewCell();
		cell->autorelease();
		auto sprite = Sprite::create("Images/CyanSquare64.png");
		sprite->setPosition(Vec2(0, 100));
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setTag(150);
		cell->addChild(sprite);

		auto label = LabelTTF::create(str, "Helvetica", 20.0);	//폰트 수정 또는 확인 필수
		label->setPosition(Vec2(sprite->getContentSize().width / 2, 100 + sprite->getContentSize().height / 2));
		label->setAnchorPoint(Vec2(0.5, 0.5));
		label->setTag(123);
		cell->addChild(label);
	}
	else
	{
		auto label = (LabelTTF*)cell->getChildByTag(123);
		label->setString(str);
	}

	return cell;
}

ssize_t StageSelectScene::numberOfCellsInTableView(TableView* table)
{
	return 4;
}