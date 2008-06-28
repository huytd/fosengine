#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <map>
#include <string>
#include "Event.h"
#include "StandardEvents.h"
//#include "EntityComponent.h"
//! \brief "Building block" of a game world, all game objects must derive from this class
/* It should be added to the enitty manager after creation
 * \remarks This object is reference counted
 */
class sgfEntity:public sgfObject
{
	/*struct strCmp 
	{
		bool operator()( const char* s1, const char* s2 ) const 
		{
		  return strcmp( s1, s2 ) < 0;
		}
	};*/
friend class sgfEntityManager;
protected:
	/// \brief constructor, you should not do initialization here.
	sgfEntity();
	/// \brief destructor, you should not do deinitialization here.
	virtual ~sgfEntity();
public:
	/// \brief Get a component with a given name
	/// \return The component or null if not found
	//sgfEntityComponent* getComponent(const char* name) const;
	/// \brief Add a component if it's not already added
	//sgfEntityComponent* addComponent(sgfEntityComponent* component);
	/// \brief Remove a component if found
	//void removeComponent(const char* name);
	/// \brief Get the entity event( used by components if they want to listen to entity events)
	//sgfEvent<SEntityEvent>* getEntityEvent() const;

	/// \return the class name of this entity;
	virtual const char* getClassName() const;
protected:
	/// \brief The event handler
	virtual void onEvent(SEntityEvent& param);
	/// \brief This method is called every frame if the object is active
	/// \see sgfEntityManager::setActive
	virtual void update(float deltaTime);
	/// \brief Called when an entity is added to the entityManager.
	/// This is where initialization should be done.
	virtual void onAdd()=0;
	/// \brief Called when an entity is removed from the entityManager
	/// This is where deinitialization should be done
	virtual void onRemove()=0;
	/// \brief Called when a level ends
	virtual void onLevelEnd();
	/// \brief Called when a level starts
	virtual void onLevelStart();
protected:
	/// \brief The entityManager that this entity belongs to
	sgfEntityManager* manager;
private:
	bool active;
public:
	bool persistent;
	//typedef std::map<char*,sgfPtr<sgfEntityComponent>,strCmp> componentsType;
	//componentsType components;
	//sgfEvent<SEntityEvent> entityEvent;
};

#endif