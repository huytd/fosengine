#pragma once
#include <SGF.h>

class Utility
{
public:
	Utility();
public:
	static void setFont(sgfEntityManager *mgr, const irr::c8 *fileName);
	static void setTxColor(sgfEntityManager *mgr, const irr::video::SColor col, irr::gui::EGUI_DEFAULT_COLOR which);
	static irr::gui::IGUIButton* createGUIBtn(sgfEntityManager *mgr, irr::video::ITexture *tex, irr::core::position2df pos, wchar_t *tooltiptext);
};
