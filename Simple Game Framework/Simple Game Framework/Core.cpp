#include "Core.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace io;

sgfCore::sgfCore()
:inited(0)
{
}

sgfCore::~sgfCore()
{
	graphicDevice->drop();
	delete guiSkin;
}

static irr::video::IVideoDriver* vd;
static irr::scene::ISceneManager* smgr;//speed up graphic updating
static irr::gui::IGUIEnvironment* env;

void sgfCore::init()
{
	if(inited)
		return;
	IrrlichtDevice* tempDevice=createDevice(irr::video::EDT_NULL);
	if(!config.read(tempDevice))// if config file is not found
		config.write(tempDevice);// write it
	tempDevice->drop();
	graphicDevice=createDeviceEx(config.getIrrlichtParams());
	//Script virtual machine
	scriptVM=new sgfScriptVM(this,graphicDevice->getFileSystem());
	//-------Standard binds----------------
	sgfDynamicVar::any_ptr=scriptVM->GetTypeId("any_ptr");
	globalVars.setTable(scriptVM,scriptVM->GetGlobals());
	//-------------------------------------
	//GUI
	env=graphicDevice->getGUIEnvironment();
	guiSkin=new irr::gui::CGUITexturedSkin(env,graphicDevice->getFileSystem());
	env->setSkin(guiSkin);
	//initialize input
	inputManager=new sgfInputManager(this);
	graphicDevice->setEventReceiver(inputManager);
	//entity manager
	entityManager=new sgfEntityManager(this);
	fileSystem=graphicDevice->getFileSystem();
	//frame event
	frameEvent.addDelegate(frameStartEvent.Delegate);
	frameEvent.addDelegate(new sgfMethodDelegate<sgfEntityManager,SFrameEvent>(entityManager,&sgfEntityManager::update));
	frameEvent.addDelegate(new sgfMethodDelegate<sgfCore,SFrameEvent>(this,&sgfCore::updateGraphic));//there's no leak here
	frameEvent.addDelegate(frameEndEvent.Delegate);
	inited=true;
	vd=graphicDevice->getVideoDriver();//some shortcuts to speed up rendering loop
	smgr=graphicDevice->getSceneManager();
}

void sgfCore::updateGraphic(SFrameEvent& data)
{
	if(graphicDevice->isWindowActive())
	{
		vd->beginScene(true,true,SColor(0,200,200,200));
		smgr->drawAll();
		env->drawAll();
		vd->endScene();
	}
	else
		graphicDevice->yield();
}

sgfEntityManager* sgfCore::getEntityManager() const
{
	return entityManager;
}

IrrlichtDevice* sgfCore::getGraphicDevice() const
{
	return graphicDevice;
}

sgfInputManager* sgfCore::getInputManager() const
{
	return inputManager;
}

IFileSystem* sgfCore::getFileSystem() const
{
	return fileSystem;
}

sgfEvent<SFrameEvent>* sgfCore::getFrameEvent() const
{
	return const_cast<sgfEvent<SFrameEvent>*>(&frameEvent);
}

sgfEvent<SFrameEvent>* sgfCore::getFrameStartEvent() const
{
	return const_cast<sgfEventRelay<SFrameEvent>*>(&frameStartEvent);
}

sgfEvent<SFrameEvent>* sgfCore::getFrameEndEvent() const
{
	return const_cast<sgfEventRelay<SFrameEvent>*>(&frameEndEvent);
}
sgfEvent<irr::SEvent::SGUIEvent>* sgfCore::getGUIEvent() const
{
	return const_cast<sgfEvent<irr::SEvent::SGUIEvent>*>(&guiEvent);
}
irr::gui::CGUITexturedSkin* sgfCore::getGUISkin() const
{
	return guiSkin;
}
sgfScriptVM* sgfCore::getScriptVM() const
{
	return scriptVM;
}