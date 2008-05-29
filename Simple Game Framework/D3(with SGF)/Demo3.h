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
	irr::scene::IMetaTriangleSelector* worldCollision;
	static Demo3* instance;
private:
	void showFPS(SFrameEvent&);
};