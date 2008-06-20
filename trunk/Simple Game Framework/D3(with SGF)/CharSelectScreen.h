#ifndef _CHAR_SELECT_SCREEN_H_
#define _CHAR_SELECT_SCREEN_H_

#include <SGF.h>

class CharSelectScreen:public sgfLevel
{
private:
	sgfEntityManager* emgr;
	irr::gui::IGUIEnvironment* env;
	sgfMethodDelegate<CharSelectScreen,irr::SEvent::SGUIEvent> onGUI;

public:

	CharSelectScreen();
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
	void onUpdate(sgfEntityManager* emgr);
	void onGUIEvent(irr::SEvent::SGUIEvent& args);
};

#endif