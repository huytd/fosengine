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
#include "SimpleCameraManager.h"

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



	

	core::triangle3df collisionTriangle;
	core::vector3df collisionPoint;
	core::vector3df targetPoint;


	SimpleCameraManager *cameraManager; 


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

	if(selector)
	{		
		selector->drop();	
	}

}

/*
 * Some method for new 3rd camera system 
 */

/// Move the scenenode
void moveN(irr::scene::ISceneNode *node, irr::core::vector3df vel) 
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	m.transformVect(vel);
	node->setPosition(node->getPosition() + vel);
} 


/// Make the camera look like 3rd camera
void makeCockpit(irr::scene::ICameraSceneNode* camera,
				 irr::scene::ISceneNode* node, 
				 irr::core::vector3df offset)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());

	irr::core::vector3df frv = irr::core::vector3df (0.0f, 0.0f, 1.0f);
	m.transformVect(frv);

	irr::core::vector3df upv = irr::core::vector3df (0.0f, 1.0f, 0.0f);
	m.transformVect(upv);

	m.transformVect(offset);

	camera->setPosition(node->getPosition() + offset); 
	camera->setUpVector(upv); 
	camera->setTarget(node->getPosition() + frv);
} 

/// Rotate the scenenode
void rotateN(irr::scene::ISceneNode *node, irr::core::vector3df rot)
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	irr::core::matrix4 n;
	n.setRotationDegrees(rot);
	m *= n;
	node->setRotation( m.getRotationDegrees() );
	node->updateAbsolutePosition();
}




void LevelOneState::onEnter(Game* game)
{
	//setup terrain
	terrain = new Terrain(game);

	//Setup viethero character
	viethero = new CCharacter(game->getDevice(),game->getSceneManager(),game->getVideoDriver(),"Media\\Model\\Ninja\\ninja.b3d");
	viethero->setPosition(irr::core::vector3df(1275.0f, 25.0f, 2675.0f));
	targetPoint = irr::core::vector3df(1275.0f, 60.0f, 2675.0f);

	game->getGuiEnvironment()->addStaticText(L"Nhấn phím ESC để quay lại menu.",rect<s32>(5, 5, 200, 100),false,true,0,-1,false); 

	//Setup HUD
	this->hud = new HUD(game);

	cameraManager = new SimpleCameraManager(viethero->getNode(), game->getSceneManager());
	game->getSceneManager()->setActiveCamera( cameraManager->getCam() ); 
	
	selector = game->getSceneManager()->createTerrainTriangleSelector(terrain->getTerrain(), 0);
	terrain->getTerrain()->setTriangleSelector(selector);

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

	if(selector)
	{		
		selector->drop();
		selector = 0;
	}
}


const bool LevelOneState::onEvent(Game* game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
	case irr::EET_GUI_EVENT:
		if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
		{				

		}
		break;

	case irr::EET_MOUSE_INPUT_EVENT:
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{				
			if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
			{
				core::line3df ray(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition()));

				if(game->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrain->getTerrain()->getTriangleSelector(), collisionPoint, collisionTriangle))
				{
					targetPoint = collisionPoint;
					viethero->walk();
				}
			}
			else if(event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
			{
				core::line3df ray(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition()));

				if(game->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrain->getTerrain()->getTriangleSelector(), collisionPoint, collisionTriangle))
				{
					viethero->attack(CCharacter::PowerAttack);
					
				}
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
			if (event.KeyInput.PressedDown)
			{
				if (event.KeyInput.Key == irr::KEY_UP)
				{
					moveN(viethero->getNode(), vector3df(0,0,5));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_DOWN)
				{
					moveN(viethero->getNode(), vector3df(0,0,-5));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_LEFT)
				{
					rotateN(viethero->getNode(),vector3df(0,10,0));
					return true;
				}
				if (event.KeyInput.Key == irr::KEY_RIGHT)
				{
					rotateN(viethero->getNode(),vector3df(0,-10,0));
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

	//Control main character
	viethero->move(targetPoint);
	float oldHeight = viethero->getPosition().Y;
	float newHeight = terrain->getTerrain()->getHeight(viethero->getPosition().X, viethero->getPosition().Z);
	
	
	if( (abs(newHeight-viethero->getPosition().Y) < 0.25))
	{
		viethero->setPosition(core::vector3df(viethero->getPosition().X, newHeight, viethero->getPosition().Z));
		printf("\n case 1");
	}
	else
	{
		if(newHeight-viethero->getPosition().Y > 0)
		{
			viethero->setPosition(core::vector3df(viethero->getPosition().X,oldHeight + 0.125f , viethero->getPosition().Z));
			//printf("\n case 2");
		}
		else if(newHeight-viethero->getPosition().Y < 0)
		{
			//printf("\n case 3");
			viethero->setPosition(core::vector3df(viethero->getPosition().X,oldHeight - 0.125f , viethero->getPosition().Z));
		}
			else 
			{
				//printf("\n case 4");	
				viethero->setPosition(core::vector3df(viethero->getPosition().X,oldHeight, viethero->getPosition().Z));
		}
		}

	if(cameraManager) cameraManager->Update();

	core::vector3df fore(0, 0, 1);	
	viethero->getNode()->getAbsoluteTransformation().rotateVect(fore);
	//cameraManager->getCam()->getAbsoluteTransformation().rotateVect(fore);
	core::vector3df rot1 = fore.getHorizontalAngle(); 

	game->setPlayerRotation(rot1.Y);

	/*=======================================*/
	/*
	
	game->getCamera()->setPosition(viethero->getPosition() + vector3df(-10, 10, 0));

	core::vector3df vim = core::vector3df(0,0,-15);
	irr::core::matrix4 m;
	m.setRotationDegrees(viethero->getRotate());
	m.transformVect(vim);
	game->getCamera()->setPosition(viethero->getPosition() + vim +  core::vector3df(0,15,0));
	game->getCamera()->updateAbsolutePosition();

	//game->getCamera()->setRotation(viethero->getRotate());

	//game->getCamera()->setTarget(targetPoint*vector3df(0, 0, 1));
	game->getCamera()->setTarget(viethero->getPosition()*vector3df(0,0,1));
	//smgr->getSceneCollisionManager()->getCollisionPoint(game->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(game->getDevice()->getCursorControl()->getPosition(),game->getSceneManager()->getActiveCamera()),selector,intersection,tri);
	*/
	/*=======================================*/
	// New 3rd camera system
	makeCockpit(game->getCamera(),viethero->getNode(),vector3df(0,20,-10)); //Gia tri offset de dieu chinh vi tri camera
	
}

#endif /*INTROSTATE_H_*/
