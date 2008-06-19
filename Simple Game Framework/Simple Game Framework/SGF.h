#ifndef _SGF_H_
#define _SGF_H_

/*! \mainpage Simple game framework(SGF)
 * \section intro Introduction
 * This framework is part of the FOS project <BR>
 * Its aim is to make game development easier for beginners and hobbyist programmers
 * \section install Installation
 * Add this project's folder to the header list of your editor
 * Add the library to the library list
 * \section usage How to use
 * \subsection step1 Step1:
 * #include "SGF.h" where you need to use the framework
 * \subsection step2 Step2:
 * Implement your own sgfGame, see SGF test for more details
 * \subsection step3 Step3:
 *	In your "main.cpp" or the file that has entry point:.
 *	\code
 *	#define USE_SGF// to let SGF knows that you are using it
 *	#include <SGF.h>
 *	#include "TestGame.h"// the header of your implementation of sgfGame
 *	GAME_ENTRY_POINT(CTestGame)// Entry point of the game
 *  \endcode
 *
 */

#ifdef _MSC_VER
#pragma comment(lib,"SGF.lib")
#pragma comment(lib,"irrlicht.lib")
#endif

#include "Game.h"
#include "ScriptVM.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Event.h"
#include "Core.h"
#include "InputManager.h"
#include "IrrLevel.h"
//#include "StandardComponents.h"
///\def SGF_LAZY
///\brief define this symbol to make all namespace members of Irrlicht global(not recommended)

#ifdef SGF_LAZY
using namespace irr;
using namespace video;
using namespace io;
using namespace scene;
using namespace core;
#endif

/// \brief Should be used in your "main.cpp"
/// \param gameClass Your implementation of sgfGame
#define GAME_ENTRY_POINT(gameClass) \
	int main(int argc, char* argv[]) \
	{ \
		gameClass game; \
		game.run(); \
		return 0; \
	} \

#endif