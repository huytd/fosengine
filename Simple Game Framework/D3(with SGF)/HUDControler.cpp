﻿/**
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
HUDControler::HUDControler()
{ 
} 

void HUDControler::onLevelStart()
{

}


void HUDControler::update(float deltaTime)
{	
}

void HUDControler::onRemove()
{
	    //Destroy Object.
	    map->removeIcon(icon);
	    map->drop();
}

void HUDControler::onAdd()
{
		ISceneManager* smgr = manager->getCore()->getGraphicDevice()->getSceneManager();
		irr::gui::IGUIEnvironment* env = manager->getCore()->getGraphicDevice()->getGUIEnvironment();
		terrain = manager->getCore()->globalVars["terrain"].getAs<ITerrainSceneNode*>();
		irr::scene::IAnimatedMeshSceneNode* node = manager->getCore()->globalVars["characterNode"].getAs<irr::scene::IAnimatedMeshSceneNode*>();
		//Map.
		map = new Map(rect<s32>(0,0,Utility::SX(manager, 1024),Utility::SX(manager, 768)),env,env->getRootGUIElement());
		manager->getCore()->globalVars["map"] = map;
		map->setSize(150,170,10,10);
		map->setMapTexture("map/Worldmap.png");
		map->setIcon0Texture("map/character.png");//icon style 0
		map->setIcon1Texture("map/npc.png");//icon style 1
		map->setIcon2Texture("map/character.png");//icon style 2
		map->setWorld(terrain);
		map->setExpand(false);
		//Thêm 1 icon
		icon = new Icon(rect<s32>(0,0,800,600),manager->getCore()->getGraphicDevice()->getGUIEnvironment(),
			            manager->getCore()->getGraphicDevice()->getGUIEnvironment()->getRootGUIElement());
        map->addIcon(icon,0,node);//Đặt kiểu 0 cho icon vừa tạo và gán cho nó Node của thằng Ninja
		map->setTarget(icon);//Sai thằng map theo dõi icon trên
}