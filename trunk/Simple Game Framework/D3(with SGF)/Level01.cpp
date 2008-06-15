#include "Level01.h"
Level01::Level01()
{
}
void Level01::onEnter(sgfEntityManager* emgr)
{
		this->emgr = emgr;
		env=emgr->getCore()->getGraphicDevice()->getGUIEnvironment();
	 //HP and MP Bar.
        HPBar = new HealthManaBar(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
		MPBar = new HealthManaBar(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
		HPBar->setTexture("bar/untitled.png");
		HPBar->setPosition(10,10);
		HPBar->setMaxValue(300);
		HPBar->setValue(190);
		MPBar->setTexture("bar/MP.png");
		MPBar->setPosition(10,36);
		MPBar->setMaxValue(150);
		MPBar->setValue(110);
		HPBar->drop();
		MPBar->drop();
	//World.
        emgr->getCore()->getGraphicDevice()->getSceneManager()->loadScene("levels/start.irr");

	//Map.
		map = new Map(rect<s32>(0,0,800,600),env,env->getRootGUIElement());
		map->setMapTexture("map/LevelOneState.jpg");
		map->setCharacterTexture("map/character.jpg");
		map->setWorldSize(10000,10000);
		map->setCharPosition(core::vector3df(5000,0,5000));
		map->drop();
}
void Level01::onExit(sgfEntityManager* emgr)
{
		env->clear();
}
