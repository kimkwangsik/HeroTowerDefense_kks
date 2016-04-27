#include "tower.h"

USING_NS_CC;

Tower::Tower(int typenum)
	: _listenter(nullptr)
	, _fixedPriority(0)
	, _useNodePriority(false)
	, _towerType(typenum)
{
	bool b0k = initWithTexture(nullptr, Rect::ZERO);
	if (b0k)
	{
		this->autorelease();
	}

	//towerSetup(typenum);
}

void Tower::setPriority(int fixedPriority)
{
	_fixedPriority = fixedPriority;
	_useNodePriority = false;
}

void Tower::setPriorityWithThis(bool useNodePriority)
{
	_useNodePriority = useNodePriority;
}

void Tower::onEnter()
{
	Sprite::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		return true;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* event)
	{
		//this->setPosition(this->getPosition() + touch->getDelta());
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		//this->setColor(Color3B::WHITE);
		//this->setOpacity(255);
	};

	if (_useNodePriority)
	{
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
	else
	{
		_eventDispatcher->addEventListenerWithFixedPriority(listener, _fixedPriority);
	}
	_listenter = listener;
};

void Tower::onExit() {
	_eventDispatcher->removeEventListener(_listenter);
	Sprite::onExit();
};

void Tower::towerSetup(int typenum)
{
	if (typenum == 1)
	{
		
	}
	else if (typenum == 2)
	{
		auto tower = Sprite::create("Images/blue.png");
	}
	else if (typenum == 3)
	{
		auto tower = Sprite::create("Images/yellow.png");
	}
	else
	{
		log("타입 설정이 잘못 되었습니다.");
	}
}