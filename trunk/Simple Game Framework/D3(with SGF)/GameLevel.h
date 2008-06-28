#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include <SGF.h>

class GameLevel:public sgfIrrLevel
{
private:
	sgfEntityManager* emgr;
	irr::gui::IGUIEnvironment* env;
	
	irr::gui::IGUIImage* toolbar;
	irr::gui::IGUIButton* btn1;
	irr::gui::IGUIButton* btn2;
	irr::gui::IGUIButton* btn3;

	sgfMethodDelegate<GameLevel,irr::SEvent::SGUIEvent> onGUI;
	sgfMethodDelegate<GameLevel,SKeyboardEvent> keyDelegate;

public:

	GameLevel(const char* fileName);
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
	void onUpdate(sgfEntityManager* emgr);
	void onGUIEvent(irr::SEvent::SGUIEvent& args);
	void onKey(SKeyboardEvent& args);

};
#endif