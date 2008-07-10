#include "Entity.h"
#include "EntityManager.h"
//#include "GameMonkey/gmMachine.h"

sgfEntity::sgfEntity()
:active(0),manager(0),persistent(0)
{	
}

sgfEntity::~sgfEntity()
{
}

void sgfEntity::onEvent(SEntityEvent& param)
{
	switch(param.type)
	{
		case SEntityEvent::EST_Update:
			update(param.deltaTime);
		break;
		case SEntityEvent::EST_Add:
			onAdd();
		break;
		case SEntityEvent::EST_Remove:
			onRemove();
		break;
		case SEntityEvent::EST_LevelEnd:
			onLevelEnd();
		break;
		case SEntityEvent::EST_LevelStart:
			onLevelStart();
		break;
		//case SEntityEvent::EST_GameEnd:
		//break;
	}
	//entityEvent(param);
}

void sgfEntity::update(float deltaTime)
{
}

void sgfEntity::onLevelEnd()
{
	if(!persistent)
		manager->removeEntity(this);
}

void sgfEntity::onLevelStart()
{
}

const char* sgfEntity::getClassName() const
{
	return "unknown";
}
/*sgfEntityComponent* sgfEntity::getComponent(const char* name) const
{
	componentsType::const_iterator component;
	component=components.find(const_cast<char*>(name));
	if(component==components.end())
		return NULL;
	else
		return (*component).second.getPtr();

}

sgfEntityComponent* sgfEntity::addComponent(sgfEntityComponent* component)
{
	std::pair<char*,sgfPtr<sgfEntityComponent>> pair(const_cast<char*>(component->getName()),sgfPtr<sgfEntityComponent>(component));
	//the pair has a reason to be here
	//when sgfPtr is destroyed, it release the reference
	sgfEntityComponent* myComponent=getComponent(component->getName());
	if(!myComponent)
	{
		component->entity=this;
		components.insert(pair);
		component->onAdd();
		return component;
	}
	else
	{
		return myComponent;
	}
}

void sgfEntity::removeComponent(const char* name)
{
	sgfEntityComponent* component=getComponent(const_cast<char*>(name));
	components.erase(const_cast<char*>(name));
	component->onRemove();
	component->entity=0;
}

sgfEvent<SEntityEvent>* sgfEntity::getEntityEvent() const
{
	return const_cast<sgfEvent<SEntityEvent>*>(&entityEvent);
}*/

