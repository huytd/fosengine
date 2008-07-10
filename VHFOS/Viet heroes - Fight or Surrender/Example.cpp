//#include <windows.h>
//#include <irrlicht.h>
//#include <iostream>
//#include "effectWrapper.h"
//
//using namespace irr;
//using namespace scene;
//using namespace video;
//using namespace core;
//using namespace io;
//using namespace gui;
//
//bool disp2d = 0;
//
//class MyEventReceiver : public IEventReceiver
//{
//public:
//
//	MyEventReceiver(scene::ICameraSceneNode* cammy)
//	{
//		Cam = cammy;
//	}
//
//	bool OnEvent(const SEvent &event)
//	{
//		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
//		{
//			switch (event.KeyInput.Key)
//			{
//			case irr::KEY_KEY_R: 
//				disp2d = !(disp2d);
//				return true;
//			}
//		}
//
//		return false;
//	}
//
//private:
//	scene::ICameraSceneNode* Cam;
//};
//
//int main()
//{
//	char C = 0;
//
//		printf
//		(
//		"Welcome to the X-Platform Effect Wrapper Demo"
//		"\nPlease select the driver type:"
//		"\n1 - OpenGL"
//		"\n2 - Direct3D9\n"
//		);
//
//	std::cin >> C;
//
//	E_DRIVER_TYPE dType = EDT_OPENGL;
//
//	switch(C)
//	{
//	case '1':
//		dType = EDT_OPENGL;
//		break;
//	case '2':
//		dType = EDT_DIRECT3D9;
//		break;
//	default:
//		dType = EDT_OPENGL;
//		break;
//	}
//
//	printf
//		(
//		"\nPlease select the ShadowMap resolution:"
//		"\n1 - 512x512"
//		"\n2 - 1024x1024"
//		"\n3 - 2048x2048"
//		"\n4 - 4096x4096\n"
//		);
//
//	std::cin >> C;
//	dimension2d<s32> shadowDimen = dimension2d<s32>(2048,2048);
//
//	switch(C)
//	{
//	case '1':
//		shadowDimen = dimension2d<s32>(512,512);
//		break;
//	case '2':
//		shadowDimen = dimension2d<s32>(1024,1024);
//		break;
//	case '3':
//		shadowDimen = dimension2d<s32>(2048,2048);
//		break;
//	case '4':
//		shadowDimen = dimension2d<s32>(4096,4096);
//		break;
//	default:
//		shadowDimen = dimension2d<s32>(1024,1024);
//		break;
//	}
//
//	printf
//		(
//		"\nPlease select the ShadowMap filtering:"
//		"\n1 - none"
//		"\n2 - 4 PCF"
//		"\n3 - 8 PCF"
//		"\n4 - 12 PCF"
//		"\n5 - 16 PCF\n"
//		);
//
//	std::cin >> C;
//
//	E_FILTER_TYPE filterType = EFT_4PCF;
//
//	switch(C)
//	{
//	case '1':
//		filterType = EFT_NONE;
//		break;
//	case '2':
//		filterType = EFT_4PCF;
//		break;
//	case '3':
//		filterType = EFT_8PCF;
//		break;
//	case '4':
//		filterType = EFT_12PCF;
//		break;
//	case '5':
//		filterType = EFT_16PCF;
//		break;
//	default:
//		filterType = EFT_4PCF;
//		break;
//	}
//
//IrrlichtDevice* device = createDevice(dType,dimension2d<s32>(800,600),32);
//
//if(!device)
//	exit(1);
//
//ISceneManager* smgr = device->getSceneManager();
//IVideoDriver* driver = device->getVideoDriver();
//
//ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,100,10);
//cam->setPosition(vector3df(0,10,0));
//cam->setFarValue(50);
//
//dimension2d<s32> ScreenRTT = (driver->getVendorInfo().equals_ignore_case("NVIDIA Corporation")) ? driver->getScreenSize() : dimension2d<s32>(512,512);
//
//effectHandler* effect = new effectHandler(device,shadowDimen,"",ScreenRTT);
//
//// If you want terrain:
////ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("terrain.bmp",0,-1,vector3df(0,0,0),vector3df(0,0,0),vector3df(1,1,1),SColor(255,255,255,255),1,ETPS_33);
////
////terrain->setMaterialTexture(0,driver->getTexture("terrain.jpg"));
////terrain->setMaterialTexture(1, driver->getTexture("detailmap.jpg"));
////terrain->scaleTexture(1,50);
////terrain->setScale(core::vector3df(4, 0.05f, 4));
////terrain->setPosition(vector3df(-120,-9.5f,-120));
////terrain->getMaterial(0).MaterialType = EMT_DETAIL_MAP;
////terrain->setMaterialFlag(EMF_LIGHTING,false);
////effect->addShadowToNode(terrain,filterType);
//
//// If you want a room:
//IAnimatedMeshSceneNode* room = smgr->addAnimatedMeshSceneNode(smgr->getMesh("ShadRoom.b3d"));
//room->setScale(vector3df(3,2,3));
//room->setPosition(vector3df(4.5f,0.5f,4));
//room->setMaterialTexture(0,driver->getTexture("wall.bmp"));
//effect->addShadowToNode(room,filterType);
//
//IAnimatedMeshSceneNode* dwarf;
//IAnimatedMesh* dwarfmesh = smgr->getMesh("dwarf.x");
//
//for(int g = 0;g < 3;g++)
//{
//	for(int v = 0;v < 3;v++)
//	{
//		dwarf = smgr->addAnimatedMeshSceneNode(dwarfmesh);
//		dwarf->setScale(vector3df(0.05f,0.05f,0.05f));
//		dwarf->setPosition(vector3df(g * 4,0.5f,v * 3));
//		dwarf->setMaterialFlag(EMF_NORMALIZE_NORMALS,true);
//
//		dwarf->setAutomaticCulling(EAC_FRUSTUM_BOX);
//		dwarf->setMaterialType(EMT_SOLID);
//
//		if(g == 0 && v == 0)
//		{
//		effect->addEffectToNode(dwarf,EET_ANISO);
//		effect->addShadowToNode(dwarf, EFT_NONE, ESM_CAST);
//		}
//		else if(g == 0 && v == 1)
//		{
//		effect->addEffectToNode(dwarf,EET_PHONG);
//		dwarf->setMaterialTexture(0,driver->getTexture("red.bmp"));
//		effect->addShadowToNode(dwarf, EFT_NONE, ESM_CAST);
//		}
//		else if(g == 0 && v == 2)
//		{
//		effect->addEffectToNode(dwarf,EET_GOOCH);
//		effect->addShadowToNode(dwarf, EFT_NONE, ESM_CAST);
//		}
//		else
//		{
//		effect->addShadowToNode(dwarf,filterType);
//		}
//
//	}
//}
//
//effect->getLightCamera()->addAnimator(smgr->createFlyCircleAnimator(vector3df(0,20,12),30,0.001f));
//effect->getLightCamera()->setPosition(dwarf->getPosition() + vector3df(0,20,30));
//effect->getLightCamera()->setNearValue(10);
//effect->setMaxShadowDistanceFromLight(70);
//smgr->addLightSceneNode(effect->getLightCamera());
//
//ISceneNode* cube = smgr->addSphereSceneNode(3,16,0,-1,vector3df(dwarf->getPosition() + vector3df(-3.5f,0,5)));
//cube->setMaterialTexture(0,driver->getTexture("red.bmp"));
//effect->addEffectToNode(cube,EET_MRWIGGLE);
//effect->addNodeToDepthPass(cube);
//
//effect->setShadowDarkness(1.0f);
//
//MyEventReceiver receiver(cam);
//device->setEventReceiver(&receiver);
//
//cam->setNearValue(0.1f);
//
//effect->setClearColour(SColor(255,250,100,0));
//
//core::stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
//
//effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurHP") + shaderExt);
//effect->addPostProcessingEffectFromFile(core::stringc("shaders/BlurVP") + shaderExt);
//effect->addPostProcessingEffectFromFile(core::stringc("shaders/BloomP") + shaderExt);
//
//while(device->run())
//{
//	wchar_t tmp[255];
//	swprintf(tmp,255,L"Shadow Map Demo (FPS: %d)",driver->getFPS());
//	device->setWindowCaption(tmp);
//
//	driver->beginScene(true,true,SColor(0,0,0,0));
//	effect->setLightTarget(vector3df(0,0,12));
//
//	// effectHandler->update() now replaces smgr->drawAll()!
//	effect->update();	
//
//	if(disp2d)
//		driver->draw2DImage(effect->getShadowMapTexture(),position2d<s32>(0,0));
//
//	driver->endScene();
//}
//
//device->drop();
//return 0;
//}
