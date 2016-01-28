#include "Board.h"
#include "CardDeck.h"
#include "AIPlayer.h"
#include "BoardState.h"
#include "GameConstants.h"
#include "FileOperation.h"
USING_NS_CC;

cocos2d::Scene *Board::m_scene;
Board *Board::m_board = NULL;
bool Board::s_enablePlayerCardTouch = false;
#ifdef USE_SCALE_FACTOR
float Board::m_scaleFactorX = 1.0;
float Board::m_scaleFactorY = 1.0;
#endif

Scene* Board::createScene()
{
	// 'scene' is an autorelease object
	m_scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Board::create();

	// add layer as a child to scene
	m_scene->addChild(layer);
	
	

	// return the scene
	return m_scene;
}

Board* Board::getBoardInstance()
{
	return m_board;
}

bool Board::init()
{
	CCLOG("Board init");
	if (!CCLayerColor::initWithColor(ccc4(10, 93, 41, 255)))
		return false;
	m_board = this;
	m_playerTurn = -1;
	m_wait = 0;
	m_gameOver = false;
	this->scheduleUpdate();
	setZOrder(-500);
	initCardHolders();
	m_boardState = BoardState::getBoardState();
	m_boardState->init();
	m_cardDeck = CardDeck::getInstance();
	m_cardDeck->init(this);
	m_cardDeck->shuffleDeck(); 
	initHighLighter();
	initPlayers();
	setAnchorPoint(cocos2d::Vec2(0, 0));
	m_menu =new SubMenu();
	m_menu->init(m_scene);
#ifdef	DEBUG_AI
	m_debugSwitchPlayer = false;
	Size size = cocos2d::Director::getInstance()->getWinSize();
	m_debugSwitchButton = cocos2d::Sprite::create("sprite/switch_turn.png");
	m_debugSwitchButton->setPosition(size.width * 0.93, size.height * 0.96);
	m_debugSwitchButton->setScale(0.3);
	addChild(m_debugSwitchButton);
#endif
	/*
	// Save Test
	char test [50]="faaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" ;
	char out[50] = " ";
	FileOperation::saveFile(test);
	FileOperation::readFile(out);
	auto label1 = cocos2d::LabelTTF::create(out, "font/arial.ttf", 18);
	label1->setPosition(100, 200);
	label1->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
	this->addChild(label1);*/

	return true;
}

void Board::initPlayers()
{
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (i == PLAYER_POSITION::SOUTH)
		{
			m_players[i] = new Player(i, HUMAN_PLAYER);
			m_players[i]->init(m_cardDeck->getCardDeck() + (i * MAX_IN_HAND_CARDS), &m_playerTurn);
		}
		else
		{
			m_players[i] = new AIPlayer(i, AI_PLAYER);
			m_players[i]->init(m_cardDeck->getCardDeck() + (i * MAX_IN_HAND_CARDS), &m_playerTurn);
		}
				addChild(m_players[i]);
	}
	setPlayerTurnHighlight(m_playerTurn);
	if (m_players[m_playerTurn]->getPlayerType() == HUMAN_PLAYER)
		Board::s_enablePlayerCardTouch = true;

}


void Board::initCardHolders()
{
	char path[256];
	cocos2d::Size size = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	sprintf(path, "%s/%s", "board", "HEART_EMPTY.tga");
	m_cardHolder[HEARTS]= Card::createCard(path);

	sprintf(path, "%s/%s", "board", "DIAMOND_EMPTY.tga");
	m_cardHolder[DIAMONDS]= Card::createCard(path);

	sprintf(path, "%s/%s", "board", "SPADES_EMPTY.tga");
	m_cardHolder[SPADES] = Card::createCard(path);

	sprintf(path, "%s/%s", "board", "CLUB_EMPTY.tga");
	m_cardHolder[CLUBS] = Card::createCard(path);
	for (int i = 0; i < NUMBER_OF_SUITS; i++)
	{
		m_cardHolder[i]->setPosition((size.width / 2.75) + (i * 100), BOARD_CARD_HOLDER_Y);
		m_cardHolder[i]->setFlipY(true);
		addChild(m_cardHolder[i], 0);
	}
	for (int i = 0; i < SUIT_SIZE; i++)
	{
		m_heartsHolder[i] = INVALID_CARD;
		m_diamondsHolder[i] = INVALID_CARD;
		m_spadesHolder[i] = INVALID_CARD;
		m_clubsHolder[i] = INVALID_CARD;
	}
}

