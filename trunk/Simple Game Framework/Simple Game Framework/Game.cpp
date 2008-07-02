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
	float deltaTime=0.01f;
	unsigned int currentTime=timer->getTime();
	unsigned int startTime=timer->getTime();
	unsigned int frameCounted=0;
	
	while(graphicDevice->run() && running && graphicDevice->isWindowActive())
	{
		SFrameEvent signal(currentTime,deltaTime);
		(*frameEvent)(signal);
		++frameCounted;
		currentTime=timer->getTime();
		int diff=currentTime-startTime;
		if(frameCounted>=10)
		{
			int fps=irr::core::ceil32 ( ( 1000 * frameCounted ) * irr::core::reciprocal(diff) );
			deltaTime=1.0f/fps;
			startTime=currentTime;
			frameCounted=0;
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