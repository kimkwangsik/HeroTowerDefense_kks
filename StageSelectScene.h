#ifndef __StageSelectScene_H__
#define __StageSelectScene_H__

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
#include "CustomTableViewCell.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class StageSelectScene : public cocos2d::LayerColor
	, public cocos2d::extension::TableViewDataSource
	, public cocos2d::extension::TableViewDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(StageSelectScene);

	int maxStage;
	virtual void scrollViewDidScroll(ScrollView* view) {};
	virtual void scrollViewDidZoom(ScrollView* view) {}
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	//cocos2d::MenuItemImage* pMenuItem1;
	//TableViewCell *cell;
	Menu *pMenu;

	void doClick(Ref* pSender, int a);
};

#endif // __HELLOWORLD_SCENE_H__
