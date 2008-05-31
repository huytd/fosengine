#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include <SGF.h>

class GameLevel :
	public sgfIrrLevel
{
public:
	GameLevel(const char* fileName);
	void onEnter(sgfEntityManager* emgr);
	void onExit(sgfEntityManager* emgr);
	void onKey(SKeyboardEvent& args);
	virtual ~GameLevel();
private:
	sgfEntityManager* emgr;
	sgfMethodDelegate<GameLevel,SKeyboardEvent> keyDelegate;
};

#endif