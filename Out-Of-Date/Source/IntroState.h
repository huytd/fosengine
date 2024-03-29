#ifndef INTROSTATE_H_
#define INTROSTATE_H_

#include <IAnimatedMeshSceneNode.h>
#include <ILightSceneNode.h>
#include <ICameraSceneNode.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>

class IntroState : public State<Game, irr::SEvent>
{

private:


	irr::scene::ILightSceneNode* light;

	irr::gui::IGUIButton* startButton;

	irr::gui::IGUIButton* exitButton;
	
public:

	IntroState();

	virtual ~IntroState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* game, const irr::SEvent& event);

	virtual void onUpdate(Game* game);


};

IntroState::IntroState() :
	light(0), 
	startButton(0), exitButton(0)
{

}

IntroState::~IntroState()
{

	if (light)
	{
		light->remove();
		light->drop();
	}

	if (startButton)
	{
		startButton->remove();
		startButton->drop();
	}

	if (exitButton)
	{
		exitButton->remove();
		exitButton->drop();
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
		game->getSceneManager()->setActiveCamera(game->getCamera());
		game->getCamera()->setTarget(irr::core::vector3df(-10.0f, 50.0f, -26.0f));
		game->getCamera()->setPosition(irr::core::vector3df(0.0f, 30.0f, -106.0f));
		irr::core::matrix4 m;
		m.setRotationDegrees(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		irr::core::vector3df upv(0.0f, 1.0f, 0.0f);
		m.transformVect(upv);
		game->getCamera()->setUpVector(upv);
	}

	//add gui
	{
		
		this->startButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(100), SY(80), SX(400), SY(200)),
			0, -1, L"Bắt đầu game [F1]", L"Nạp level 1");
		this->startButton->grab();

		this->exitButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(100), SY(255), SX(400), SY(400)),
			0, -1, L"Đóng [ESC]", L"Thoát về windows");
		this->exitButton->grab();
	}

	game->getSceneManager()->loadScene("media\\model\\intromap.irr");
}

void IntroState::onLeave(Game* game)
{
	this->light->remove();
	this->light->drop();
	this->light = 0;
	
	startButton->remove();
	startButton->drop();
	startButton = 0;
	
	if (exitButton)
	{
		exitButton->remove();
		exitButton->drop();
		exitButton = 0;
	}


}


const bool IntroState::onEvent(Game* game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
		case irr::EET_GUI_EVENT:
			if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
			{				
				if (event.GUIEvent.Caller == this->exitButton)
				{
					game->getDevice()->closeDevice();
					return true;
				}
				else if (event.GUIEvent.Caller == this->startButton)
				{
					game->setState(game->findGameState("levelone"));
					return true;
				}
			}
		break;
		
		
		case irr::EET_KEY_INPUT_EVENT:
		{
			if (!event.KeyInput.PressedDown)
			{
				if (event.KeyInput.Key == irr::KEY_ESCAPE)
				{
					game->getDevice()->closeDevice();
					return true;
				}
				else if (event.KeyInput.Key == irr::KEY_F1)
				{
					game->setState(game->findGameState("levelone"));
					return true;
				}
			}
	    }
		break;

		default: return false;
	}

	return false;
}


void IntroState::onUpdate(Game* game)
{
	// animate sprites
	//sprite2.play();
}

#endif /*INTROSTATE_H_*/
