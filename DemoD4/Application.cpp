/**
 * \Summary Application manager
 * \Filename: application.cpp
 * \Encoding: UTF-8
 * \Tabsize: 8
 * \Indentation: 4
 * \CreatedDate: 18:33 2008/05/26
 * \InitializedBy: Irrlicht Forum  - randomMesh
 * \CreatedBy: FOSP Team
 * \Copyright: FOS Project
 */
#include "Application.h"

#include "Game.h"
#include "GlobalState.h"
#include "IntroState.h"
#include "LevelOneState.h"

Application::Application() :
	device(0)
#ifdef _SOUND
	, soundEngine(0)
#endif
{
	
}

//! Init application
//! \return Bool value, return false if cannot init application, return true if application inited
const bool Application::init()
{
	//read the configuration file
	this->configuration.read();
	
	//init Irrlicht engine
	this->device = irr::createDeviceEx(this->configuration.getIrrlichtParams());
	
	if (!this->device)
	{
		printf("Could not startup Irrlicht.\n");
		return false;
	}
	
#ifdef _SOUND
	// init irrKlang engine
	this->soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine)
	{
		printf("Could not startup irrKlang.\n");
		this->configuration.setSoundEnabled(false);
	}
#endif
	

	return true;
}

//! Start and run application
const void Application::run()
{	
	//! create global game state
	GlobalState global;

	//! create all other game states the game can be in
	IntroState intro;
	LevelOneState levelone;

	//! create game (it's the state machine)
	Game game(
		this->configuration,
		this->device
#ifdef _SOUND
		, this->soundEngine
#endif
	);

	//! drop irrlicht and irrklang device since game grabbed them
	this->device->drop();
#ifdef _SOUND
	this->soundEngine->drop();
#endif
	
	//! register game states
	game.addGameState("intro", &intro);
	game.addGameState("levelone", &levelone);
	
	//! setup the state machine
	game.setGlobalState(&global);

	//! start the game with an intro
	game.setState(&intro);
	
	//! main loop
	while (game.isRunning())
	{
		//! update game timer
		game.tick();

		//printf("     elapssed=%f",game.getElapsed());
		//! update the state machine (aka rendering and event receiving)
		game.onUpdate();
	}
}
