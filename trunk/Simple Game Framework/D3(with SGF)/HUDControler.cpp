/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Create A Minimap In Game
 * \Filename: HUDControler.cpp
 * \Encoding: UTF-8
 * \CreatedDate: 13:34 2008/06/21
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#include "HUDControler.h"
#include "Utility.h"

using namespace irr;
using namespace scene;
using namespace core;
HUDControler::HUDControler(ISceneNode* targetNode)
{    
	    this->targetNode = targetNode;		
} 

void HUDControler::onLevelStart()
{		
		ISceneManager* smgr = manager->getCore()->getGraphicDevice()->getSceneManager();
		irr::gui::IGUIEnvironment* env = manager->getCore()->getGraphicDevice()->getGUIEnvironment();

		//Map.
		map = new Map(rect<s32>(0,0,Utility::SX(manager, 1024),Utility::SX(manager, 768)),env,env->getRootGUIElement());
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

void HUDControler::onAdd()
{}