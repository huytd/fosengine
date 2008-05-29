#include "Demo3.h"
#include "SGF.h"
#include "Terrain.h"
#include "Character.h"

using namespace irr;

Demo3* Demo3::instance=NULL;

Demo3::Demo3()
{
	Demo3::instance=this;//single instance
	core.config.getIrrlichtParams().DriverType=irr::video::EDT_DIRECT3D9;//override default setting
	core.init();//initialize the core
	//display fps
	sgfEvent<SFrameEvent>* frameEnd=core.getFrameEndEvent();
	frameEnd->addDelegate(new sgfMethodDelegate<Demo3,SFrameEvent>(this,&Demo3::showFPS));
	//register entity class
	registerClass(Character);
	registerClass(Terrain);
	core.globalVars["Test"]="Abc";
	printf(core.globalVars["Test"].getString());
	//set up collision
	core.globalVars["worldCollision"]=(void*)(core.getGraphicDevice()->getSceneManager()->createMetaTriangleSelector());
	//load first level
	core.getEntityManager()->loadLevel(new sgfIrrLevel("levels/start.irr"));
}

Demo3::~Demo3()
{
	core.globalVars["worldCollision"].getAs<scene::IMetaTriangleSelector*>()->drop();
}

int lastFPS=0;
void Demo3::showFPS(SFrameEvent&)
{
	IrrlichtDevice* device=core.getGraphicDevice();
	int FPS=device->getVideoDriver()->getFPS();
	if(FPS!=lastFPS)
	{
		core::stringw str = L"FPS: ";
		str+=FPS;
		lastFPS=FPS;
		device->setWindowCaption(str.c_str());
	}
}