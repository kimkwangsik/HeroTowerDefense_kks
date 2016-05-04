#ifndef __GameStageScene_H__
#define __GameStageScene_H__

#include "cocos2d.h"
#include "Tower.h"
#include "Monster.h"

class GameStageScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	cocos2d::TMXTiledMap* tmap;
	cocos2d::TMXLayer* metainfo;
	cocos2d::TMXObjectGroup* objects;

	cocos2d::Vector<cocos2d::Sprite*> _heart;
	int _heartCount;
	void heartCreate(int a, cocos2d::Vec2 position);

	cocos2d::Size winSize;
	cocos2d::Size VisibleWinSize;
	cocos2d::Size winSizePixel;
	cocos2d::Size sizeRate;
	cocos2d::Vec2 origin;

	cocos2d::Menu* towerMenu;
	
	cocos2d::MenuItemImage* towerMenuOnOff;

	void createStage(int level);

	int towerTpye;
	Tower* clickTower;
	cocos2d::Sprite* moveSprtie;
	cocos2d::Vector<Tower*> _setupTower;
	void setupTower(Tower* tower);

	cocos2d::EventListener* _listenter;

	int viaX[15];
	int viaY[15];


	void addMonster();
	void removeMonster(Monster* slime);
	//cocos2d::Sequence* abcde;

	cocos2d::Sequence* MoveAction(Monster* slime);
	cocos2d::Sequence* SequenceMoveAction(Monster* slime, int num, int max);
	cocos2d::Sequence* SequenceMonsterAdd(int num, int max);
	cocos2d::Vector<cocos2d::MoveTo*> _Action;

	cocos2d::Vector<Monster*> _monster;
	cocos2d::Vector<Monster*> *_pMonster;

	std::vector<cocos2d::Vec2> _Vec2Point;

	//cocos2d::Vector<cocos2d::Vec2> objVec2;


	cocos2d::Sprite* timerBase;
	cocos2d::Sprite* timerGauge;
	int gauge;
	cocos2d::ProgressTimer *gaugeBar;
	int phaseLevel = 0;
	cocos2d::LabelTTF* phaseLabel;

	bool towerTouch = false;

	void myTick(float f);
	void SpriteProgressToRadial(float f);

	//cocos2d::Vector<cocos2d::Sprite*> _monster;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void doClick(Ref* pSender);

	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 positionForTileCoord(cocos2d::Vec2 position);
	GameStageScene(int stagelevel);

	bool towerMenustatus = true;
	float towerMenuSize;

	void masicMenuCreate();
	void towerMenuCreate();



};

#endif // __HELLOWORLD_SCENE_H__
