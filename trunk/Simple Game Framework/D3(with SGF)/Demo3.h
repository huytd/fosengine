#pragma once
#include "Game.h"
#include <irrlicht.h>

class Demo3 :
	public sgfGame
{
public:
	Demo3(void);
	~Demo3(void);
public:
	static Demo3* instance;
private:
	void showFPS(SFrameEvent&);
};