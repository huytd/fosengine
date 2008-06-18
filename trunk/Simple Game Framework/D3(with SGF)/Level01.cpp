﻿#include "Level01.h"

//! Quickly create IGUIButton
irr::gui::IGUIButton* createGUIBtn(sgfEntityManager* mgr, irr::video::ITexture* tex, core::position2df pos, wchar_t* tooltiptext=0)
{
	gui::IGUIButton* bt;
	gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	bt = env->addButton(core::rect<irr::s32>(pos.X, pos.Y, pos.X + 36, pos.Y + 36),0,-1,0,tooltiptext);
	bt->setImage(tex);
	bt->setUseAlphaChannel(true);
	//bt->grab(); // why???
	return bt;
}

//! Set font
void setFont(sgfEntityManager* mgr, const irr::c8* fileName)
{
	gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	irr::gui::IGUIFont* font = env->getFont(fileName);
	if (font)
	{
		env->getSkin()->setFont(font);
		env->getSkin()->setFont(font, irr::gui::EGDF_TOOLTIP);
	}
}

Level01::Level01(const char* fileName)
:sgfIrrLevel(fileName)
{
	onGUI.addRef();
	onGUI.bind(this,&Level01::onGUIEvent);
}

//! TASK : Fix this problem, GUIEvent not work
void Level01::onGUIEvent(irr::SEvent::SGUIEvent& args)
	{
		if(args.EventType==irr::gui::EGET_BUTTON_CLICKED)
		{
			if(args.Caller == btn1)
			{
				emgr->loadLevel(emgr->getPreviousLevel());
			}
			else if(args.Caller == btn2)
			{
				map->setVisible(!map->isVisible());		
			}
			else if(args.Caller == btn3)
			{
				map->setExpand(!map->getIsExpand());
			}
		}
	}

void Level01::onEnter(sgfEntityManager* emgr)
{
	sgfIrrLevel::onEnter(emgr);
	this->emgr = emgr;
	
	env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	setFont(emgr, "font/myfont.xml");
	//HP and MP Bar.
	HPBar = new HealthManaBar(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	MPBar = new HealthManaBar(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	HPBar->setTexture("bar/HP.png");
	HPBar->setPosition(10,10);
	HPBar->setMaxValue(300);
	HPBar->setValue(190);
	MPBar->setTexture("bar/MP.png");
	MPBar->setPosition(10,36);
	MPBar->setMaxValue(150);
	MPBar->setValue(10);

	//Destroy object
	HPBar->drop();
	MPBar->drop();

	//! Get character node
	irr::scene::ISceneNode* characterNode = emgr->getCore()->globalVars["characterNode"].getAs<irr::scene::ISceneNode*>();

	//Map.
	map = new Map(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	map->setMapTexture("map/Worldmap.png");
	map->setCharacterTexture("map/character.png");
	map->setWorldSize(512,512);
	map->setCharPosition(characterNode->getPosition());
	map->drop();

	//! Get screen size
	irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();

	//Toolbar in game.
	toolbar = env->addImage(env->getVideoDriver()->getTexture("hud/toolbar.png"),position2d<s32>((screenSize.Width/2)-512,screenSize.Height-64),true);
	btn1 = createGUIBtn(emgr, env->getVideoDriver()->getTexture("hud/btn1.png"),position2df(((screenSize.Width/2)-512) + 163, screenSize.Height - 35),L"Thoát game");
	btn2 = createGUIBtn(emgr, env->getVideoDriver()->getTexture("hud/btn2.png"),position2df(((screenSize.Width/2)-512) + 193, screenSize.Height - 36),L"Ẩn/Hiện bản đồ");
	btn3 = createGUIBtn(emgr, env->getVideoDriver()->getTexture("hud/btn3.png"),position2df(((screenSize.Width/2)-512) + 224, screenSize.Height - 35),L"Phóng to/Thu nhỏ bản đồ");
	
	emgr->getCore()->getGUIEvent()->addDelegate(&onGUI);
}
void Level01::onExit(sgfEntityManager* emgr)
{
	sgfIrrLevel::onExit(emgr);
	env->clear();
	emgr->getCore()->getGUIEvent()->removeDelegate(&onGUI);
}