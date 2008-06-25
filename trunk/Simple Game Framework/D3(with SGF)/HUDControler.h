/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Create A Minimap In Game
 * \Filename: HUDControler.h
 * \Encoding: UTF-8
 * \CreatedDate: 13:34 2008/06/21
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#ifndef _HUD_CONTROLER_H_
#define _HUD_CONTROLER_H_

#include <SGF.h>
#include <irrlicht.h>
#include "Icon.h" 
#include "Map.h"

using namespace irr;
using namespace scene;
using namespace core;
class HUDControler: public sgfEntity
{
public:
	HUDControler();
	
private:
	//! Minimap in game
	Map* map;
	Icon* icon;
    ITerrainSceneNode* terrain;
	void onAdd();
	void update(float deltaTime);//Update.
	void onRemove();
	void onLevelStart();
};
#endif
