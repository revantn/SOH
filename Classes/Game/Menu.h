#ifndef _MENU_H_
#define _MENU_H_
#include "cocos2d.h"
#include <string>	
	class Board;

	class SubMenu
	{
	public:
		bool init(cocos2d::Scene *scene);
		void showMenu(int menu, bool value);
		bool isMenuActive();

		
	private:
		cocos2d::LayerColor *m_layerOption;
		cocos2d::LayerColor *m_layerNotification;
		cocos2d::Sprite *m_backBtn;

		cocos2d::Sprite *m_menuBG;

		cocos2d::Sprite *m_restartBtn;
		cocos2d::Sprite *m_newGameBtn;

		cocos2d::Sprite *m_notificationBG;
		cocos2d::Sprite *m_resultRestartGameBtn;
		cocos2d::Sprite *m_resultNewGameBtn;

		float m_scaleFactorX;
		float m_scaleFactorY;
		Board *m_board;
		void addEvents();
		std::string m_notification;
		cocos2d::LabelTTF *m_notificationLabel;
		
	};



#endif