void Board::changePlayerTurn()
{
#ifdef DEBUG_AI
	m_debugSwitchPlayer = false;
#endif
	if (m_players[m_playerTurn]->getNumberofInhandCards() == 0)
	{
		m_gameOver = true;
		return;
	}
	m_greenHighLight[m_playerTurn]->setVisible(false);
	m_playerTurn = (m_playerTurn + 1) % MAX_PLAYERS;
	if (m_players[m_playerTurn]->getPlayerType() == HUMAN_PLAYER)
	{
		s_enablePlayerCardTouch = true;
	}
	else
	{
		s_enablePlayerCardTouch = false;
	}
	m_greenHighLight[m_playerTurn]->setVisible(true);
}
void Board::update(float dt)
{
	if (m_menu->isMenuActive())
	{
		return;
	}
	else if (m_gameOver && m_boardState->getState() == ACTIVE )
	{
		m_menu->showMenu(NOTIFICATION_MENU, true);
		m_boardState->setState(IDLE);
		return;
	}

	auto selectedCard = m_players[m_playerTurn]->throwCard();
	if (selectedCard == INVALID_CARD)
		return;
#ifdef DEBUG_AI
	if ((selectedCard == TURN_PASS) && (m_debugSwitchPlayer || m_players[m_playerTurn]->getPlayerType() == HUMAN_PLAYER))
#else
	if (selectedCard == TURN_PASS)
#endif
	{
		changePlayerTurn();
		return;
	}
	if ((selectedCard != TURN_PASS || m_boardState->isValidCard(selectedCard)) && (m_players[m_playerTurn]->getPlayerType() == AI_PLAYER))
	{
		if (m_players[m_playerTurn]->getPlayerType() == AI_PLAYER)
		{
			m_wait += dt;
#ifdef DEBUG_AI
			if ( m_debugSwitchPlayer)
#else
			if (m_wait > AI_PLAYER_WAIT_TIME)
#endif
			{
				placeCardOnBoard(selectedCard);
				m_players[m_playerTurn]->cardConsumed(selectedCard);
				Card::resetSelectedCard();
				changePlayerTurn();
				m_wait = 0;
			}
			
		}
		
		Card::resetSelectedCard();
	}
	else  if (m_players[m_playerTurn]->getPlayerType() == HUMAN_PLAYER)
	{
		if (isUserCardValid(selectedCard))
		{
			m_players[m_playerTurn]->cardConsumed(selectedCard);
			Card::resetSelectedCard();
			changePlayerTurn();
			return;
		}
		auto card = m_cardDeck->getCard(selectedCard);
		card->returnToPosition();
		card->resetSelectedCard();

	}

}
bool Board::isUserCardValid(int card)
{
	auto selectedCard = m_cardDeck->getCard(card);
	auto pos = selectedCard->getPosition();
	cocos2d::Rect rect;
	int	y;
	
	if (m_boardState->isValidCard(card))
	{
		switch (card / SUIT_SIZE)
		{
		case HEARTS:
			for (int i = 0; i < SUIT_SIZE; i++)
			{
				if (m_heartsHolder[i] == INVALID_CARD && card != HEART_7)
					continue;
				Card *cardSprite = NULL;

				if (card == HEART_7)
					cardSprite =m_cardHolder[HEARTS];
				else 
					cardSprite = m_cardDeck->getCard(m_heartsHolder[i]);

				rect = cardSprite->getBoundingBox();
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);

				if (rect.containsPoint(pos))
				{
					selectedCard->setCardPosition(m_cardHolder[HEARTS]->getPositionX(), y);
					selectedCard->enableTouch(false);
					m_heartsHolder[(card % SUIT_SIZE)] = card;
					return true;// to indicate that we have consumed it.
				}
			}
			break;
		case DIAMONDS:
			for (int i = 0; i < SUIT_SIZE; i++)
			{
				if (m_diamondsHolder[i] == INVALID_CARD && card != DIAMOND_7)
					continue;
				Card *cardSprite = NULL;

				if (card == DIAMOND_7)
					cardSprite = m_cardHolder[DIAMONDS];
				else
					cardSprite = m_cardDeck->getCard(m_diamondsHolder[i]);
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);

				rect = cardSprite->getBoundingBox();
				if (rect.containsPoint(pos))
				{
					selectedCard->setCardPosition(m_cardHolder[DIAMONDS]->getPositionX(), y);
					selectedCard->enableTouch(false);
					m_diamondsHolder[(card % SUIT_SIZE)] = card;
					return true;// to indicate that we have consumed it.
				}
			}
			break;
		case SPADES:
			for (int i = 0; i < SUIT_SIZE; i++)
			{
				if (m_spadesHolder[i] == INVALID_CARD && card != SPADE_7)
					continue;
				Card *cardSprite = NULL;

				if (card == SPADE_7)
					cardSprite = m_cardHolder[SPADES];
				else
					cardSprite = m_cardDeck->getCard(m_spadesHolder[i]);

				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);

				rect = cardSprite->getBoundingBox();
				if (rect.containsPoint(pos))
				{
					
					selectedCard->setCardPosition(m_cardHolder[SPADES]->getPositionX(), y);
					selectedCard->enableTouch(false);
					m_spadesHolder[(card % SUIT_SIZE)] = card;
					return true;// to indicate that we have consumed it.
					
				}
			}
			break;
		case CLUBS:
			for (int i = 0; i < SUIT_SIZE; i++)
			{
				if (m_clubsHolder[i] == INVALID_CARD && card != CLUB_7)
					continue;
				Card *cardSprite = NULL;

				if (card == CLUB_7)
					cardSprite = m_cardHolder[CLUBS];
				else
					cardSprite = m_cardDeck->getCard(m_clubsHolder[i]);

				rect = cardSprite->getBoundingBox();
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);

				if (rect.containsPoint(pos))
				{
					
					selectedCard->setCardPosition(m_cardHolder[CLUBS]->getPositionX(), y);
					selectedCard->enableTouch(false);
					m_clubsHolder[(card % SUIT_SIZE)] = card;
					return true;// to indicate that we have consumed it.
					
				}
			}
			break;
		default:
			selectedCard->returnToPosition();
			break;
		}
	}
	return false;
}
void Board::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


