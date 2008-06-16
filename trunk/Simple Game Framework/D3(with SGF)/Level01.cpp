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
	MPBar->setValue(110);

	//Destroy object
	HPBar->drop();
	MPBar->drop();

	//Map.
	map = new Map(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
	map->setMapTexture("map/LevelOneState.jpg");
	map->setCharacterTexture("map/character.jpg");
	map->setWorldSize(10000,10000);
	map->setCharPosition(core::vector3df(5000,0,5000));
	map->drop();

	//World.
	//emgr->getCore()->getGraphicDevice()->getSceneManager()->loadScene("levels/start.irr");

}
void Level01::onExit(sgfEntityManager* emgr)
{
	sgfIrrLevel::onExit(emgr);
	env->clear();
}
