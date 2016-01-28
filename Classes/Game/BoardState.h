#ifndef _BOARDSTATE_H_
#define _BOARDSTATE_H_
#include "GameConstants.h"


enum STATE
{
	ACTIVE,
	IDLE
};
class BoardState
{
public:
	void init();
	bool isValidCard(int card);
	void consumeCard(int card);
	static BoardState* getBoardState();
	void setState(int state);
	int getState();

private:
	BoardState();
	static BoardState *m_boardState;
	int m_state;

	int m_head[NUMBER_OF_SUITS];
	int m_tail[NUMBER_OF_SUITS];
	
};
#endif