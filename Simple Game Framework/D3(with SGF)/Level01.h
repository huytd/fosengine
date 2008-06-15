#ifndef _LEVEL_01_H_
#define _LEVEL_01_H_

#include <SGF.h>
#include "HealthManaBar.h"
#include "Map.h"
class Level01:public sgfLevel
{
private:
	sgfEntityManager* emgr;
	irr::gui::IGUIEnvironment* env;
	HealthManaBar* HPBar;
	HealthManaBar* MPBar;
	Map* map;
public:

	Level01();
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
};
#endif