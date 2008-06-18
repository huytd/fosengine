#ifndef _LEVEL_01_H_
#define _LEVEL_01_H_

#include <SGF.h>
#include "HealthManaBar.h"
#include "Map.h"

class Level01:public sgfIrrLevel
{
private:
	sgfEntityManager* emgr;
	irr::gui::IGUIEnvironment* env;
	HealthManaBar* HPBar;
	HealthManaBar* MPBar;
	Map* map;
	irr::gui::IGUIImage* toolbar;
	irr::gui::IGUIButton* btn1;
	irr::gui::IGUIButton* btn2;
	irr::gui::IGUIButton* btn3;
	sgfMethodDelegate<Level01,irr::SEvent::SGUIEvent> onGUI;

public:

	Level01(const char* fileName);
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
	void onUpdate(sgfEntityManager* emgr);
	void onGUIEvent(irr::SEvent::SGUIEvent& args);
};
#endif