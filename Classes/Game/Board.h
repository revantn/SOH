#ifndef _BOARD_H_
#define _BOARD_H_

#include <cocos2d.h>
#include "Player.h"
#include "Menu.h"
#include "GameConstants.h"
class CardDeck; // Froward declaration
class Card; // Froward g
class BoardState; // Froward declaration
class Board :public cocos2d::LayerColor
{

public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	static bool s_enablePlayerCardTouch;
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	void initPlayers();
	void initCardHolders();
	static Board* getBoardInstance();
	// a GLselector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void update(float dt) override;
	bool isUserCardValid(int card);
	void restartGame();
	void newGame();
	Player *getCurrentPlayer();
#ifdef USE_SCALE_FACTOR
	static void setScaleFactors(float x, float y);
	static float getScaleFactorX()  { return m_scaleFactorX; }
	static float getScaleFactorY()  { return m_scaleFactorY; }
#endif
	~Board();
	// implement the "static create()" method manually
	CREATE_FUNC(Board);
	
private:
	bool m_gameOver;
	static Board *m_board;
	CardDeck *m_cardDeck;
	Player *m_players[MAX_PLAYERS];
	Card  *m_cardHolder[NUMBER_OF_SUITS];
	BoardState * m_boardState;
	SubMenu * m_menu;
#ifdef DEBUG_AI
	cocos2d::Sprite *m_debugSwitchButton;
	bool m_debugSwitchPlayer;
#endif
	static cocos2d::Scene *m_scene;
	float m_wait;
	int m_playerTurn;
	int m_heartsHolder[SUIT_SIZE];
	int m_diamondsHolder[SUIT_SIZE];
	int m_spadesHolder[SUIT_SIZE];
	int m_clubsHolder[SUIT_SIZE];
	void placeCardOnBoard(int card);
	void setCardZorder(Node* node);
	void startActionSequence(Card *selectedCard, float x, float y);
	void initHighLighter();
	void reinitHighLighter();
	void setPlayerTurnHighlight(int playerPoistion);
	cocos2d::Sprite *m_greyHighLight[MAX_PLAYERS];
	cocos2d::Sprite *m_greenHighLight[MAX_PLAYERS];
	void changePlayerTurn();
	cocos2d::Sprite *m_menuButton;
	cocos2d::Sprite *m_exitBtn;
	void reinit();
	void addEvents();
#ifdef USE_SCALE_FACTOR
	static float m_scaleFactorX;
	static float m_scaleFactorY;
#endif

};
#endif



