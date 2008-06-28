#include "GameLevel.h"
#include "Utility.h"

using namespace irr;
using namespace gui;

GameLevel::GameLevel(const char* fileName)
:sgfIrrLevel(fileName)
{
	onGUI.addRef();
	onGUI.bind(this,&GameLevel::onGUIEvent);
	keyDelegate.addRef();
	keyDelegate.bind(this,&GameLevel::onKey);
}

void GameLevel::onKey(SKeyboardEvent& args)
{
	if((args.keyEvent==SKeyboardEvent::EKS_PRESSED)&&(args.keyCode==irr::KEY_ESCAPE ))
	{
		emgr->loadLevel(emgr->getPreviousLevel());//go back
	}
}

//! TASK : Fix this problem, GUIEvent not work
void GameLevel::onGUIEvent(irr::SEvent::SGUIEvent& args)
	{
		if(args.EventType==irr::gui::EGET_BUTTON_CLICKED)
		{
			if(args.Caller == btn1)
			{
				emgr->loadLevel(emgr->getPreviousLevel());
			}
			else if(args.Caller == btn2)
			{
				//map->setVisible(!map->isVisible());		
			}
			else if(args.Caller == btn3)
			{
				//map->setExpand(!map->getIsExpand());
			}
		}
	}

void GameLevel::onEnter(sgfEntityManager* emgr)
{
	sgfIrrLevel::onEnter(emgr);
	this->emgr = emgr;
	
	env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	Utility::setFont(emgr, "font/myfont.xml");

	emgr->getCore()->getGraphicDevice()->getGUIEnvironment()->addStaticText(L"Press esc to exit",core::rect<s32>(10,10,80,30));
	emgr->getCore()->getInputManager()->getKeyboardEvent()->addDelegate(&keyDelegate);


	//! Get character node
	irr::scene::ISceneNode* characterNode = emgr->getCore()->globalVars["characterNode"].getAs<irr::scene::ISceneNode*>();

	//! Get screen size
	irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();

	//Toolbar in game.
	//toolbar = env->addImage(env->getVideoDriver()->getTexture("hud/toolbar.png"),position2d<s32>((screenSize.Width/2)-512,screenSize.Height-64),true);
	/*btn1 = Utility::createGUIBtn(emgr, env->getVideoDriver()->getTexture("hud/btn1.png"),position2df(((screenSize.Width/2)-512) + 163, screenSize.Height - 35),L"Thoát game");
	btn2 = Utility::createGUIBtn(emgr, env->getVideoDriver()->getTexture("hud/btn2.png"),position2df(((screenSize.Width/2)-512) + 193, screenSize.Height - 36),L"Ẩn/Hiện bản đồ");
	btn3 = Utility::createGUIBtn(emgr, env->getVideoDriver()->getTexture("hud/btn3.png"),position2df(((screenSize.Width/2)-512) + 224, screenSize.Height - 35),L"Phóng to/Thu nhỏ bản đồ");
	*/
	emgr->getCore()->getGUIEvent()->addDelegate(&onGUI);
}
void GameLevel::onExit(sgfEntityManager* emgr)
{
	sgfIrrLevel::onExit(emgr);
	env->clear();
	emgr->getCore()->getGUIEvent()->removeDelegate(&onGUI);
	emgr->getCore()->getInputManager()->getKeyboardEvent()->removeDelegate(&keyDelegate);
}