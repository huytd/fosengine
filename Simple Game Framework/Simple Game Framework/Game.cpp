#include "Game.h"
#include "Core.h"

sgfGame::sgfGame()
{
}

sgfGame::~sgfGame()
{
}

void sgfGame::run()
{
	running=true;
	sgfEvent<SFrameEvent>* frameEvent=core.getFrameEvent();
	irr::IrrlichtDevice* graphicDevice=core.getGraphicDevice();
	irr::ITimer* timer=graphicDevice->getTimer();
	unsigned int endTime=timer->getRealTime();
	unsigned int deltaTime=0;
	while(graphicDevice->run()&&running)
	{
		unsigned int startTime=timer->getRealTime();
		SFrameEvent signal(startTime,deltaTime);
		frameEvent->fire(signal);
		endTime=timer->getRealTime();
		deltaTime=endTime-startTime;
	}
}

sgfCore* sgfGame::getCore() const
{
	return const_cast<sgfCore*>(&core);
}

void sgfGame::quit()
{
	running=false;
}