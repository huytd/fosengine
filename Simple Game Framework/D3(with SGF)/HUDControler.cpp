/**
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine
 * \Summary: Third Person Camera
 * \Filename: HUDControler.cpp
 * \Encoding: UTF-8
 * \CreatedDate: 2:17:PM 2008/06/15
 * \CreatedBy: FOSP Team 
 * \Copyright: FOS Project
 **/

#include "HUDControler.h"
using namespace irr;
using namespace scene;
using namespace core;
HUDControler::HUDControler(ISceneNode* targetNode)
{    
	    //this->targetNode = targetNode;		
} 

void HUDControler::onAdd()
{

}
void HUDControler::onLevelStart()
{
		//cursor = manager->getCore()->getGraphicDevice()->getCursorControl();
		//ISceneManager* smgr = manager->getCore()->getGraphicDevice()->getSceneManager();
	
		//manager->setActive(this,true);
}

void HUDControler::update(float deltaTime)
{	

}

void HUDControler::onRemove()
{
		//manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(&mouseDelegate);//Gỡ Event ra.
		//camera->remove();//đập cái camera.
}

