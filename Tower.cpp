#include "tower.h"

USING_NS_CC;

Tower::Tower(int typenum)
	: _listenter(nullptr)
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

void Tower::setPriorityWithThis(bool useNodePriority)
{
	_useNodePriority = useNodePriority;
}

void Tower::onEnter()
{
	Sprite::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);

	b_Yes = Sprite::create("Images/Button/scaled-at-25/b_Yes.png");
	b_Yes->setPosition(Vec2(30, 30));
	b_Yes->setAnchorPoint(Vec2(1, 0));
	addChild(b_Yes, 50);

	b_No = Sprite::create("Images/Button/scaled-at-25/b_No.png");
	b_No->setPosition(Vec2(0, 30));
	b_No->setAnchorPoint(Vec2(0, 0));
	addChild(b_No, 50);

	towerMenuVisible = true;


	listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		Vec2 LocationInNode = this->convertToNodeSpace(touch->getLocation());

			bool b_YesTouch = b_Yes->getBoundingBox().containsPoint(LocationInNode);
			if (b_YesTouch && towerMenuVisible)
			{
				log("Yes!");
				towerSetup = true;
				towerMenuVisible = false;
				//removeChild(b_No, false);
				//removeChild(b_Yes, true);
				b_No->setVisible(false);
				b_Yes->setVisible(false);
				this->setOpacity(255.f);
				return true;
			}

			bool b_NoTouch = b_No->getBoundingBox().containsPoint(LocationInNode);
			if (b_NoTouch && towerMenuVisible)
			{
				towerSetup = false;
				towerMenuVisible = false;
				removeFromParent();
				log("No..");
			}

		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		
		if (rect.containsPoint(LocationInNode))
		{
			log("%d¹ø Å¸¿ö", _towerType);

			/*auto abc = Sprite::create("Images/bar_base.png");
			abc->setPosition(Vec2(0, 20));
			abc->setAnchorPoint(Vec2(0.5, 0));
			addChild(abc, 50);*/

			return true;
		}

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

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 30);
	_listenter = listener;
};

void Tower::onExit() {
	_eventDispatcher->removeEventListener(_listenter);
	Sprite::onExit();
};
//
//bool Tower::towerSetup(int typenum)
//{
//	return true;
//}