#include <irrlicht.h>
#include "IrrLevel.h"
#include <string.h>
#include "EntityManager.h"
#include "Core.h"
#include "Entity.h"

using namespace irr;
using namespace scene;

std::map<const char*,sgfIrrLevel::createFunctionPtr,sgfIrrLevel::strCmp> sgfIrrLevel::createFunctions;

sgfIrrLevel::sgfIrrLevel(const char* FileName)
:fileName(_strdup(FileName))
{
}

sgfIrrLevel::~sgfIrrLevel(void)
{
	free(fileName);//free memory
}

void sgfIrrLevel::onEnter(sgfEntityManager* emgr)
{
	ISceneManager* smgr=emgr->getCore()->getGraphicDevice()->getSceneManager();
	smgr->loadScene(fileName);
	ISceneNode* root=smgr->getRootSceneNode();
	core::list<ISceneNode*> children=root->getChildren();
	for(core::list<ISceneNode*>::Iterator i=children.begin();i!=children.end();i++)
	{
		ISceneNode* current=(*i);
		std::map<const char*,createFunctionPtr,strCmp>::iterator function;
		function=sgfIrrLevel::createFunctions.find(current->getName());
		if(function!=sgfIrrLevel::createFunctions.end())
		{
			sgfIrrLevel::createFunctionPtr func;
			func=(*function).second;
			sgfEntity* entity=(func)(current);//call the creation function to create our entity
			if(entity)
				emgr->addEntity(entity);// add the entity
		}
	}
}

void sgfIrrLevel::onExit(sgfEntityManager* emgr)
{
	emgr->getCore()->getGraphicDevice()->getSceneManager()->clear();
}

void sgfIrrLevel::_registerClass(const char* className,sgfIrrLevel::createFunctionPtr function)
{
	sgfIrrLevel::createFunctions.insert(std::make_pair<const char*,sgfIrrLevel::createFunctionPtr>(className,function));
}