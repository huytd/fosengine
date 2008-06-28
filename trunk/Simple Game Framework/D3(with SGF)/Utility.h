#pragma once
#include <SGF.h>

class Utility
{
public:
	Utility();
public:
	static void setFont(sgfEntityManager *mgr, const irr::c8 *fileName);
	static void setTxColor(sgfEntityManager *mgr, const irr::video::SColor col, irr::gui::EGUI_DEFAULT_COLOR which);
	static irr::gui::IGUIButton* Utility::createGUIBtn(sgfEntityManager *mgr, irr::video::ITexture *tex, irr::core::dimension2di bsize, irr::core::position2df pos, wchar_t *tooltiptext);

	static irr::s32 SX(sgfEntityManager *mgr, irr::s32 x);
	static irr::s32 SY(sgfEntityManager *mgr, irr::s32 y);
};