Board::~Board()
{
	//FileOperation::saveFile(NULLP);
	delete m_boardState;
}

void Board::setCardZorder(Node* node)
{
	node->setLocalZOrder(node->getLocalZOrder() - 200);
}

void Board::startActionSequence(Card *selectedCard, float x, float y)
{
	auto callback = CallFunc::create(CC_CALLBACK_0(Board::setCardZorder, this, selectedCard));
	selectedCard->setLocalZOrder(selectedCard->getLocalZOrder() + 200);
	auto moveTo = MoveTo::create(1, Vec2(x, y));
	auto seq = Sequence::create(moveTo, callback, NULL);
	selectedCard->runAction(seq);
}
void Board::placeCardOnBoard(int card)
{
	auto selectedCard = m_cardDeck->getCard(card);
	cocos2d::Rect rect;
	int	y;
	if (m_boardState->isValidCard(card))
	{
		m_cardDeck->getCardCover(card)->setVisible(false);
		selectedCard->setVisible(true);
		selectedCard->enableTouch(false);
		switch (card / SUIT_SIZE)
		{
		case HEARTS:
			{
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);
				startActionSequence(selectedCard, m_cardHolder[HEARTS]->getPositionX(), y);
				m_heartsHolder[(card % SUIT_SIZE)] = card;
			}
			break;
		case DIAMONDS:
			{
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);
				startActionSequence(selectedCard, m_cardHolder[DIAMONDS]->getPositionX(), y);
				m_diamondsHolder[(card % SUIT_SIZE)] = card;
			}
			break;
		case SPADES:
			{
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);
				startActionSequence(selectedCard, m_cardHolder[SPADES]->getPositionX(), y);
				m_spadesHolder[(card % SUIT_SIZE)] = card;
			}
			break;
		case CLUBS:
			{
				y = BOARD_Y + ((card % SUIT_SIZE) * BOARD_CARD_OFFSET);
				startActionSequence(selectedCard, m_cardHolder[CLUBS]->getPositionX(), y);
				m_clubsHolder[(card % SUIT_SIZE)] = card;
			}
			break;

		}
	}
}

