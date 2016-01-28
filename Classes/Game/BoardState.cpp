#include "BoardState.h"
#include "2d\CCSprite.h"
BoardState *BoardState::m_boardState = 0;

void BoardState::init()
{
	for (int i = 0; i < NUMBER_OF_SUITS; i++)
	{
		m_head[i] = INVALID_CARD;
		m_tail[i] = INVALID_CARD;
	}
	m_state = ACTIVE;
}

void BoardState::setState(int state)
{
	m_state = state;
}

int BoardState::getState()
{
	return m_state;
}

BoardState::BoardState()
{
}
BoardState* BoardState::getBoardState()
{
	if (!m_boardState)
	{
		m_boardState = new BoardState();
	}
	return m_boardState;
}
void BoardState::consumeCard(int card)
{
	switch (card / SUIT_SIZE)
	{
	case HEARTS:
		if (card == HEART_7)
		{
			m_head[HEARTS] = HEART_7;
			m_tail[HEARTS] = HEART_7;
		}
		else if (m_tail[HEARTS] - 1 == card)
		{
			m_tail[HEARTS] = card;
		}
		else if (m_head[HEARTS] + 1 == card)
		{
			m_head[HEARTS] = card;
		}
		break;
	case DIAMONDS:
		if (card == DIAMOND_7)
		{
			m_head[DIAMONDS] = DIAMOND_7;
			m_tail[DIAMONDS] = DIAMOND_7;
		}
		else if (m_tail[DIAMONDS] - 1 == card)
		{
			m_tail[DIAMONDS] = card;
		}
		else if (m_head[DIAMONDS] + 1 == card)
		{
			m_head[DIAMONDS] = card;
		}
	case SPADES:
		if (card == SPADE_7)
		{
			m_head[SPADES] = SPADE_7;
			m_tail[SPADES] = SPADE_7;
		}
		else if (m_tail[SPADES] - 1 == card)
		{
			m_tail[SPADES] = card;
		}
		else if (m_head[SPADES] + 1 == card)
		{
			m_head[SPADES] = card;
		}
	case CLUBS:
		if (card == CLUB_7)
		{
			m_head[CLUBS] = CLUB_7;
			m_tail[CLUBS] = CLUB_7;
		}
		else if (m_tail[CLUBS] - 1 == card)
		{
			m_tail[CLUBS] = card;
		}
		else if (m_head[CLUBS] + 1 == card)
		{
			m_head[CLUBS] = card;
		}

	default:
		break;
	}
}
bool BoardState::isValidCard(int card)
{
	if (card == INVALID_CARD)return
		 false;
	if (m_head[HEARTS] == INVALID_CARD && m_tail[HEARTS] == INVALID_CARD)
	{
		if (card == HEART_7)
			return true;
		return false;
	}

	switch (card/SUIT_SIZE)
	{
	case HEARTS:
		if (card == HEART_7)
		{
			
			return true;
		}
		else if (m_tail[HEARTS] - 1 == card)
		{
			return true;
		}
		else if (m_head[HEARTS] + 1 == card)
		{
			return true;
		}
		return false;
		break;
	case DIAMONDS:
		if (card == DIAMOND_7)
		{
			
			return true;
		}
		else if (m_tail[DIAMONDS] - 1 == card)
		{
			return true;
		}
		else if (m_head[DIAMONDS] + 1 == card)
		{
			return true;
		}
		return false;
	case SPADES:
		if (card == SPADE_7)
		{
			return true;
		}
		else if (m_tail[SPADES] - 1 == card)
		{
			return true;
		}
		else if (m_head[SPADES] + 1 == card)
		{
			return true;
		}
		return false;
	case CLUBS:
		if (card == CLUB_7)
		{
			return true;
		}
		else if (m_tail[CLUBS] - 1 == card)
		{
			return true;
		}
		else if (m_head[CLUBS] + 1 == card)
		{
			return true;
		}
		return false;

	default:
		break;
	}
}


