#ifndef _CARD_H_
#define _CARD_H_
#include <cocos2d.h>
class Card:public cocos2d::Sprite
{
private:
	int m_faceValue;
	static int s_SelectedCard;
	cocos2d::Vec2 m_position;
	bool handleTouchEvents;
public:
	static int getSelectedCard();
	static void resetSelectedCard( );
	static Card* createCard(const char*);
	Card(int id = -1);
	void setCardValue(int value);
	void addEvents();
	void removeEvents();
	
	void setCardPosition(int x, int y);
	void returnToPosition();
	bool onTouchDown(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchUp(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMove(cocos2d::Touch* touch, cocos2d::Event* event);

	void enableTouch(bool value);
};

#endif