#ifndef LEVELONESTATE_H_
#define LEVELONESTATE_H_

#include <IAnimatedMeshSceneNode.h>
#include <ILightSceneNode.h>
#include <ICameraSceneNode.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>
#include "CCharacter.h"
#include "Terrain.h"
#include "HUD.h"
class LevelOneState : public State<Game, irr::SEvent>
{

private:
	irr::scene::ILightSceneNode* light;

	//Main character
	CCharacter* viethero;

	//Game's terrain
	Terrain* terrain;

	//Game Heads-Up Display
	HUD *hud;

	/// Selector to get collition
	scene::ITriangleSelector* selector;

	scene::SMeshBufferLightMap buffer;
	scene::SMesh mesh;
	scene::ISceneNodeAnimator* anim;
	scene::ISceneNodeAnimator* animmy;

	scene::ISceneNode* selectedSceneNode;
    scene::ISceneCollisionManager* coll;

	core::triangle3df collisionTriangle;
	core::vector3df collisionPoint;

public:

	LevelOneState();

	virtual ~LevelOneState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* game, const irr::SEvent& event);

	virtual void onUpdate(Game* game);
};

LevelOneState::LevelOneState() :
	light(0), viethero(0), terrain(0), 	hud(0), selector(0)
{

}

LevelOneState::~LevelOneState()
{
	if (light)
	{
		light->remove();
		light->drop();
	}

	if(viethero)
		delete viethero;
	

	if(terrain)
		delete terrain;
	
	if (hud)
		delete hud;


}

void LevelOneState::onEnter(Game* game)
{

	//setup terrain
	terrain = new Terrain(game);

	//Setup viethero character
	viethero = new CCharacter(game->getDevice(),game->getSceneManager(),game->getVideoDriver(),"Media\\Model\\Ninja\\ninja.b3d");
	viethero->setPosition(irr::core::vector3df(1275.0f, 90.0f, 2675.0f));
	
	//this->light = game->getSceneManager()->addLightSceneNode();
	//this->light->setPosition(irr::core::vector3df(0.0f, 20.0f, 0.0f));
	//this->light->grab();

	//irr::video::SLight& lightData = this->light->getLightData();
	//lightData.AmbientColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	//lightData.DiffuseColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	//lightData.SpecularColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	//lightData.Radius = 20.0f;

	//
	game->getGuiEnvironment()->addStaticText(L"Nhấn phím ESC để quay lại menu.",rect<s32>(5, 5, 200, 100),false,true,0,-1,false); 

	//setup global camera
	{
		//game->getCamera()->setPosition(irr::core::vector3df(1280.0f, 61.0f, 2700.0f));
	
	/*	irr::core::matrix4 m;
		m.setRotationDegrees(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		irr::core::vector3df upv(0.0f, 1.0f, 0.0f);
		m.transformVect(upv);
		game->getCamera()->setUpVector(upv);*/
		
	}

	//Setup HUD
	this->hud = new HUD(game);

   terrain->getTerrain()->getMeshBufferForLOD(buffer, 0); 

   mesh.addMeshBuffer(&buffer);
   mesh.recalculateBoundingBox();

   selector = game->getSceneManager()->createOctTreeTriangleSelector(&mesh, terrain->getTerrain(), 128);
   terrain->getTerrain()->setTriangleSelector(selector);

   anim = game->getSceneManager()->createCollisionResponseAnimator(
	   selector, game->getCamera(), core::vector3df(60,100,60),
      core::vector3df(0,0,0),
      core::vector3df(0,50,0));
   
   game->getCamera()->addAnimator(anim);
   anim->drop(); anim = 0; 

   selectedSceneNode = 0;

   coll= game->getSceneManager()->getSceneCollisionManager();;

   
	/*animmy = game->getSceneManager()->createCollisionResponseAnimator(selector, viethero->getMesh(),core::vector3df(5, 1, 5),core::vector3df(0, -0.3, 0), core::vector3df(0, 0, 0));
	viethero->getMesh()->addAnimator(anim);
	animmy->drop(); animmy = 0; */

	selector->drop(); selector = 0;				
}

void LevelOneState::onLeave(Game* game)
{
	if (light)
	{
		this->light->remove();
		this->light->drop();
		this->light = 0;
	}

	game->getGuiEnvironment()->clear();
	
	if(viethero)
	{
		delete viethero;
		this->viethero = 0;
	}
	
	if(terrain)
	{
		delete terrain;
		this->terrain = 0;
	}

	if (hud)
	{
		delete hud;
		hud = 0;
	}
}

const bool LevelOneState::onEvent(Game* game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
		case irr::EET_GUI_EVENT:
			if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
			{				
				game->getCamera()->setPosition(collisionPoint);
				printf("%f....", collisionPoint.getDistanceFrom(vector3df(0,0,0)));
			}
		break;

		case irr::EET_MOUSE_INPUT_EVENT:
			if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
			{				
				if(event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
				{
				    printf("%f %f %f....", collisionPoint.getDistanceFrom(vector3df(0,0,0)));
					viethero->walk();
				}
			}
		break;
		
		
		case irr::EET_KEY_INPUT_EVENT:
		{
			if (!event.KeyInput.PressedDown)
			{
				if (event.KeyInput.Key == irr::KEY_ESCAPE)
				{
					game->setState(game->findGameState("intro"));
					return true;
				}
			}
	    }
		break;

		default: return false;
	}

	return false;
}

void LevelOneState::onUpdate(Game* game)
{
	this->hud->update(game);
	
	core::line3df ray(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition()));

	if(1)
		if(game->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrain->getTerrain()->getTriangleSelector(), collisionPoint, collisionTriangle)){
			int p=0;			
			viethero->move(collisionPoint); 
		}
	
		game->getCamera()->setPosition(viethero->getPosition() + vector3df(-10,-10,-10));
		game->getCamera()->setTarget(viethero->getPosition() + vector3df(-10,-10,-10));
				
	//smgr->getSceneCollisionManager()->getCollisionPoint(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition(),game->getSceneManager()->getActiveCamera()),selector,intersection,tri);
}

#endif /*INTROSTATE_H_*/
