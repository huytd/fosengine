#ifndef _sgfGame_H_
#define _sgfGame_H_

#include "irrlicht.h"
//#include "ScriptVM.h"
#include "Event.h"
#include "StandardEvents.h"
#include "Core.h"

/// \brief Base class for a game object
class sgfGame
{
public:
	sgfGame();
	~sgfGame();
public:
	/// \brief This method is called when the game start
	virtual void run();
	/// \brief stop the game
	virtual void quit();
	/// \return The core that this game is using
	sgfCore* getCore() const;

protected:
	/// \brief Whether the game is running
	bool running;
	/// \brief The core this game is using
	sgfCore core;
};

#endif