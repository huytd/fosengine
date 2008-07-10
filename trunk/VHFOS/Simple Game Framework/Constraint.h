#ifndef _SGF_PHYSIC_CONSTRAINT_H_
#define _SGF_PHYSIC_CONSTRAINT_H_

#include "Newton.h"
#include "Physic.h"

class sgfPhysicConstraint
{
public:
	virtual void destroy();
protected:
	sgfPhysicConstraint(sgfPhysicWorld* world);
	virtual ~sgfPhysicConstraint();
	void initJoint();
	static void Destructor(const NewtonJoint* joint);

	sgfPhysicWorld* world;
	NewtonJoint* ptr;
};

class NewtonUpVector: public sgfPhysicConstraint
{
public:
	NewtonUpVector(sgfPhysicWorld* world,sgfPhysicBody* body,irr::core::vector3df& pin);
	void setPin(irr::core::vector3df& pin);
	irr::core::vector3df getPin() const;
};

#endif