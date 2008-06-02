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
	irr::video::IVideoDriver* vd=graphicDevice->getVideoDriver();
	float deltaTime=0.0f;
	unsigned int currentTime;
	while(graphicDevice->run()&&running)
	{
		currentTime=timer->getRealTime();
		int fps=vd->getFPS();
		if(fps)
		{
			deltaTime=1.0f/(float)fps;
			SFrameEvent signal(currentTime,deltaTime);
			(*frameEvent)(signal);
		}
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