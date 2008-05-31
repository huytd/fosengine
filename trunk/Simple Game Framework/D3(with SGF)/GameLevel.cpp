#include "GameLevel.h"

using namespace irr;
using namespace gui;

GameLevel::GameLevel(const char* fileName)
:sgfIrrLevel(fileName)
{
	keyDelegate.addRef();
	keyDelegate.bind(this,&GameLevel::onKey);
}

GameLevel::~GameLevel()
{
}

void GameLevel::onEnter(sgfEntityManager* emgr)
{
	sgfIrrLevel::onEnter(emgr);
	this->emgr=emgr;
	emgr->getCore()->getGraphicDevice()->getGUIEnvironment()->addStaticText(L"Press esc to exit",core::rect<s32>(10,10,80,30));
	emgr->getCore()->getInputManager()->getKeyboardEvent()->addDelegate(&keyDelegate);

}

void GameLevel::onKey(SKeyboardEvent& args)
{
	if((args.keyEvent==SKeyboardEvent::EKS_PRESSED)&&(args.keyCode==irr::KEY_ESCAPE ))
	{
		emgr->loadLevel(emgr->getPreviousLevel());//go back
	}
}

void GameLevel::onExit(sgfEntityManager* emgr)
{
	sgfIrrLevel::onExit(emgr);
	emgr->getCore()->getGraphicDevice()->getGUIEnvironment()->clear();
	emgr->getCore()->getInputManager()->getKeyboardEvent()->removeDelegate(&keyDelegate);
}