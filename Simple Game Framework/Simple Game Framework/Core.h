#ifndef _SGF_CORE_H_
#define _SGF_CORE_H_

#include <irrlicht.h>
#include "EntityManager.h"
#include "StandardEvents.h"
#include "ScriptVM.h"
#include "Configuration.h"
#include "sgfPtr.h"
#include "InputManager.h"
#include "GuiSkin/CGUITexturedSkin.h"
#include "DynamicVar.h"
#include "Physic.h"

/*! \brief The "heart" of the framework, provide access to all the components
 * \remarks It's not singleton, however, there should be only one core in a game
 */
class sgfCore
{
friend class sgfGame;
public:
	sgfCore();
	~sgfCore();
public:
	/*!\brief Initialize the core
	 *
	 */
	virtual void init(sgfGame* game);
	/*! \brief Return the graphic device
	 *\return The graphic device
	 */
	irr::IrrlichtDevice* getGraphicDevice() const;
	///\return The entityManager
	sgfEntityManager* getEntityManager() const;
	///\return the file system
	irr::io::IFileSystem* getFileSystem() const;
	///\return frame start event
	sgfEvent<SFrameEvent>* getFrameStartEvent() const;
	sgfEvent<SFrameEvent>* getFrameEndEvent() const;
	sgfEvent<irr::SEvent::SGUIEvent>* getGUIEvent() const;
	///\return inputManager
	sgfInputManager* getInputManager() const;
	///\return guiSkin
	irr::gui::CGUITexturedSkin* getGUISkin() const;
	///\return script machine
	sgfScriptVM* getScriptVM() const;
	///\return physic world
	sgfPhysicWorld* getPhysicWorld() const;
	/// \return The game object
	template <class gameClass>
	inline gameClass* getGame() const
	{
		return static_cast<gameClass*>(game);
	}
	inline sgfGame* getGame() const
	{
		return game;
	}
	/// \brief Multi-purpose varSet
	sgfDynamicVarSet globalVars;
	/// \brief Default configuration
	Configuration config;

private:
	sgfEvent<SFrameEvent>* getFrameEvent() const;
	bool inited;
	sgfPtr<sgfScriptVM> scriptVM;
	sgfPtr<sgfEntityManager> entityManager;
	irr::IrrlichtDevice* graphicDevice;
	irr::io::IFileSystem* fileSystem;
	sgfEvent<irr::SEvent::SGUIEvent> guiEvent;
	sgfEvent<SFrameEvent> frameEvent;
	sgfEventRelay<SFrameEvent> frameStartEvent;
	sgfEventRelay<SFrameEvent> frameEndEvent;
	sgfPtr<sgfInputManager> inputManager;
	sgfPtr<sgfPhysicWorld> physicWorld;
	irr::gui::CGUITexturedSkin* guiSkin;
	sgfGame* game;
};

#endif