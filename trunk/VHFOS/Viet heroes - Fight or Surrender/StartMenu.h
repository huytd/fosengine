#ifndef _START_MENU_H_
#define _START_MENU_H_

#include <SGF.h>
#include "GameLevel.h"
#include "GameLevel.h"
#include "Utility.h"

#include "CharSelectScreen.h"

#include "CGUIIconSlot.h"
#include "CGUIIcon.h"
#include "CGUISlotWindow.h"
#include "CGUIBringUpSlotWindowButton.h"

class StartMenu : public sgfLevel
{

public:
	CGUISlotWindow* window;
	CGUIIcon *icon;
	CGUIIcon *icon2;
	CGUIIcon *bigIcon;

	IGUIButton* button ;
	ITexture* slotTex;
	ITexture* iconTex ;
	ITexture* iconTex2;
	ITexture* bigIconTex;
	core::array<IGUIElement*> slotArray;

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
				emgr->loadLevel(new GameLevel("levels/start.irr"));
				//emgr->loadLevel(new CharSelectScreen());
			}
			else if(args.Caller->getID()==2)
			{
				emgr->loadLevel(new GameLevel("levels/levelOne.irr"));
			}
			else if(args.Caller->getID()==3)
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
		irr::gui::IGUIEnvironment* env = emgr->getCore()->getGraphicDevice()->getGUIEnvironment();

		//! Get mouse
		irr::gui::ICursorControl* cursor = emgr->getCore()->getGraphicDevice()->getCursorControl();

		//! Get screen size
		irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();

		//! Set button text color
		Utility::setTxColor(emgr,irr::video::SColor(255,255,255,255),EGDC_BUTTON_TEXT);

		//env->addImage(env->getVideoDriver()->getTexture("textures/startBG.jpg"),position2d<s32>(0,0),false,0,-1,0);
		bgimg = env->addImage(irr::core::rect<irr::s32>(0,0,screenSize.Width, screenSize.Height),0,-1,0);
		bgimg->setImage(env->getVideoDriver()->getTexture("textures/startBG.jpg"));
		bgimg->setScaleImage(true);

		//! Add menu button
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2), 0, 1,  L"Bắt đầu", L"Bắt đầu game");
		
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2+50,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2+50), 0, 2,  L"Bắt đầu 2", L"Bắt đầu game");
		
		env->addButton(irr::core::rect<irr::s32>((screenSize.Width-150),(screenSize.Height-60)/2+50+50,
			                                     (screenSize.Width),
												 (screenSize.Height+60)/2+50+50), 0, 3,  L"Thoát", L"Thoát khỏi game");

		
		//! Đặt con trỏ chuột đúng giữa nút bắt đầu.
		cursor->setPosition(screenSize.Width/2,screenSize.Height/2);


		//emgr->getCore()->getGraphicDevice()->getVideoDriver()
		//emgr->getCore()->getGraphicDevice()->getGUIEnvironment()

		//! create a slot window
		window = new CGUISlotWindow(emgr->getCore()->getGraphicDevice(), 
													env->getRootGUIElement(),
													-1, 
													rect<s32>(25, 25, 300, 200));
		

		////! create a button to show/hide the window
		button = window->createBringUpButton(rect<s32>(10,210,110,242));
		
		//! load some very beautiful textures
		slotTex = env->getVideoDriver()->getTexture("hud\\slot.png");
		iconTex = env->getVideoDriver()->getTexture("hud\\icon.png");
		iconTex2 = env->getVideoDriver()->getTexture("hud\\icon2.png");
		bigIconTex = env->getVideoDriver()->getTexture("hud\\bigicon.png");

		//! create an array of slots in the window
		slotArray = window->addSlotArray(core::rect<s32>(0,0,32,32), slotTex, env->getRootGUIElement(), -1,
		core::position2d<s32>(80,40),core::dimension2d<s32>(6,6),core::dimension2d<s32>(0,0));

		//! create an icon
		icon = new CGUIIcon(env, env->getRootGUIElement(), -1, rect<s32>(0,0,32,32));		
		icon->setImage(iconTex);

		//! create another icon
		icon2 = new CGUIIcon(env, env->getRootGUIElement(), -1, rect<s32>(0,0,32,32));
		
		icon2->setImage(iconTex2);

		//! create another icon
		bigIcon = new CGUIIcon(env, env->getRootGUIElement(), -1, rect<s32>(0,0,64,64));
		
		bigIcon->setImage(bigIconTex);

		//! Setup icon
		icon->setMoveable(true);
		icon2->setMoveable(true);
		bigIcon->setMoveable(true);

		/*icon->setCanBeOutsideSlot(true);
		icon2->setCanBeOutsideSlot(true);*/

		//! let the icons know about the slots
		icon->setUsableSlotArray(&slotArray);
		icon2->setUsableSlotArray(&slotArray);
		bigIcon->setUsableSlotArray(&slotArray);


		//! Add delegate
		emgr->getCore()->getGUIEvent()->addDelegate(&onGUI);

	}
	
	void onExit(sgfEntityManager* emgr)
	{
		irr::gui::IGUIEnvironment* env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	
		env->clear();
		emgr->getCore()->getGUIEvent()->removeDelegate(&onGUI);

		//! Destroy drag and drop gui object
		window->drop();
		icon->drop();
		button->drop();
		bigIcon->drop();
		icon2->drop();

	}
};

#endif