void Board::initHighLighter()
{
	//cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::Size size = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	for (int index = 0; index < MAX_PLAYERS; index++)
	{
		m_greyHighLight[index] = Sprite::create("sprite/gray_highlighter.png");
		m_greenHighLight[index] =  Sprite::create("sprite/green_highlighter.png");
		switch (index)
		{
		case PLAYER_POSITION::EAST:
			m_greyHighLight[index]->setPosition(size.width * 0.93, size.height * 0.19);
			m_greenHighLight[index]->setPosition(size.width * 0.93, size.height * 0.19);
			break;
		case PLAYER_POSITION::NORTH:
			m_greyHighLight[index]->setPosition(size.width * 0.8, size.height * 0.85);
			m_greenHighLight[index]->setPosition(size.width * 0.8, size.height * 0.85);
			break;
		case PLAYER_POSITION::WEST:
			m_greyHighLight[index]->setPosition(size.width * 0.13, size.height * 0.85);
			m_greenHighLight[index]->setPosition(size.width * 0.13, size.height * 0.85);
			break;
		case PLAYER_POSITION::SOUTH:
			m_greyHighLight[index]->setPosition(size.width * 0.32, size.height * 0.12);
			m_greenHighLight[index]->setPosition(size.width * 0.32, size.height * 0.12);
			break;

		default:
			break;
		}
		m_greenHighLight[index]->setVisible(false);
		this->addChild(m_greyHighLight[index]);
		this->addChild(m_greenHighLight[index]);
	}
	m_menuButton = Sprite::create("sprite/menu.png");
	m_menuButton->setPosition(size.width * 0.05, size.height * 0.94);

	m_exitBtn = Sprite::create("sprite/exit.png");
	m_exitBtn->setPosition(size.width * 0.95, size.height * 0.94);

	addEvents();
	addChild(m_menuButton);
	addChild(m_exitBtn);

}
void Board::reinitHighLighter()
{
	for (int playerIndex = 0; playerIndex < MAX_PLAYERS; ++playerIndex)
	{
		m_greenHighLight[playerIndex]->setVisible(false);
	}
}


void Board::setPlayerTurnHighlight(int playerPoistion)
{
	m_greenHighLight[playerPoistion]->setVisible(true);
}

void Board::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (m_menu->isMenuActive())
		{
			return false;
		}
		cocos2d::Vec2 p = touch->getLocation();
		cocos2d::Rect rect = m_menuButton->getBoundingBox();

		if (rect.containsPoint(p))
		{
			m_menuButton->setScale(1.05);
		}

		rect = m_exitBtn->getBoundingBox();
		if (rect.containsPoint(p))
		{
			m_exitBtn->setScale(1.05);
		}
#ifdef DEBUG_AI
		rect = m_debugSwitchButton->getBoundingBox();
		if (rect.containsPoint(p))
		{
			m_debugSwitchButton->setScale(0.35);
		}
#endif

		return true;
	};

	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (m_menu->isMenuActive())
		{
			return ;
		}
		cocos2d::Vec2 p = touch->getLocation();
		cocos2d::Rect rect = m_menuButton->getBoundingBox();
		m_menuButton->setScale(1);
		m_exitBtn->setScale(1);
		if (rect.containsPoint(p))
		{
			m_menu->showMenu(OPTIONS_MENU, true);
		}

		rect = m_exitBtn->getBoundingBox();
		if (rect.containsPoint(p))
		{
			exit(0);
		}
#ifdef DEBUG_AI
		m_debugSwitchButton->setScale(0.30);
		rect = m_debugSwitchButton->getBoundingBox();
		if (rect.containsPoint(p))
		{
			m_debugSwitchPlayer = true;
		}
#endif
		
	};
	listener->onTouchMoved = [=](Touch* touch, cocos2d::Event* event)
	{
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


void Board::reinit()
{
	m_cardDeck->reinit();
	reinitHighLighter();
	// Player reinit...
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		m_players[i]->init(m_cardDeck->getCardDeck() + (i * MAX_IN_HAND_CARDS), &m_playerTurn);
		if (m_players[m_playerTurn]->getPlayerType() == HUMAN_PLAYER)
		{
			Board::s_enablePlayerCardTouch = true;
		}

	}
	
	setPlayerTurnHighlight(m_playerTurn);
	//BoardState reinit...
	m_boardState->init();

	for (int i = 0; i < SUIT_SIZE; i++)
	{
		m_heartsHolder[i] = INVALID_CARD;
		m_diamondsHolder[i] = INVALID_CARD;
		m_spadesHolder[i] = INVALID_CARD;
		m_clubsHolder[i] = INVALID_CARD;
	}

	m_gameOver = false;
	m_menu->showMenu(ALL_MENU, false);
}
void Board::restartGame()
{
	reinit();
}

void Board::newGame()
{
	m_cardDeck->shuffleDeck();
	reinit();
}
#ifdef USE_SCALE_FACTOR
void Board::setScaleFactors(float x, float y)
{
	m_scaleFactorX = x;
	m_scaleFactorY = y;
}
#endif

Player *Board::getCurrentPlayer()
{
	return m_players[m_playerTurn];
}
