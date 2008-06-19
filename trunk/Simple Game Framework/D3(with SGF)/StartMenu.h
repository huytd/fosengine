#ifndef _START_MENU_H_
#define _START_MENU_H_

#include <SGF.h>
#include "Level01.h"
#include "GameLevel.h"
#include "Utility.h"

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
				emgr->getCore()->getGame()->quit();
			}
		}
	}
	void onEnter(sgfEntityManager* emgr)
	{
		this->emgr=emgr;
		Utility::setFont(emgr,"font/myfont.xml");
		irr::gui::IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
		irr::gui::ICursorControl* cursor = emgr->getCore()->getGraphicDevice()->getCursorControl();
		irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();
		Utility::setTxColor(emgr,irr::video::SColor(255,255,0,0),EGDC_BUTTON_TEXT);
		env->addImage(env->getVideoDriver()->getTexture("textures/startBG.jpg"),position2d<s32>(0,0),false,0,-1,0);
		
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2), 0, 1,  L"Start", L"Bắt đầu game");
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2+50,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2+50), 0, 2,  L"Quit", L"Thoát khỏi game");
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