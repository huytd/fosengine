#ifndef INTROSTATELOADER_H_
#define INTROSTATELOADER_H_

#include <IAnimatedMeshSceneNode.h>
#include <ILightSceneNode.h>
#include <ICameraSceneNode.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>

class IntroStateLoader : public State<Game, irr::SEvent>
{

private:
	irr::scene::ILightSceneNode* light;

public:

	IntroStateLoader();

	virtual ~IntroStateLoader();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* game, const irr::SEvent& event);

	virtual void onUpdate(Game* game);
};

IntroStateLoader::IntroStateLoader() :
	light(0), 
	startButton(0), exitButton(0)
{

}

IntroStateLoader::~IntroStateLoader()
{
	if (light)
	{
		light->remove();
		light->drop();
	}
}

void IntroState::onEnter(Game* game)
{
	
	this->light = game->getSceneManager()->addLightSceneNode();
	this->light->setPosition(irr::core::vector3df(0.0f, 10.0f, 0.0f));
	this->light->grab();

	irr::video::SLight& lightData = this->light->getLightData();
	lightData.AmbientColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.DiffuseColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.SpecularColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.Radius = 20.0f;

	//setup global camera
	{
		game->getCamera()->setTarget(irr::core::vector3df(-10.0f, 50.0f, -26.0f));
		game->getCamera()->setPosition(irr::core::vector3df(0.0f, 30.0f, -106.0f));
		irr::core::matrix4 m;
		m.setRotationDegrees(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		irr::core::vector3df upv(0.0f, 1.0f, 0.0f);
		m.transformVect(upv);
		game->getCamera()->setUpVector(upv);
	}


}

void IntroState::onLeave(Game* game)
{
	this->light->remove();
	this->light->drop();
	this->light = 0;
	
	
}

const bool IntroState::onEvent(Game* game, const irr::SEvent& event)
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
			}
	    }
		break;

		default: return false;
	}

	return false;
}

void IntroState::onUpdate(Game* game)
{
	
}

#endif /*INTROSTATELOADER_H_*/
