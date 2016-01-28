#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <cocos2d.h>
#include "GameConstants.h"

class CardDeck; // Forward Declaration
class BoardState; // Forward Declaration
class Player:public cocos2d::Node
{
public:
	Player(int id, int playerType );
	virtual bool init(int *cards, int *isMyturn);
	void initPlayerPosition();
	int getPlayerType();
	virtual int  throwCard();
	virtual void cardConsumed(int selectedCard);
	int getNumberofInhandCards() const;
	void setPlayerName( std::string name );
	std::string getPlayerName()const { return m_playerName; }

protected:
	char *m_name;
	int m_id;	// Position
	int m_playerType;
	int m_numOfInHandCards;
	int m_inHandCards[MAX_IN_HAND_CARDS];
	int	m_nextCardDistance[SUIT_SIZE];
	std::string m_playerName;
	cocos2d::Vec2 m_selectedCardPosition;
	CardDeck *m_cardDeck;
	std::vector<int>	m_playableCards;
	std::vector<int>	m_playablecadsNextDist;
	BoardState			*m_boardState;
	void				collectPlayableCards();
private:

};
#endif