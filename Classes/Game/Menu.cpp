#include "Menu.h"
#include "Board.h"

bool SubMenu::init(cocos2d::Scene *scene)
{
#ifdef USE_SCALE_FACTOR
	m_scaleFactorX = Board::getBoardInstance()->getScaleFactorX();
	m_scaleFactorY = Board::getBoardInstance()->getScaleFactorY();
#endif
	m_board = Board::getBoardInstance();

	//cocos2d::Size size = cocos2d::Director::getInstance()->getWinSize();
	cocos2d::Size size = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();


	m_layerOption = cocos2d::LayerColor::create(cocos2d::Color4B(112, 138, 144, 125));
	m_layerOption->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
	
#ifdef USE_SCALE_FACTOR
	m_layerOption->setScale(m_scaleFactorX, m_scaleFactorY);
#endif
	m_layerNotification = cocos2d::LayerColor::create(cocos2d::Color4B(112, 138, 144, 125));
	scene->addChild(m_layerOption, 200);
	scene->addChild(m_layerNotification, 200);
	m_layerOption->setVisible(false);
	m_layerNotification->setVisible(false);
#ifdef USE_SCALE_FACTOR
	m_layerNotification->setScale(m_scaleFactorX, m_scaleFactorY);
#endif
	
	m_backBtn = cocos2d::Sprite::create("sprite/backBtn.png");
	m_backBtn->setPosition(size.width * 0.95, size.height * 0.94);
	m_layerOption->addChild(m_backBtn);

	m_menuBG = cocos2d::Sprite::create("sprite/MenuBG.png");
	m_menuBG->setPosition(size.width / 2, size.height / 2);
	m_layerOption->addChild(m_menuBG);

	//Opetion Title
	auto label1 = cocos2d::LabelTTF::create("Options", "font/arial.ttf", 28);
	label1->setPosition(size.width * 0.08, size.height * 0.35);
	label1->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
	m_menuBG->addChild(label1);

	m_restartBtn = cocos2d::Sprite::create("sprite/restartGameButton.png");
	m_restartBtn->setPosition(size.width * 0.5, size.height * 0.6);
	m_layerOption->addChild(m_restartBtn);

	m_newGameBtn = cocos2d::Sprite::create("sprite/button.png");
	m_newGameBtn->setPosition(size.width * 0.5, size.height * 0.5);
	m_layerOption->addChild(m_newGameBtn);


	m_notificationBG = cocos2d::Sprite::create("sprite/notificationBG.png");
	m_notificationBG->setPosition(size.width / 2, size.height / 2);
	m_layerNotification->addChild(m_notificationBG);

	m_resultRestartGameBtn = cocos2d::Sprite::create("sprite/restartGameButton.png");
	m_resultRestartGameBtn->setPosition((size.width / 2) * 0.74, (size.height / 2) * 0.72);
	m_layerNotification->addChild(m_resultRestartGameBtn);

	m_resultNewGameBtn = cocos2d::Sprite::create("sprite/button.png");
	m_resultNewGameBtn->setPosition((size.width / 2) * 1.26, (size.height / 2) * 0.72);
	m_layerNotification->addChild(m_resultNewGameBtn);

	m_notificationLabel = cocos2d::LabelTTF::create(m_notification, "font/arial.ttf", 32);
	m_notificationLabel->setAnchorPoint(cocos2d::Point(0.0f, 0.0f));
	m_notificationLabel->setPosition(size.width * 0.18, size.height * 0.2);
	m_notificationBG->addChild(m_notificationLabel);

	addEvents();
	return false;
}

bool SubMenu::isMenuActive()
{
	return m_layerOption->isVisible() || m_layerNotification->isVisible();
}
void SubMenu::showMenu(int menu, bool value)
{
	if (menu == OPTIONS_MENU)
	{
		m_layerOption->setVisible(value);
	}
	if (menu == NOTIFICATION_MENU)
	{
		cocos2d::Size size = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

		m_layerNotification->setVisible(value);
		std::string playerName = m_board->getCurrentPlayer()->getPlayerName();
		m_notification = "";
		if (m_board->getCurrentPlayer()->getPlayerType() == HUMAN_PLAYER)
		{
			m_notification = "You Won...!";
		}
		else
		{
			m_notification = playerName + " Won...!";
		}
		m_notificationLabel->setString(m_notification);
		
	}
	if (menu == ALL_MENU)
	{
		m_layerOption->setVisible(value);
		m_layerNotification->setVisible(value);
	}
}

void SubMenu::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (!isMenuActive())
		{
			return false;
		}
		cocos2d::Vec2 p = touch->getLocation();
		cocos2d::Rect btnRect = m_restartBtn->getBoundingBox();

		if (btnRect.containsPoint(p))
		{
			m_restartBtn->setScale(1.1);
		}
		
		btnRect = m_newGameBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_newGameBtn->setScale(1.1);
		}
		btnRect = m_backBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_backBtn->setScale(1.1);
		}

		btnRect = m_resultRestartGameBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_resultRestartGameBtn->setScale(1.1);
		}

		btnRect = m_resultNewGameBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_resultNewGameBtn->setScale(1.1);
		}

		return true;
	};

	listener->onTouchEnded = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		if (!isMenuActive())
		{
			return ;
		}
		cocos2d::Vec2 p = touch->getLocation();
		cocos2d::Rect btnRect = m_restartBtn->getBoundingBox();
		m_restartBtn->setScale(1.0);
		m_newGameBtn->setScale(1.0);
		m_backBtn->setScale(1.0);

		if (btnRect.containsPoint(p))
		{
			m_board->restartGame();
			return;
		}

		btnRect = m_newGameBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_board->newGame();
			return;
		}
		btnRect = m_backBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			showMenu(OPTIONS_MENU, false);
		}
		btnRect = m_resultRestartGameBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_board->restartGame();

		}

		btnRect = m_resultNewGameBtn->getBoundingBox();
		if (btnRect.containsPoint(p))
		{
			m_layerNotification->setVisible(false);
			m_board->newGame();
		}
		if (m_layerNotification->isVisible())
		{
			showMenu(ALL_MENU, false);
		}

		//showMenu(ALL_MENU, false);

		//return false;
	};
	listener->onTouchMoved = [=](cocos2d::Touch* touch, cocos2d::Event* event)
	{
	};
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, m_layerOption);
}