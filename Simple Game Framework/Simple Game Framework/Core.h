#ifndef _SGF_CORE_H_
#define _SGF_CORE_H_

#include <irrlicht.h>
#include "EntityManager.h"
#include "StandardEvents.h"
//#include "ScriptVM.h"
#include "Configuration.h"
#include "sgfPtr.h"
#include "InputManager.h"
#include "GuiSkin/CGUITexturedSkin.h"
#include "DynamicVar.h"

/*! \brief The "heart" of the framework, provide access to all the components
 * \remarks It's not singleton, however, there should be only one core in a game
 */
class sgfCore
{
public:
	sgfCore();
	~sgfCore();
public:
	/*!\brief Initialize the core
	 *
	 */
	virtual void init();
	/*! \brief Return the graphic device
	 *\return The graphic device
	 */
	irr::IrrlichtDevice* getGraphicDevice() const;
	///\return The entityManager
	sgfEntityManager* getEntityManager() const;
	///\return the file system
	irr::io::IFileSystem* getFileSystem() const;
	///\return main frame event
	sgfEvent<SFrameEvent>* getFrameEvent() const;
	///\return frame start event
	sgfEvent<SFrameEvent>* getFrameStartEvent() const;
	sgfEvent<SFrameEvent>* getFrameEndEvent() const;
	///\return inputManager
	sgfInputManager* getInputManager() const;
	///\return guiSkin
	irr::gui::CGUITexturedSkin* getGUISkin() const;

	/// \brief Multi-purpose varSet
	sgfDynamicVarSet globalVars;
	/// \brief Default configuration
	Configuration config;
private:
	virtual void updateGraphic(SFrameEvent& data);
private:
	bool inited;
	sgfPtr<sgfEntityManager> entityManager;
	irr::IrrlichtDevice* graphicDevice;
	irr::io::IFileSystem* fileSystem;
	sgfEvent<SFrameEvent> frameEvent;
	sgfEventRelay<SFrameEvent> frameStartEvent;
	sgfEventRelay<SFrameEvent> frameEndEvent;
	sgfPtr<sgfInputManager> inputManager;
	irr::gui::CGUITexturedSkin* guiSkin;
};

#endif