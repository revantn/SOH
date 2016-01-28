#include "CardDeck.h"
#include "Board.h"

CardDeck* CardDeck::m_instance = NULL;

CardDeck* CardDeck::getInstance()
{
	if (m_instance == NULL)
		m_instance = new CardDeck();
	return m_instance;
}



bool CardDeck::init(Board *board)
{
	m_board = board;
	char path[256];
	for (int i = 0; i < CARD_DECK::DECK_SIZE; i++)
	{
		int tempCardsNum = (i + 1); 

		switch (i / SUIT_SIZE)
		{
			case SUITS::HEARTS :
				sprintf(path, "%s/%s_%d.png", "cards", "HEART", tempCardsNum);
					break;
			case SUITS::DIAMONDS:
					sprintf(path, "%s/%s_%d.png", "cards", "DIAMOND", tempCardsNum);
					break;
			case SUITS::SPADES:
					sprintf(path, "%s/%s_%d.png", "cards", "SPADES", tempCardsNum);
					break;
			case SUITS::CLUBS:
					sprintf(path, "%s/%s_%d.png", "cards", "CLUB", tempCardsNum);
					break;
			default:
					return false;
		}
		auto *cardFace = Card::createCard(path);
		m_cardSprites.push_back(cardFace);
		cardFace->setCardValue(i);
		m_board->addChild(cardFace);
		cardFace->setVisible(false);
		cardFace->addEvents();
		cardFace->enableTouch(false);
		cardFace->setLocalZOrder(i);
		sprintf(path, "%s/%s", "cards", "COVER.png");
		auto *cardCover = cocos2d::Sprite::create(path);
#ifdef USE_SCALE_FACTOR
		cardCover->setScale(Board::getScaleFactorX(), Board::getScaleFactorY());
#endif
		m_board->addChild(cardCover);
		cardCover->setVisible(false);
		m_cardCoverSprites.push_back(cardCover);
	}
	
	m_cardShuffler->init();
	
	return true;
}

void CardDeck::reinit()
{
	for (int card = 0; card < CARD_DECK::DECK_SIZE; ++card)
	{
		m_cardSprites[card]->setVisible(false);
		m_cardSprites[card]->enableTouch(false);
		m_cardCoverSprites[card]->setVisible(false);
	}
}


Card* CardDeck::getCard(int value)
{
	return m_cardSprites[value];
}

cocos2d::Sprite* CardDeck::getCardCover(int value)
{
	return m_cardCoverSprites[value];
}

int* CardDeck::getCardDeck()
{
	return m_cardDeck;
}
void CardDeck::shuffleDeck()
{
	m_cardShuffler->shuffleCards(m_cardDeck);
}