#pragma once
#include <SGF.h>

class Utility
{
public:
	Utility();
public:
	static void setFont(sgfEntityManager *mgr, const irr::c8 *fileName);
	static irr::gui::IGUIButton* createGUIBtn(sgfEntityManager *mgr, irr::video::ITexture *tex, irr::core::position2df pos, wchar_t *tooltiptext);
};
