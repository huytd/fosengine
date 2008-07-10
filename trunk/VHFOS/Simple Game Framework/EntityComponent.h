#ifndef _ENTITY_COMPONENT_H_
#define _ENTITY_COMPONENT_H_

#include "sgfObject.h"

class sgfEntityComponent : public sgfObject
{
friend class sgfEntity;
public:
	sgfEntityComponent();
	virtual ~sgfEntityComponent();
	virtual void onAdd(){};
	virtual void onRemove(){};
	virtual const char* getName() const=0;
protected:
	sgfEntity* entity;
};

#endif