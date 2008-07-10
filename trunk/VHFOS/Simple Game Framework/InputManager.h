#ifndef _SGF_INPUT_MANAGER_H_
#define _SGF_INPUT_MANAGER_H_

#include "CoreComponent.h"
#include "Event.h"
#include <irrlicht.h>

typedef irr::EKEY_CODE EKEY_CODE;
/// \brief Parameter type for keyboard Event
struct SKeyboardEvent
{
	/// \brief The keycode of the key in this event
	EKEY_CODE keyCode;
	/// Type of event
	enum EKEY_EVENT
	{
		/// The key is pressed down
		EKS_PRESSED=0,
		/// Released
		EKS_RELEASED,
		/// Held
		EKS_HELD
	} keyEvent;
	SKeyboardEvent(EKEY_CODE keyCode=irr::KEY_KEY_A, EKEY_EVENT keyEvent=EKS_PRESSED)
		:keyCode(keyCode),keyEvent(keyEvent)
	{}
};


typedef irr::EMOUSE_INPUT_EVENT EMOUSE_EVENT;
/// \brief Parameter type for mouse event
struct SMouseEvent
{
	EMOUSE_EVENT mouseEvent;
	float wheel;
	int  x;
	int y; 
	SMouseEvent(EMOUSE_EVENT mouseEvent=irr::EMIE_LMOUSE_PRESSED_DOWN ,float wheel=0,int x=0, int y=0)
		:mouseEvent(mouseEvent),wheel(wheel),x(x),y(y)
	{}
};

/*struct SJoystickSignal
{
}*/

/// \brief This class receives input and generates input event
class sgfInputManager:public sgfCoreComponent, public irr::IEventReceiver
{
public:
	sgfInputManager(sgfCore* core);

protected:
	virtual ~sgfInputManager();

public:
	/// \return keyboard event
	sgfEvent<SKeyboardEvent>* getKeyboardEvent() const;
	/// \return mouse event
	sgfEvent<SMouseEvent>* getMouseEvent() const;
	/// \return Whether a key is pressed down
	bool isKeyPressed(EKEY_CODE key) const;
	/// \brief Event receiver for Irrlicht
	virtual bool OnEvent(const irr::SEvent& event);

private:
	sgfEvent<SKeyboardEvent> keyboardEvent;
	sgfEvent<SMouseEvent> mouseEvent;
	bool keyState[irr::KEY_KEY_CODES_COUNT];
};

#endif