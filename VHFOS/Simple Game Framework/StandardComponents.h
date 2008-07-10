#ifndef _STANDARD_COMPONENTS_H_
#define _STANDARD_COMPONENTS_H_

#include "EntityComponent.h"
#include "Entity.h"
#include <irrlicht.h>
#include "Event.h"

/// \brief A component for objects that have transformation
class sgfTransformComponent:public sgfEntityComponent
{
public:
	const irr::core::matrix4& getTransform();
	void setTransform(irr::core::matrix4& transform);
	void setPosition(const irr::core::vector3df& position);
	const irr::core::vector3df getPosition();
	void setScale(const irr::core::vector3df& scale);
	const irr::core::vector3df getScale();
	void setRotation(const irr::core::vector3df& position);
	const irr::core::vector3df getRotation();
	sgfEvent<irr::core::matrix4> onSetTransform;
	sgfPtr<sgfDelegate<irr::core::matrix4>> onGetTransform;
	virtual const char* getName() const;
private:
	irr::core::matrix4 transform;
};

/// \brief A component for objects that is renderable
class sgfRenderComponent:public sgfEntityComponent
{
public:
	sgfRenderComponent(irr::scene::ISceneNode* node);
	virtual ~sgfRenderComponent();
	irr::scene::ISceneNode* getSceneNode() const;
	bool isVisible() const;
	void setVisible(bool visible);
	virtual void onAdd();
	virtual void onRemove();
	virtual const char* getName() const;
private:
	void onGetTransform(irr::core::matrix4& transform);
	void onSetTransform(irr::core::matrix4& transform);
	irr::scene::ISceneNode* node;
	sgfMethodDelegate<sgfRenderComponent,irr::core::matrix4> setTransformDelegate;
};

#endif