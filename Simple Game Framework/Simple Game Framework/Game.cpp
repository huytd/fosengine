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
	core.entityManager->endGame();
}

void sgfGame::updateGraphic(SFrameEvent& data)
{
	static irr::IrrlichtDevice* device=core.graphicDevice;
	static irr::video::IVideoDriver* vd=device->getVideoDriver();
	static irr::scene::ISceneManager* smgr=device->getSceneManager();
	static irr::gui::IGUIEnvironment* env=device->getGUIEnvironment();
	if(device->isWindowActive())
	{
		vd->beginScene(true,true,irr::video::SColor(0,200,200,200));
		smgr->drawAll();
		env->drawAll();
		vd->endScene();
	}
	else
		device->yield();
}

void sgfGame::quit()
{
	running=false;
}