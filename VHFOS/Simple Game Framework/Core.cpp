#include "Core.h"
#include "Game.h"

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

void sgfCore::init(sgfGame* game)
{
	if(inited)
		return;
	this->game=game;
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
	irr::gui::IGUIEnvironment* env=graphicDevice->getGUIEnvironment();
	guiSkin=new irr::gui::CGUITexturedSkin(env,graphicDevice->getFileSystem());
	env->setSkin(guiSkin);
	//initialize input
	inputManager=new sgfInputManager(this);
	graphicDevice->setEventReceiver(inputManager);
	//entity manager
	entityManager=new sgfEntityManager(this);
	fileSystem=graphicDevice->getFileSystem();
	//physic
	physicWorld=new sgfPhysicWorld(this);
	//frame event
	frameEvent.addDelegate(frameStartEvent.Delegate);
	frameEvent.addDelegate(new sgfMethodDelegate<sgfEntityManager,SFrameEvent>(entityManager,&sgfEntityManager::update));
	frameEvent.addDelegate(new sgfMethodDelegate<sgfScriptVM,SFrameEvent>(scriptVM,&sgfScriptVM::Update));
	frameEvent.addDelegate(new sgfMethodDelegate<sgfPhysicWorld,SFrameEvent>(physicWorld,&sgfPhysicWorld::update));
	frameEvent.addDelegate(new sgfMethodDelegate<sgfGame,SFrameEvent>(game,&sgfGame::updateGraphic));
	frameEvent.addDelegate(frameEndEvent.Delegate);

	inited=true;
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
sgfPhysicWorld* sgfCore::getPhysicWorld() const
{
	return physicWorld;
}