#include "Constraint.h"

sgfPhysicConstraint::sgfPhysicConstraint(sgfPhysicWorld* world)
:world(world)
{
}
sgfPhysicConstraint::~sgfPhysicConstraint()
{
}

void sgfPhysicConstraint::Destructor(const NewtonJoint* joint)
{
	delete (sgfPhysicConstraint*)NewtonJointGetUserData(joint);
}

void sgfPhysicConstraint::initJoint()
{
	NewtonJointSetUserData(ptr,this);
	NewtonJointSetDestructor(ptr,sgfPhysicConstraint::Destructor);
}

void sgfPhysicConstraint::destroy()
{
	NewtonDestroyJoint(world->getPtr(),ptr);
}

NewtonUpVector::NewtonUpVector(sgfPhysicWorld* world,sgfPhysicBody* body,irr::core::vector3df& pin)
:sgfPhysicConstraint(world)
{
	ptr=NewtonConstraintCreateUpVector(world->getPtr(),reinterpret_cast<float*>(&pin),body->getPtr());
	initJoint();
}

void NewtonUpVector::setPin(irr::core::vector3df& pin)
{
	NewtonUpVectorSetPin(ptr,reinterpret_cast<float*>(&pin));
}

irr::core::vector3df NewtonUpVector::getPin() const
{
	float vec[3];
	NewtonUpVectorGetPin(ptr,vec);
	return irr::core::vector3df(vec[0],vec[1],vec[2]);
}