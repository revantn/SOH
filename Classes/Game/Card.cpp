#include "Card.h"
#include "Board.h"
#include "GameConstants.h"
USING_NS_CC;

int Card::s_SelectedCard = INVALID_CARD;

int Card::getSelectedCard()
{
	return s_SelectedCard;
}
void Card::resetSelectedCard()
{
	s_SelectedCard = INVALID_CARD;
}
Card::Card(int id)
{
}
void Card::setCardValue(int value)
{
	this->m_faceValue = value;
}
Card* Card::createCard(char const *texture)
{
	Card* card = new Card();
	if (card && card->initWithFile(texture))
	{
		card->autorelease();
#ifdef USE_SCALE_FACTOR
		auto scaleX = Board::getBoardInstance()->getScaleFactorX();
		auto scaleY = Board::getBoardInstance()->getScaleFactorY();

		card->setScale(scaleX, scaleY);
#endif
		//card->setFlippedX(true);
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
	
}



void Card::returnToPosition()
{
	//setZOrder((m_faceValue % SUIT_SIZE) - 200);
	auto moveTo = MoveTo::create(0.25, m_position);
	runAction(moveTo);
}
//void Card::moveToPosition(Vec2 targetPosition)
//{
//	//setZOrder((m_faceValue % SUIT_SIZE) - 200);
//	auto moveTo = MoveTo::create(0.25, targetPosition);
//	runAction(moveTo);
//}
void Card::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (!(Board::s_enablePlayerCardTouch && handleTouchEvents))
		{
			return false;
		}
		return this->onTouchDown(touch, event);
	};

	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (!(Board::s_enablePlayerCardTouch && handleTouchEvents))
		{
			return;
		}
		this->onTouchUp(touch, event);
	};
	listener->onTouchMoved = [=](Touch* touch, cocos2d::Event* event)
	{
		if (!(Board::s_enablePlayerCardTouch && handleTouchEvents))
		{
			return;
		}
		this->onTouchMove(touch, event);
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void Card::removeEvents()
{
	cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}
void Card::setCardPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
	Sprite::setPosition(x, y);
}
bool Card::onTouchDown(cocos2d::Touch* touch, cocos2d::Event* event)
{
	//setZOrder(600);
//	return true;
	cocos2d::Vec2 p = touch->getLocation();
	cocos2d::Rect rect = this->getBoundingBox();

	if (rect.containsPoint(p))
	{
		setLocalZOrder(getLocalZOrder() + 200);
		return true; // to indicate that we have consumed it.
	}

	return false; // we did not consume this event, pass thru.
}

void Card::onTouchUp(cocos2d::Touch* touch, cocos2d::Event* event)
{
	s_SelectedCard = m_faceValue;
	setLocalZOrder(getLocalZOrder() - 200);

	//auto moveTo = MoveTo::create(0.25, m_position);
	//runAction(moveTo);
}

void Card::onTouchMove(cocos2d::Touch* touch, cocos2d::Event* event)
{

	setPosition(getPosition() + touch->getDelta());
}


void Card::enableTouch(bool value)
{
	handleTouchEvents = value;
}
