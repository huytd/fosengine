#ifndef _START_MENU_H_
#define _START_MENU_H_

#include <SGF.h>
#include "Level01.h"
#include "GameLevel.h"
#include "Utility.h"

#include "CharSelectScreen.h"

#include "CGUIIconSlot.h"
#include "CGUIIcon.h"
#include "CGUISlotWindow.h"
#include "CGUIBringUpSlotWindowButton.h"

class StartMenu:public sgfLevel
{
private:
	sgfMethodDelegate<StartMenu,irr::SEvent::SGUIEvent> onGUI;
	sgfEntityManager* emgr;
	irr::gui::IGUIImage* bgimg;
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
				//emgr->loadLevel(new CharSelectScreen());
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

		//! Loading unicode font
		Utility::setFont(emgr,"font/myfont.xml");

		//! Get Gui Environment Manager
		irr::gui::IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();

		//! Get mouse
		irr::gui::ICursorControl* cursor = emgr->getCore()->getGraphicDevice()->getCursorControl();

		//! Get screen size
		irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();

		//! Set button text color
		Utility::setTxColor(emgr,irr::video::SColor(155,255,255,0),EGDC_BUTTON_TEXT);

		//env->addImage(env->getVideoDriver()->getTexture("textures/startBG.jpg"),position2d<s32>(0,0),false,0,-1,0);
		bgimg = env->addImage(irr::core::rect<irr::s32>(0,0,screenSize.Width, screenSize.Height),0,-1,0);
		bgimg->setImage(env->getVideoDriver()->getTexture("textures/startBG.jpg"));
		bgimg->setScaleImage(true);

		//! Add menu button
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2), 0, 1,  L"Start", L"Bắt đầu game");
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2+50,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2+50), 0, 2,  L"Quit", L"Thoát khỏi game");

		
		//! Đặt con trỏ chuột đúng giữa nút bắt đầu.
		cursor->setPosition(screenSize.Width/2,screenSize.Height/2);


		//emgr->getCore()->getGraphicDevice()->getVideoDriver()
		//emgr->getCore()->getGraphicDevice()->getGUIEnvironment()
		
		//
		//

		//! create a slot window
		CGUISlotWindow* window = new CGUISlotWindow(emgr->getCore()->getGraphicDevice(), 
													env->getRootGUIElement(),
													-1, 
													rect<s32>(25, 25, 300, 200));
		window->drop();

		//! create a button to show/hide the window
		IGUIButton* button = window->createBringUpButton(rect<s32>(10,210,110,242));
		button->drop();

		//! load some very beautiful textures
		ITexture* slotTex = env->getVideoDriver()->getTexture("hud\\slot.png");
		ITexture* iconTex = env->getVideoDriver()->getTexture("hud\\icon.png");
		ITexture* iconTex2 = env->getVideoDriver()->getTexture("hud\\icon2.png");

		//! create an array of slots in the window
		core::array<IGUIElement*> slotArray = window->addSlotArray(core::rect<s32>(0,0,32,32), slotTex, env->getRootGUIElement(), -1,
		core::position2d<s32>(10,30),core::dimension2d<s32>(6,3),core::dimension2d<s32>(10,10));

		//! create an icon
		CGUIIcon *icon = new CGUIIcon(env, env->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
		icon->drop();
		icon->setImage(iconTex);

		//! create another icon
		CGUIIcon *icon2 = new CGUIIcon(env, env->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
		icon2->drop();
		icon2->setImage(iconTex2);

		//! let the icons know about the slots
		icon->setUsableSlotArray(&slotArray);
		icon2->setUsableSlotArray(&slotArray); 

		//! Add delegate
		emgr->getCore()->getGUIEvent()->addDelegate(&onGUI);

	}
	void onExit(sgfEntityManager* emgr)
	{
		irr::gui::IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
		env->clear();
		emgr->getCore()->getGUIEvent()->removeDelegate(&onGUI);
	}
};

#endif