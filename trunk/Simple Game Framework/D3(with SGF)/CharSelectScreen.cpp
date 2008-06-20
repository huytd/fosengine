#include "CharSelectScreen.h"
#include "Utility.h"

CharSelectScreen::CharSelectScreen()
{
	onGUI.addRef();
	onGUI.bind(this,&CharSelectScreen::onGUIEvent);
}

void CharSelectScreen::onEnter(sgfEntityManager *emgr)
{
	this->emgr=emgr;
	Utility::setFont(emgr,"font/myfont.xml");
	env = emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	irr::gui::ICursorControl* cursor = emgr->getCore()->getGraphicDevice()->getCursorControl();
	irr::core::dimension2di screenSize = env->getVideoDriver()->getScreenSize();
	//bgimg = env->addImage(env->getVideoDriver()->getTexture("textures/CharSel.jpg"),irr::core::position2di(0,0),false,0,-1,0);
	bgimg = env->addImage(irr::core::rect<irr::s32>(0,0,screenSize.Width, screenSize.Height),0,-1,0);
	bgimg->setImage(env->getVideoDriver()->getTexture("textures/CharSel.jpg"));
	bgimg->setScaleImage(true);
	
}

void CharSelectScreen::onExit(sgfEntityManager *emgr)
{

}

void CharSelectScreen::onGUIEvent(irr::SEvent::SGUIEvent &args)
{

}
