#include "EntityManager.h"
#include "Game.h"

sgfEntityManager::sgfEntityManager(sgfCore* core)
	:sgfCoreComponent(core)
{
}

sgfEntityManager::~sgfEntityManager()
{
}

void sgfEntityManager::endGame()
{
//notify all entites about the end of the game
	generalEvent(SEntityEvent(SEntityEvent::EST_LevelEnd));
	if(currentLevel.getPtr())
		currentLevel->onExit(this);
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

sgfEntity* sgfEntityManager::getEntityByClassName(const char* className) const
{
	std::list<sgfPtr<sgfEntity>>::const_iterator i;
	for(i=entities.begin();i!=entities.end();i++)
	{
		if(!strcmp(className,(*i)->getClassName()))
			return *i;
	}
	return 0;
}

const std::list<sgfEntity*>& sgfEntityManager::getEntityCollectionByClassName(const char* className) const
{
	entityCollection.clear();
	std::list<sgfPtr<sgfEntity>>::const_iterator i;
	for(i=entities.begin();i!=entities.end();i++)
	{
		if(!strcmp(className,(*i)->getClassName()))
			entityCollection.push_back(*i);
	}
	return entityCollection;
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
	if(currentLevel.getPtr())//if it is null
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
	return currentLevel;
}

sgfLevel* sgfEntityManager::getPreviousLevel() const
{
	return previousLevel;
}