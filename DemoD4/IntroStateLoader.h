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

#include "GUI/CGUIProgressBar.h"
#include "GUI/CImageGUISkin.h"
#include "GUI/SkinLoader.h"

class IntroStateLoader : public State<Game, irr::SEvent>
{

private:
	bool isCompleted;

	unsigned int currentResourceID;

	//! Get total resource to calculate loading percent loadingPercent = (currentResourceID / totalResource)/100
	unsigned int totalResource;

	gui::CGUIProgressBar* progressBar;

public:
	IntroStateLoader();
	virtual ~IntroStateLoader();
	virtual void onEnter(Game* game);
	virtual void onLeave(Game* game);
	virtual const bool onEvent(Game* game, const irr::SEvent& event);
	virtual void onUpdate(Game* game);

	//! Load resource one resource each called until all resources are loaded
	void loadResource(Game* game);
};

void IntroStateLoader::loadResource(Game *game)
{
	switch(currentResourceID)
	{
		case 0: 
			{
				//! load resource dau tien;
				game->getSceneManager()->loadScene("media\\model\\intromap.irr");

				//! Inscrese current resource id counter
				currentResourceID ++;
				break;
			}

		//...

		//! TODO : Complete function
		//case n: 
			{
				//! TODO load resource cuoi cung;


				
				//break;
			}
	}
	if(currentResourceID == totalResource)
	{
		//! Danh dau la da load xong resource
		isCompleted = true;
	}
}

IntroStateLoader::IntroStateLoader()  
										: progressBar(0)
{

}

IntroStateLoader::~IntroStateLoader()
{


	if(progressBar)
	{
		progressBar->remove();
		progressBar->drop();	
	}
}

void IntroStateLoader::onEnter(Game* game)
{	
	isCompleted = false;
	totalResource = 1;
	currentResourceID = 0;

	//! Load new GUI system
	io::IFileSystem* filesys = game->getDevice()->getFileSystem();
    gui::SImageGUISkinConfig guicfg = LoadGUISkinFromFile(filesys, game->getVideoDriver(), "media\\gui\\gui.cfg");
    gui::CImageGUISkin* skin = new gui::CImageGUISkin(game->getVideoDriver(), game->getGuiEnvironment()->getSkin());
    skin->loadConfig(guicfg);

	game->getGuiEnvironment()->setSkin(skin);
	

	//! TODO Display loading screen or some game intro image

	//! TODO Init progress bar
	progressBar = new gui::CGUIProgressBar(game->getGuiEnvironment()->getRootGUIElement(), game->getGuiEnvironment(), core::rect<s32>(10,150,210,180));
    progressBar->setProgress(0.0f);
}

void IntroStateLoader::onUpdate(Game* game)
{
	//! TODO Change to introState if loading progress is completed
	if(isCompleted)
	{
		printf("\n\n Loaded......... \n\n");
		game->setState(game->findGameState("intro"));				
	}
	//else
	{
		//! Call loading resource function
		loadResource(game);

		//! TODO Update progress bar
		//progressBar->update();
	}
}

void IntroStateLoader::onLeave(Game* game)
{
	game->getGuiEnvironment()->clear();

	if(progressBar)
	{
		progressBar->remove();
		progressBar->drop();
		progressBar = 0;
	}
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
