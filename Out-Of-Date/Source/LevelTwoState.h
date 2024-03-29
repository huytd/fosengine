#ifndef LEVELTWOSTATE_H_
#define LEVELTWOSTATE_H_

#include <IAnimatedMeshSceneNode.h>
#include <ILightSceneNode.h>
#include <ICameraSceneNode.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>
#include "CCharacter.h"
#include "Terrain.h"
#include "shadermaterial.h"
#include "shadergroup.h"

class LevelTwoState : public State<Game, irr::SEvent>
{

private:
	irr::scene::ILightSceneNode* light;
	
	CCharacter* hero;

	Terrain* terrain;

	ShaderGroup* hdr;
public:

	LevelTwoState();

	virtual ~LevelTwoState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* game, const irr::SEvent& event);

	virtual void onUpdate(Game* game);
};

LevelTwoState::LevelTwoState() :
	light(0), hero(0), terrain(0)
{

}

LevelTwoState::~LevelTwoState()
{
	if (light)
	{
		light->remove();
		light->drop();
	}

	if(hero)
		delete hero;
	

	if(terrain)
		delete terrain;
	
}

void LevelTwoState::onEnter(Game* game)
{
	if (game->getConfiguration().getHDREnable() == true)
	{
		hdr = new ShaderGroup(game->getDevice(), game->getSceneManager());
	}
	//setup terrain
	terrain = new Terrain(game);

	//Setup hero character
	hero = new CCharacter(game->getDevice(),game->getSceneManager(),game->getVideoDriver(),"Media\\Model\\vh\\vh.b3d");
	hero->setPosition(irr::core::vector3df(1275.0f, 60.0f, 2675.0f));
	
	//this->light = game->getSceneManager()->addLightSceneNode();
	//this->light->setPosition(irr::core::vector3df(0.0f, 20.0f, 0.0f));
	//this->light->grab();

	//irr::video::SLight& lightData = this->light->getLightData();
	//lightData.AmbientColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	//lightData.DiffuseColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	//lightData.SpecularColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	//lightData.Radius = 20.0f;

	game->getGuiEnvironment()->addStaticText(game->getConfiguration().getNoidung().c_str(),rect<s32>(5, 5, 200, 100),false,true,0,-1,false); 

	//setup global camera
	{
		game->getCamera()->setPosition(irr::core::vector3df(1280.0f, 61.0f, 2700.0f));
	
		irr::core::matrix4 m;
		m.setRotationDegrees(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		irr::core::vector3df upv(0.0f, 1.0f, 0.0f);
		m.transformVect(upv);
		game->getCamera()->setUpVector(upv);
		
	}
}

void LevelTwoState::onLeave(Game* game)
{
	if (light)
	{
		this->light->remove();
		this->light->drop();
		this->light = 0;
	}

	//game->getGuiEnvironment()->addStaticText(L"",rect<s32>(5, 5, 200, 100),false,true,0,-1,false); 
	
	if(hero)
	{
		delete hero;
		this->hero = 0;
	}
	
	if(terrain)
	{
		delete terrain;
		this->terrain = 0;
	}

	if(hdr)
	{
		delete hdr;
		this->hdr = 0;
	}

	game->getGuiEnvironment()->clear();
}

const bool LevelTwoState::onEvent(Game* game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
		case irr::EET_GUI_EVENT:
			if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
			{				
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

void LevelTwoState::onUpdate(Game* game)
{
	//game->drawBoundingBoxes(fighterNode, true, false);
	if (game->getConfiguration().getHDREnable() == true)	hdr->render();
}

#endif /*INTROSTATE_H_*/
