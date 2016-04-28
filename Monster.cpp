#include "Monster.h"

USING_NS_CC;

Monster::Monster()
	: _listenter(nullptr)
	, _fixedPriority(0)
	, _useNodePriority(false)
{
	bool b0k = initWithTexture(nullptr, Rect::ZERO);
	if (b0k)
	{
		this->autorelease();
	}
}

void Monster::remove()
{
	log("remove()");
	removeFromParent();
	onExit();
}

void Monster::setPriority(int fixedPriority)
{
	_fixedPriority = fixedPriority;
	_useNodePriority = false;
}

void Monster::setPriorityWithThis(bool useNodePriority)
{
	_useNodePriority = useNodePriority;
}

void Monster::onEnter()
{
	Sprite::onEnter();
};

void Monster::onExit() {
	_eventDispatcher->removeEventListener(_listenter);
	Sprite::onExit();
};