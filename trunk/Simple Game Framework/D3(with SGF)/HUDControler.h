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
#include <functional>
#include <irrlicht.h>

#include "Map.h"

using namespace irr;
using namespace scene;
using namespace core;
class HUDControler: public sgfEntity
{
public:
	HUDControler(ISceneNode* targetNode);
	
private:
	//! Active character.
	ISceneNode* targetNode;

	//! Minimap in game
	Map* map;

	
	void onAdd();
	void update(float deltaTime);//Update.
	void onRemove();
	void onLevelStart();
};
#endif
