#ifndef _START_MENU_H_
#define _START_MENU_H_

#include <SGF.h>
#include "Level01.h"
#include "GameLevel.h"

class StartMenu:public sgfLevel
{
private:
	sgfMethodDelegate<StartMenu,irr::SEvent::SGUIEvent> onGUI;
	sgfEntityManager* emgr;
public:

	StartMenu()
	{
		onGUI.addRef();
		onGUI.bind(this,&StartMenu::onGUIEvent);
	}
	
	void onGUIEvent(irr::SEvent::SGUIEvent& args)
	{
		if(args.EventType==irr::gui::EGET_BUTTON_CLICKED)
		{
			if(args.Caller->getID()==1)//start
			{
				printf("Level01\n");
				emgr->loadLevel(new Level01("levels/start.irr"));
			}
			else if(args.Caller->getID()==2)
			{
				Demo3::instance->quit();
			}
		}
	}
	void onEnter(sgfEntityManager* emgr)
	{
		this->emgr=emgr;
		
		irr::gui::IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
		irr::gui::ICursorControl* cursor = emgr->getCore()->getGraphicDevice()->getCursorControl();
		irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-80)/2,(screenSize.Height-60)/2,
			                                     (screenSize.Width+80)/2,
												 (screenSize.Height+60)/2), 0, 1,  L"Start", L"Start the game");
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-80)/2,(screenSize.Height-60)/2+50,
			                                     (screenSize.Width+80)/2,
												 (screenSize.Height+60)/2+50), 0, 2,  L"Quit", L"Quit the game");
		emgr->getCore()->getGUIEvent()->addDelegate(&onGUI);
		cursor->setPosition(screenSize.Width/2,screenSize.Height/2);//Đặt con trỏ chuột đúng giữa nút bắt đầu.
	}
	void onExit(sgfEntityManager* emgr)
	{
		irr::gui::IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
		env->clear();
		emgr->getCore()->getGUIEvent()->removeDelegate(&onGUI);
	}
};

#endif