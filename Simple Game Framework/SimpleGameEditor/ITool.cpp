#include "ITool.h"

ITool::ITool()
:toggle(true)
{
	mouseDelegate=new sgfMethodDelegate<ITool,SMouseEvent>(this,&ITool::onMouse);
}
ITool::~ITool()
{
}
void ITool::onRemove()
{
	manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(mouseDelegate);	
}
void ITool::onMouse(SMouseEvent& mouseEvent)
{

}

void ITool::registerMouse()
{
	manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(mouseDelegate);
}
void ITool::unregisterMouse()
{
	manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(mouseDelegate);
}
void ITool::onAdd(){};
void ITool::onActivate(){};
void ITool::onDeactivate(){};