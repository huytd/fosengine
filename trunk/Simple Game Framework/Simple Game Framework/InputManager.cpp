#include "InputManager.h"
#include <iostream>
#include "Core.h"

sgfInputManager::sgfInputManager(sgfCore* core)
	:sgfCoreComponent(core)
{
	memset(keyState,0,sizeof(keyState));//reset all key's state to 0
}

sgfInputManager::~sgfInputManager()
{
}

sgfEvent<SKeyboardEvent>* sgfInputManager::getKeyboardEvent() const
{
	return const_cast<sgfEvent<SKeyboardEvent>*>(&keyboardEvent);
}

sgfEvent<SMouseEvent>* sgfInputManager::getMouseEvent() const
{
	return const_cast<sgfEvent<SMouseEvent>*>(&mouseEvent);
}

bool sgfInputManager::isKeyPressed(EKEY_CODE key) const
{
	if(key<irr::KEY_KEY_CODES_COUNT)
	{
		return keyState[key];
	}
	return false;
}

bool sgfInputManager::OnEvent(const irr::SEvent& event)
{
	if(event.EventType==irr::EET_KEY_INPUT_EVENT)
	{
		EKEY_CODE key=event.KeyInput.Key;
		if(key>=irr::KEY_KEY_CODES_COUNT)
			return false;//don't accept outragous key code from fu**ing keyboards
		SKeyboardEvent keySignal(key);
		if(event.KeyInput.PressedDown)
		{
			if(keyState[key])//if it was pressed before
			{
				keySignal.keyEvent=SKeyboardEvent::EKS_HELD;//it is held
			}
			else
			{
				keySignal.keyEvent=SKeyboardEvent::EKS_PRESSED;//it is pressed
			}
			keyState[key]=true;
		}
		else
		{
			keySignal.keyEvent=SKeyboardEvent::EKS_RELEASED;
			keyState[key]=false;
		}
		keyboardEvent(keySignal);//emit the signal
	}
	else if(event.EventType==irr::EET_MOUSE_INPUT_EVENT)
	{//mouse event should be simple
		SMouseEvent mouse(event.MouseInput.Event,event.MouseInput.Wheel,event.MouseInput.X,event.MouseInput.Y);//prepare the signal
		mouseEvent(mouse);//emit it
	}
	return false;
}