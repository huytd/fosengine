#include "EntityManager.h"
#include "Game.h"

sgfEntityManager::sgfEntityManager(sgfCore* core)
	:sgfCoreComponent(core)
{
}

sgfEntityManager::~sgfEntityManager()
{
	generalEvent.fire(SEntityEvent(SEntityEvent::EST_GameEnd));
}

bool sgfEntityManager::isActive(sgfEntity* entity)
{
	return entity->active;
}

void sgfEntityManager::setActive(sgfEntity* entity,bool active)
{
	if((entity->active)&&(!active))//turn off
	{
		tempDelegate.bind(entity,&sgfEntity::onEvent);
		updateEvent.removeDelegate(&tempDelegate);
	}
	if((!(entity->active))&&(active))//turn on
	{
		updateEvent.addDelegate(new sgfMethodDelegate<sgfEntity,SEntityEvent>(entity,&sgfEntity::onEvent));
	}
	entity->active=active;
}

void sgfEntityManager::addEntity(sgfEntity* entity)
{
	entity->manager=this;//let the entity know that this is its manager
	entity->onEvent(SEntityEvent(SEntityEvent::EST_Add));
	entities.push_back(sgfPtr<sgfEntity>(entity));
	generalEvent.addDelegate(new sgfMethodDelegate<sgfEntity,SEntityEvent>(entity,&sgfEntity::onEvent));
}

unsigned int sgfEntityManager::getNumEntity() const
{
	return (unsigned int)(entities.size());
}

void sgfEntityManager::removeEntity(sgfEntity* entity)
{
	entity->onEvent(SEntityEvent(SEntityEvent::EST_Remove));
	entity->manager=0;
	setActive(entity,false);
	entities.remove(sgfPtr<sgfEntity>(entity));
	tempDelegate.bind(entity,&sgfEntity::onEvent);
	generalEvent.removeDelegate(&tempDelegate);
}

void sgfEntityManager::clear()
{
	generalEvent.removeAll();
	updateEvent.removeAll();	
	entities.clear();
}

void sgfEntityManager::update(SFrameEvent& arg)
{
	updateEvent(SEntityEvent(SEntityEvent::EST_Update,arg.deltaTime));
}

void sgfEntityManager::loadLevel(sgfLevel* level)
{
	sgfPtr<sgfLevel> levelToLoad(level);//fix for ref counting madness
	generalEvent(SEntityEvent(SEntityEvent::EST_LevelEnd));
	if(currentLevel.getPtr())
	{
		currentLevel->onExit(this);
		previousLevel=currentLevel;
	}

	currentLevel=levelToLoad;
	currentLevel->onEnter(this);
	generalEvent(SEntityEvent(SEntityEvent::EST_LevelStart));
}

sgfLevel* sgfEntityManager::getCurrentLevel() const
{
	return currentLevel.getPtr();
}

sgfLevel* sgfEntityManager::getPreviousLevel() const
{
	return previousLevel.getPtr();
}