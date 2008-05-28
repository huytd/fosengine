/**
 * \Summary Display loading screen and loading progress bar and load resource for intro state
 * \Filename: IntroStateLoader.h
 * \Encoding: UTF-8
 * \Tabsize: 8
 * \Indentation: 4
 * \CreatedDate: 11:32 2008/05/28
 * \InitializedBy: Do Quoc Khanh - doqkhanh
 * \CreatedBy: FOSP Team
 * \Copyright: FOS Project
 */
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

public:
	IntroStateLoader();
	virtual ~IntroStateLoader();
	virtual void onEnter(Game* game);
	virtual void onLeave(Game* game);
	virtual const bool onEvent(Game* game, const irr::SEvent& event);
	virtual void onUpdate(Game* game);
};

IntroStateLoader::IntroStateLoader() 
{
}

IntroStateLoader::~IntroStateLoader()
{
}

void IntroStateLoader::onEnter(Game* game)
{
	
	//! TODO Display loading screen or some game intro image
	//! TODO Init progress bar

}

void IntroStateLoader::onUpdate(Game* game)
{
	//! TODO Update progress bar

	//! TODO Change to introState if loading progress is completed
}

void IntroStateLoader::onLeave(Game* game)
{
}

const bool IntroStateLoader::onEvent(Game* game, const irr::SEvent& event)
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
				//! TODO Enable user exit to main menu if he click ESC button when loading state
			}
	    }
		break;

		default: return false;
	}
	return false;
}


#endif /*INTROSTATELOADER_H_*/
