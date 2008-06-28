#include "Demo3.h"
#include <SGF.h>
#include "Terrain.h"
#include "Character.h"
#include "NPC.h"
#include "StartMenu.h"
#include "Enemy.h"

#include <exception>
using namespace irr;

void testException()
{
	throw std::exception("test exception");
}

Demo3::Demo3()
{
	core.config.getIrrlichtParams().DriverType=irr::video::EDT_DIRECT3D9;//override default setting
	core.init(this);//initialize the core
	//set skin(use script to set skin)
	//core.getGUISkin()->setSkin("guiSkin/guiSkin.xml");
	//display fps
	Character::colID=core.getPhysicWorld()->createCollisionClass();
	Enemy::colID=core.getPhysicWorld()->createCollisionClass();
	sgfEvent<SFrameEvent>* frameEnd=core.getFrameEndEvent();
	frameEnd->addDelegate(new sgfMethodDelegate<Demo3,SFrameEvent>(this,&Demo3::showFPS));
	//test script
	sgfScriptVM* vm=core.getScriptVM();
	vm->SetDebugMode(true);
	//bind CGUITexturedSkin class
#define Typename irr::gui::CGUITexturedSkin
	gmType guiSkinType=vm->Bind(
		CLASS("GUISkin"),
		METHOD(setSkin),
		END
		);
#undef Typename
	vm->Bind(GLOBAL_NAMESPACE,FUNCTION(testException),END);
	//vm->UpdateTypeIds();//since there's no cross-reference between classes, this is not necessary
	//pass our instance to script
	core.globalVars["skin"].setAs<irr::gui::CGUITexturedSkin*>(core.getGUISkin(),guiSkinType);
	//vm->ExecuteFile("testScript.gm");//a script can be executed directly, I just want to test compiling
	//vm->CompileFile("testScript.gm","testScript.lib");
	//vm->ExecuteLibFile("testScript.lib");
	//call a script function from C++

	//vm->Call<const char*>(vm->GetFunction("scriptFunction"),"test");

	//register entity class
	registerClass(Character);
	registerClass(Terrain);
	registerClass(NPC);
	registerClass(Enemy);
	
	//set up collision
	core.globalVars["worldCollision"]=(void*)(core.getGraphicDevice()->getSceneManager()->createMetaTriangleSelector());
	//load first level
	core.getEntityManager()->loadLevel(new StartMenu);
}

Demo3::~Demo3()
{
	core.globalVars["worldCollision"].getAs<scene::IMetaTriangleSelector*>()->drop();
}

int lastFPS=0, lastMaximalPrimitive=0, lastPrimitive=0;
int FPS=0;
int MaximalPrimitive = 0;
int Primitive = 0;
void Demo3::showFPS(SFrameEvent&)
{
	IrrlichtDevice* device=core.getGraphicDevice();
	FPS=device->getVideoDriver()->getFPS();
	MaximalPrimitive = device->getVideoDriver()->getMaximalPrimitiveCount();
	Primitive  = device->getVideoDriver()->getPrimitiveCountDrawn();

	if(FPS!=lastFPS || MaximalPrimitive!=lastMaximalPrimitive || Primitive!=lastPrimitive)
	{
		core::stringw str = L"FPS: ";
		str+=FPS;

		str+= L" - MaxPrimitive: ";
		str+=MaximalPrimitive;
		
		str+= L" - PrimitiveCountDrawn: ";
		str+=Primitive;
				
		lastFPS=FPS;
		lastPrimitive = Primitive;
		lastMaximalPrimitive = MaximalPrimitive;

		device->setWindowCaption(str.c_str());
	}
}