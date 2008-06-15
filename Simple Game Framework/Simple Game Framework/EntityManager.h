#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_

#include <list>
#include "Entity.h"
#include "Event.h"
#include "StandardEvents.h"
#include "CoreComponent.h"
#include "Level.h"
/// \brief The class that manages entity
class sgfEntityManager:public sgfCoreComponent
{
friend class sgfGame;
public:
	sgfEntityManager(sgfCore* core);
	~sgfEntityManager();
public:
	/// \brief Add an entity to the manager, call sgfEntity::onAdd
	/// \see sgfEntity::onAdd
	void addEntity(sgfEntity* entity);
	/// \brief Remove an entity, call sgfEntity::onRemove
	/// \see sgfEntity::onRemove
	void removeEntity(sgfEntity* entity);
	/// \return the number of entity
	unsigned int getNumEntity() const;
	/// \brief Remove all entity
	void clear();
	/// \param entity The entity to check for activeness
	/// \return Whether it is active
	bool isActive(sgfEntity* entity);
	/// \brief Make an enitity active or inactive
	/// \param entity The entity to activate
	/// \param active The value
	/// \see sgfEntity::update
	void setActive(sgfEntity* entity, bool active);
	/// \brief Load a level
	void loadLevel(sgfLevel* level);
	
	sgfLevel* getCurrentLevel() const;
	sgfLevel* getPreviousLevel() const;
	/// \brief Update the entity system
	/// \remarks The framework call this for you
	void update(SFrameEvent& arg);

private:
	void endGame();

	sgfPtr<sgfLevel> currentLevel;
	sgfPtr<sgfLevel> previousLevel;
	std::list<sgfPtr<sgfEntity>> entities;
	sgfEvent<SEntityEvent> generalEvent;
	sgfEvent<SEntityEvent> updateEvent;
	sgfMethodDelegate<sgfEntity,SEntityEvent> tempDelegate;
};

#endif