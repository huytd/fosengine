#include "Level01.h"
Level01::Level01(const char* fileName)
:sgfIrrLevel(fileName)
{
	
}
void Level01::onEnter(sgfEntityManager* emgr)
{
	sgfIrrLevel::onEnter(emgr);
	this->emgr = emgr;
	env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();

	//HP and MP Bar.
	HPBar = new HealthManaBar(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	MPBar = new HealthManaBar(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	HPBar->setTexture("bar/HP.png");
	HPBar->setPosition(10,10);
	HPBar->setMaxValue(300);
	HPBar->setValue(190);
	MPBar->setTexture("bar/MP.png");
	MPBar->setPosition(10,36);
	MPBar->setMaxValue(150);
	MPBar->setValue(10);

	//Destroy object
	HPBar->drop();
	MPBar->drop();

	//! Get character node
	irr::scene::ISceneNode* characterNode = emgr->getCore()->globalVars["characterNode"].getAs<irr::scene::ISceneNode*>();

	//Map.
	map = new Map(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	map->setMapTexture("map/Worldmap.png");
	map->setCharacterTexture("map/character.png");
	map->setWorldSize(512,512);
	map->setCharPosition(characterNode->getPosition());
	map->drop();
}
void Level01::onExit(sgfEntityManager* emgr)
{
	sgfIrrLevel::onExit(emgr);
	env->clear();
}
