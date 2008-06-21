/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Third Person Camera
 * \Filename: ThirdPersonCamera.h
 * \Encoding: UTF-8
 * \CreatedDate: 12:47 AM 6/21/2008
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
	ISceneNode* targetNode;//Character.
	Map* map;

	sgfMethodDelegate<HUDControler,SMouseEvent> mouseDelegate;//Register mouse event.
	void onAdd();
	void update(float deltaTime);//Update.
	void onRemove();
	void onLevelStart();
};
#endif
