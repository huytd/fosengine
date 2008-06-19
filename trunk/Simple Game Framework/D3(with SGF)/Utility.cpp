#include "Utility.h"

Utility::Utility()
{
}


void Utility::setFont(sgfEntityManager *mgr, const irr::c8 *fileName)
{
	irr::gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
			irr::gui::IGUIFont* font = env->getFont(fileName);
			if (font)
			{
				env->getSkin()->setFont(font);
				env->getSkin()->setFont(font, irr::gui::EGDF_TOOLTIP);
			}
}

irr::gui::IGUIButton* Utility::createGUIBtn(sgfEntityManager *mgr, irr::video::ITexture *tex, irr::core::position2df pos, wchar_t *tooltiptext)
{
	irr::gui::IGUIButton* bt;
			irr::gui::IGUIEnvironment* env=mgr->getCore()->getGraphicDevice()->getGUIEnvironment();
			bt = env->addButton(irr::core::rect<irr::s32>(pos.X, pos.Y, pos.X + 36, pos.Y + 36),0,-1,0,tooltiptext);
			bt->setImage(tex);
			bt->setUseAlphaChannel(true);
			//bt->grab(); // why???
			return bt;
}