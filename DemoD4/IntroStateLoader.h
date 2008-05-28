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
	bool isCompleted = false;

	//unsigned int currentResourceID = 0;

	//! Get total resource to calculate loading percent loadingPercent = (currentResourceID / totalResource)/100
	//unsigned int totalResource = n;

	//ProgressBar *progressBar;

public:
	IntroStateLoader();
	virtual ~IntroStateLoader();
	virtual void onEnter(Game* game);
	virtual void onLeave(Game* game);
	virtual const bool onEvent(Game* game, const irr::SEvent& event);
	virtual void onUpdate(Game* game);

	//! Load resource one resource each called until all resources are loaded
	void loadResource();
};

IntroStateLoader::void loadResource()
{
	//switch(currentResourceID)
	{
		//case 0: 
			{
				//! TODO load resource dau tien;


				//! Inscrese current resource id counter
				//currentResourceID ++;
				//break;
			}

		//...

		//! TODO : Complete function
		//case n: 
			{
				//! TODO load resource cuoi cung;


				//! Danh dau la da load xong resource
				//isCompleted = true;
				//break;
			}
	}
}

IntroStateLoader::IntroStateLoader()  
//										: progressBar(0)
{
//	progressBar = new ProgressBar(init variable);
}

IntroStateLoader::~IntroStateLoader()
{
	//if(progressBar)
	//{
	//	delete progressBar;	
	//}
}

void IntroStateLoader::onEnter(Game* game)
{
	//! TODO Display loading screen or some game intro image

	//! TODO Init progress bar
}

void IntroStateLoader::onUpdate(Game* game)
{

	//! TODO Change to introState if loading progress is completed
	if(isCompleted)
	{
		game->setState(game->findGameState("intro"));
		return;
	}

	//else
	{
		//! Call loading resource function
		//loadResource();

		//! TODO Update progress bar
		//progressBar->update();
	}
}

void IntroStateLoader::onLeave(Game* game)
{
	/*if(progressBar)
	{
		delete progressBar;
		progressBar = 0;
	}*/
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
				//! Enable user exit to Windows if he click ESC button when loading state
				if (event.KeyInput.Key == irr::KEY_ESCAPE)
				{
					game->getDevice()->closeDevice();
					return true;
				}
			}
	    }
		break;

		default: return false;
	}
	return false;
}

#endif /*INTROSTATELOADER_H_*/
