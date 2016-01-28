#ifndef _CARDDECK_H_
#define _CARDDECK_H_

#include <vector>
#include "Card.h"
#include "CardShuffler.h"
#include "GameConstants.h"

class Board;
class CardDeck
{
public:
	static CardDeck* getInstance();
	bool init(Board *board);
	void reinit();
	void shuffleDeck();
	Card* getCard(int value);
	cocos2d::Sprite * getCardCover(int value);
	int* getCardDeck();
private:
	CardDeck(){}
	Board* m_board;
	static CardDeck* m_instance;
	std::vector<Card*> m_cardSprites;
	std::vector<cocos2d::Sprite *> m_cardCoverSprites;
	CardShuffler * m_cardShuffler;
	int m_cardDeck[CARD_DECK::DECK_SIZE];


};
#endif