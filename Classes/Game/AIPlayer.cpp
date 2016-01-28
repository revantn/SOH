#include "AIPlayer.h"
#include "BoardState.h"

#ifdef DEBUG_AI
cocos2d::LabelTTF *AIPlayer::debugLable = NULL;
std::string AIPlayer::m_debugInfo;
#endif 

int AIPlayer::m_selectedCard = INVALID_CARD;
int AIPlayer::m_selectedCardPriority = INVALID_CARD;
AIPlayer::AIPlayer(int id, int playerType):Player(id, playerType)
{ 
}

bool AIPlayer::init(int *cards, int *isMyturn)
{
	Player::init(cards, isMyturn);
#ifdef DEBUG_AI
	m_debugInfo = "AI Info : ";
	showAIDebug();
#endif
	for (int i = 0; i < SUIT_SIZE; i++)
	{
		m_nextCardDistance[i] = INVALID_CARD;
	}
	for (int i = 0; i < NUMBER_OF_SUITS; i++)
	{
		for (int j = 0; j < lowNhigh_max; j++)
		{
			m_extreamCards[i][j] = INVALID_CARD;
		}
	}
	calculateNextCardDistance();
	extreamCardsInhand();
	return true;
}
int AIPlayer::throwCard()
{
#ifdef DEBUG_AI
	m_debugInfo = "AI Info : \n";
	m_debugInfo += "next Card Dist: ";
	for (int i = 0; i < SUIT_SIZE; i++)
	{
		m_debugInfo += std::to_string(m_nextCardDistance[i]) + " ";
	}
	m_debugInfo += "\n";
#endif

	collectPlayableCards();

#ifdef DEBUG_AI
	m_debugInfo += "Playable Cards: ";
	for (int i = 0; i < m_playableCards.size(); i++)
	{
		m_debugInfo += std::to_string(m_playableCards[i]) + " ";
	}
	m_debugInfo += "\n";
#endif

	int card = choosePerfectCard();
#ifdef DEBUG_AI
	m_debugInfo += "Selected Card: " + std::to_string(card);
	showAIDebug();
#endif

	return 	card;
}




