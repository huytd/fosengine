#include "Utility.h"

Utility::Utility()
{
}

void Utility::drawImage(sgfEntityManager *emgr, irr::c8 *imagePath)
{
	irr::gui::IGUIImage* bgimg;

	//! Loading unicode font
	Utility::setFont(emgr,"font/myfont.xml");

	//! Get Gui Environment Manager
	irr::gui::IGUIEnvironment* env = emgr->getCore()->getGraphicDevice()->getGUIEnvironment();

	//! Get screen size
	irr::core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();

	bgimg = env->addImage(irr::core::rect<irr::s32>(0,0,screenSize.Width, screenSize.Height),0,-1,0);
	bgimg->setImage(env->getVideoDriver()->getTexture(imagePath));
	bgimg->setScaleImage(true);
	//bgimg->drop();
}

void Utility::setFont(sgfEntityManager *mgr, const irr::c8 *fileName)
{
	irr::gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
			irr::gui::IGUIFont* font = env->getFont(fileName);
			if (font)
			{
				env->getSkin()->setFont(font);
				env->getSkin()->setFont(font, irr::gui::EGDF_TOOLTIP);
				env->getSkin()->setFont(font, irr::gui::EGDF_BUTTON);
			}
			
}

void Utility::setTxColor(sgfEntityManager *mgr, const irr::video::SColor col, irr::gui::EGUI_DEFAULT_COLOR which)
{
	irr::gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	irr::video::SColor co = env->getSkin()->getColor(which);
	co.set(col.getAlpha(), col.getRed(), col.getBlue(), col.getGreen());
	env->getSkin()->setColor(which, co);
}

irr::gui::IGUIButton* Utility::createGUIBtn(sgfEntityManager *mgr, irr::video::ITexture *tex, irr::core::dimension2di bsize, irr::core::position2df pos, wchar_t *tooltiptext)
{
	irr::gui::IGUIButton* bt;
			irr::gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
			bt = env->addButton(irr::core::rect<irr::s32>(pos.X, pos.Y, pos.X + 36, pos.Y + 36),0,-1,0,tooltiptext);
			bt->setImage(tex);
			bt->setUseAlphaChannel(true);
			return bt;
}

irr::s32 Utility::SX(sgfEntityManager *mgr, irr::s32 x)
{
	return (irr::s32)(mgr->getCore()->getGraphicDevice()->getVideoDriver()->getScreenSize().Width*(irr::f32)(x)/1024.0f);
}

irr::s32 Utility::SY(sgfEntityManager *mgr, irr::s32 y)
{
	return (irr::s32)(mgr->getCore()->getGraphicDevice()->getVideoDriver()->getScreenSize().Height*(irr::f32)(y)/768.0f);
}