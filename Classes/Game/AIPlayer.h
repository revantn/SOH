#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_

#include "Player.h"
#include "GameConstants.h"



class AIPlayer : public Player
{
public:
	AIPlayer(int id, int playerType);
	bool init(int *cards, int *isMyturn) override;
	int throwCard() override;
#ifdef DEBUG_AI
	static cocos2d::LabelTTF *debugLable;
	static std::string m_debugInfo;
	void showAIDebug();
#endif
	//void cardConsumed(int selectedCard) override;
	
private:
	enum lowNhigh
	{
		low,
		high,
		lowNhigh_max
	};
	void				calculateNextCardDistance();
	void				extreamCardsInhand();
	void				setSectedCard(int card, int priority);
	
	int					choosePerfectCard();
	int					getNextCardDistance(int card);
	int					m_extreamCards[NUMBER_OF_SUITS][lowNhigh_max];
	
	
	static int			m_selectedCard;
	static int			m_selectedCardPriority;
	
};



#endif