void AIPlayer::calculateNextCardDistance()
{
	m_nextCardDistance[0] = INVALID_CARD;
	for (int i = 0; i < MAX_IN_HAND_CARDS - 1; i++)
	{
		switch (m_inHandCards[i]/SUIT_SIZE)
		{
		case HEARTS:
			if ((m_inHandCards[i + 1] / SUIT_SIZE) == HEARTS)
			{
				if (m_inHandCards[i] < HEART_7 && m_inHandCards[i + 1] <= HEART_7)
				{
					m_nextCardDistance[i + 1] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
				else if (m_inHandCards[i] > HEART_7 && m_inHandCards[i + 1] >= HEART_7)
				{
					m_nextCardDistance[i] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
			}
			break;
		case DIAMONDS:
			if ((m_inHandCards[i + 1] / SUIT_SIZE) == DIAMONDS)
			{
				if (m_inHandCards[i] < DIAMOND_7 && m_inHandCards[i + 1] <= DIAMOND_7)
				{
					m_nextCardDistance[i + 1] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
				else if (m_inHandCards[i] > DIAMOND_7 && m_inHandCards[i + 1] >= DIAMOND_7)
				{
					m_nextCardDistance[i] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
			}
			break;
		case SPADES:
			if ((m_inHandCards[i + 1] / SUIT_SIZE) == SPADES)
			{
				if (m_inHandCards[i] < SPADE_7 && m_inHandCards[i + 1] <= SPADE_7)
				{
					m_nextCardDistance[i + 1] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
				else if (m_inHandCards[i] > SPADE_7 && m_inHandCards[i + 1] >= SPADE_7)
				{
					m_nextCardDistance[i] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
			}
			break;
		case CLUBS:
			if ((m_inHandCards[i + 1] / SUIT_SIZE) == CLUBS)
			{
				if (m_inHandCards[i] < CLUB_7 && m_inHandCards[i + 1] <= CLUB_7)
				{
					m_nextCardDistance[i + 1] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
				else if (m_inHandCards[i] > CLUB_7 && m_inHandCards[i +1] >= CLUB_7)
				{
					m_nextCardDistance[i] = m_inHandCards[i + 1] - m_inHandCards[i];
				}
			}
			break;

		default:
			break;
		}
	}
}


void AIPlayer::extreamCardsInhand()
{
	for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
	{
		switch (m_inHandCards[i] / SUIT_SIZE)
		{
		case HEARTS:
			if (m_inHandCards[i] <= HEART_7 )
			{
				if (m_extreamCards[HEARTS][low] == INVALID_CARD || m_extreamCards[HEARTS][low] > m_inHandCards[i])
					m_extreamCards[HEARTS][low] = m_inHandCards[i];
			}
			else
			{
				m_extreamCards[HEARTS][high] = m_inHandCards[i];
			}
			break;
		case DIAMONDS:
			if (m_inHandCards[i] <= DIAMOND_7)
			{
				if (m_extreamCards[DIAMONDS][low] == INVALID_CARD || m_extreamCards[DIAMONDS][low] > m_inHandCards[i])
					m_extreamCards[DIAMONDS][low] = m_inHandCards[i];
			}
			else
			{
				m_extreamCards[DIAMONDS][high] = m_inHandCards[i];
			}
			break;
		case SPADES:
			if (m_inHandCards[i] <= SPADE_7)
			{
				if (m_extreamCards[SPADES][low] == INVALID_CARD || m_extreamCards[SPADES][low] > m_inHandCards[i])
					m_extreamCards[SPADES][low] = m_inHandCards[i];
			}
			else
			{
				m_extreamCards[SPADES][high] = m_inHandCards[i];
			}
			break;
		case CLUBS:
			if (m_inHandCards[i] <= CLUB_7)
			{
				if (m_extreamCards[CLUBS][low] == INVALID_CARD || m_extreamCards[CLUBS][low] > m_inHandCards[i])
					m_extreamCards[CLUBS][low] = m_inHandCards[i];
			}
			else
			{
				m_extreamCards[CLUBS][high] = m_inHandCards[i];
			}
			break;


		}
	}
}


int AIPlayer::getNextCardDistance(int card)
{
	for (int i = 0; i < MAX_IN_HAND_CARDS; i++)
	{
		if (m_inHandCards[i] == card)
		{
			return m_nextCardDistance[i];
		}
	}
	return INVALID_CARD;
}

void AIPlayer::setSectedCard(int card, int prioprity)
{
	if (m_selectedCardPriority <= prioprity)
	{
		m_selectedCard = card;
		m_selectedCardPriority = prioprity;
	}

}
int AIPlayer::choosePerfectCard()
{
	m_selectedCard = TURN_PASS;
	m_selectedCardPriority = INVALID_CARD;
	if (m_playableCards.size() == 0)
	{
		return TURN_PASS;
	}
	if (m_playableCards.size() == 1)
	{
		return m_playableCards[0];
	}
	for (int i = 0; i < m_playableCards.size(); i++)
	{
		
		int nextCardDistance = 0;
		switch (m_playableCards[i] / SUIT_SIZE)
		{
		case HEARTS:
			if (m_playableCards[i] <= HEART_7 && m_playableCards[i] != HEART_KING && m_extreamCards[HEARTS][low] == HEART_ACE)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= HEART_7 && m_playableCards[i] != HEART_ACE &&  m_extreamCards[HEARTS][high] == HEART_KING)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= HEART_7 && m_playableCards[i] != HEART_QUEEN && m_extreamCards[HEARTS][high] == HEART_QUEEN)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] <= HEART_7 && m_playableCards[i] != HEART_2 &&  m_extreamCards[HEARTS][low] == HEART_2)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			if (m_selectedCardPriority <= m_playablecadsNextDist[i])
			{
				setSectedCard(m_playableCards[i], m_playablecadsNextDist[i]);
			}

			break;
		case DIAMONDS:
			if (m_playableCards[i] <= DIAMOND_7 && m_playableCards[i] != DIAMOND_KING && m_extreamCards[DIAMONDS][low] == DIAMOND_ACE)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= DIAMOND_7 && m_playableCards[i] != DIAMOND_ACE &&  m_extreamCards[DIAMONDS][high] == DIAMOND_KING)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= DIAMOND_7 && m_playableCards[i] != DIAMOND_QUEEN && m_extreamCards[DIAMONDS][high] == DIAMOND_QUEEN)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] <= DIAMOND_7 && m_playableCards[i] != DIAMOND_2 &&  m_extreamCards[DIAMONDS][low] == DIAMOND_2)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			if (m_selectedCardPriority <= m_playablecadsNextDist[i])
			{
				setSectedCard(m_playableCards[i], m_playablecadsNextDist[i]);
			}

			break;
		case SPADES:
			if (m_playableCards[i] <= SPADE_7 && m_playableCards[i] != SPADE_KING && m_extreamCards[SPADES][low] == SPADE_ACE)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= SPADE_7 && m_playableCards[i] != SPADE_ACE &&  m_extreamCards[SPADES][high] == SPADE_KING)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= SPADE_7 && m_playableCards[i] != SPADE_QUEEN && m_extreamCards[SPADES][high] == SPADE_QUEEN)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] <= SPADE_7 && m_playableCards[i] != SPADE_2 &&  m_extreamCards[SPADES][low] == SPADE_2)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			if (m_selectedCardPriority <= m_playablecadsNextDist[i])
			{
				setSectedCard(m_playableCards[i], m_playablecadsNextDist[i]);
			}
			break;
		case CLUBS:
			if (m_playableCards[i] <= CLUB_7 && m_playableCards[i] != CLUB_KING && m_extreamCards[CLUBS][low] == CLUB_ACE)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= CLUB_7 && m_playableCards[i] != CLUB_ACE &&  m_extreamCards[CLUBS][high] == CLUB_KING)
			{
				setSectedCard(m_playableCards[i], HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] >= CLUB_7 && m_playableCards[i] != CLUB_QUEEN && m_extreamCards[CLUBS][high] == CLUB_QUEEN)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			else if (m_playableCards[i] <= CLUB_7 && m_playableCards[i] == CLUB_2 &&  m_extreamCards[CLUBS][low] == CLUB_2)
			{
				setSectedCard(m_playableCards[i], NEXT_HIGH_PRIOPRITY);
			}
			if (m_selectedCardPriority <= m_playablecadsNextDist[i])
			{
				setSectedCard(m_playableCards[i], m_playablecadsNextDist[i]);
			}
			break;
		}

	}
	return m_selectedCard;
}

#ifdef DEBUG_AI
void AIPlayer::showAIDebug()
{
	if (!debugLable)
	{
		cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
		debugLable = cocos2d::LabelTTF::create(m_debugInfo, "font/arial.ttf", 12);
		debugLable->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
		debugLable->setPosition(size.width * 0.1, size.height * 0.8);
		this->addChild(debugLable);
	} 
	debugLable->setString(m_debugInfo);
}
#endif