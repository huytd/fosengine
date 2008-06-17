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

public:

	Level01(const char* fileName);
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
	void onUpdate(sgfEntityManager* emgr);
};
#endif