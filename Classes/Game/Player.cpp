#include "Player.h"

#include "Card.h"
#include "CardDeck.h"
#include "Board.h"
#include "BoardState.h"
#include <algorithm>

Player::Player(int id, int playerType) 
{
	m_id = id;
	m_playerType = playerType;
	m_cardDeck = CardDeck::getInstance();
}

int Player::getNumberofInhandCards() const
{
	return m_numOfInHandCards;
}
void Player::setPlayerName(std::string name)
{
	m_playerName = name;
	//cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::Size size = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto label1 = cocos2d::LabelTTF::create(name, "font/arial.ttf", 18);
	label1->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
	this->addChild(label1);
	switch (m_id)
	{
		case PLAYER_POSITION::EAST:
			label1->setPosition(size.width * 0.90, size.height * 0.1);
			break;
		case PLAYER_POSITION::WEST:
			label1->setPosition(size.width * 0.1, size.height * 0.89);
			break;
		case PLAYER_POSITION::NORTH:
			label1->setPosition(size.width * 0.78, size.height * 0.89);
			break;
		case PLAYER_POSITION::SOUTH:
			label1->setPosition(size.width * 0.29, size.height * 0.05);
			break;
	}
}

bool Player::init(int *cards, int *isMyturn)
{
	m_boardState = BoardState::getBoardState();
	for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
	{
		m_inHandCards[i] = *(cards + i);
		if (m_inHandCards[i] == CARD_DECK::HEART_7)
			*isMyturn = m_id;
	}
	std::sort(m_inHandCards, m_inHandCards + MAX_IN_HAND_CARDS);
	m_numOfInHandCards = MAX_IN_HAND_CARDS;
	initPlayerPosition();
	return false;
}


void Player::initPlayerPosition()
{
	cocos2d::Size size = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	int x = 0, y = 0;

	switch (m_id)
	{
	case PLAYER_POSITION::WEST:
			x = size.width/ 19;
			y = size.height / 3.2;
			for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
			{
#ifdef DEBUG_AI
				auto card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setCardPosition(x, y + (i * PLAYER_CARD_OFFSET));
				card->setVisible(true);
				card->addEvents();
#else 
				auto cardCover = m_cardDeck->getCardCover(m_inHandCards[i]);
				cardCover->setVisible(true);
				cardCover->setPosition(x, y + (i * PLAYER_CARD_OFFSET));
				auto card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setPosition(x, y + (i * PLAYER_CARD_OFFSET));
				setPlayerName("West");

#endif
			}
			break;
		case PLAYER_POSITION::EAST:
			x = size.width/1.0538 ;
			y = size.height / 3.2;
			for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
			{
#ifdef DEBUG_AI
				auto card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setCardPosition(x, y + (i * PLAYER_CARD_OFFSET));
				card->setVisible(true);
				card->addEvents();

#else 
				auto cardCover = m_cardDeck->getCardCover(m_inHandCards[i]);
				cardCover->setVisible(true);
				cardCover->setPosition(x, y + (i * PLAYER_CARD_OFFSET));
				auto card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setPosition(x, y + (i * PLAYER_CARD_OFFSET));
#endif
				setPlayerName("East");
			}
			break;
		case PLAYER_POSITION::NORTH:
			x = size.width / 2.5;
			y = size.height/1.08;
			for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
			{
#ifdef DEBUG_AI
				auto card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setCardPosition(x + (i * PLAYER_CARD_OFFSET), y);
				card->setVisible(true);
				card->addEvents();

#else 
				auto cardCover = m_cardDeck->getCardCover(m_inHandCards[i]);
				cardCover->setVisible(true);
				cardCover->setPosition(x + (i * PLAYER_CARD_OFFSET), y);
				auto card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setPosition(x + (i * PLAYER_CARD_OFFSET), y);

#endif
				setPlayerName("North");
			}
			break;
		case PLAYER_POSITION::SOUTH:
			x = size.width / 2.5;
			y = size.height / 13;
			for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
			{
				Card* card = m_cardDeck->getCard(m_inHandCards[i]);
				card->setCardPosition(x + (i * PLAYER_CARD_OFFSET), y);
				card->setVisible(true);
				card->enableTouch(true);
			}
			setPlayerName("Shakuni");
			break;
	}

}



int Player::getPlayerType()
{
	return m_playerType;
}
int Player::throwCard()
{
	collectPlayableCards();
	if (m_playableCards.size() == 0)
	{
		return TURN_PASS;
	}
	return Card::getSelectedCard();
}

void Player::cardConsumed(int selectedCard)
{
	for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
	{
		if (m_inHandCards[i] == selectedCard)
		{
			m_numOfInHandCards--;
			m_inHandCards[i] = INVALID_CARD;
			m_boardState->consumeCard(selectedCard);
		}
	}
}


void Player::collectPlayableCards()
{

	m_playableCards.erase(m_playableCards.begin(), m_playableCards.end());
	m_playablecadsNextDist.erase(m_playablecadsNextDist.begin(), m_playablecadsNextDist.end());
	for (int index = 0; index < MAX_IN_HAND_CARDS; index++)
	{
	
		if (m_boardState->isValidCard(m_inHandCards[index]))
		{
			m_playableCards.push_back(m_inHandCards[index]);
			m_playablecadsNextDist.push_back(m_nextCardDistance[index]);
		}
		if (m_inHandCards[index] == HEART_7)
		{// Forcing to play HEART_7 card
			m_playableCards.clear();
			m_playableCards.push_back(m_inHandCards[index]);
			break;
		}
	}
}