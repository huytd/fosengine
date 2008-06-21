/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Third Person Camera
 * \Filename: HUDControler.cpp
 * \Encoding: UTF-8
 * \CreatedDate: 2:17:PM 2008/06/15
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#include "HUDControler.h"
using namespace irr;
using namespace scene;
using namespace core;
HUDControler::HUDControler(ISceneNode* targetNode)
{    
	    this->targetNode = targetNode;		
} 

void HUDControler::onAdd()
{
}

void HUDControler::onLevelStart()
{		
		ISceneManager* smgr = manager->getCore()->getGraphicDevice()->getSceneManager();
		irr::gui::IGUIEnvironment* env = manager->getCore()->getGraphicDevice()->getGUIEnvironment();

		//Map.
		map = new Map(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
		map->setMapTexture("map/Worldmap.png");
		map->setCharacterTexture("map/character.png");
		map->setWorldSize(512,512);
		map->setCharPosition(targetNode->getPosition());
		
		manager->setActive(this,true);
}

void HUDControler::update(float deltaTime)
{	
		map->setCharPosition(targetNode->getPosition());
}

void HUDControler::onRemove()
{
		map->drop();
}