/*
 ***********************************************
 * Character control
 * *********************************************
 * file name: ThayCung.h
 * encoding: UTF-8
 * tab size: 8
 * indentation: 4
 * created on: 9:03 PM 3/17/2008
 * init by: Do Quoc Khanh - doqkhanh
 * created by: FOSP Team
 * copyright: FOS Project
 */
#include "ThayCung.h"

#include <irrlicht.h>
#define ANIMATION_SPEED 30
#define MOVING_SPEED 0.6f

ThayCung::ThayCung(IrrlichtDevice* device, irr::scene::ISceneManager *smgr,irr::video::IVideoDriver *videodriver, char* meshPath)
{
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(meshPath));
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	fSpeed = MOVING_SPEED;

	curState =  Idle;
	oldState = Idle;

	curAttackType = MagicAttack;

	node->setJointMode(irr::scene::EJUOR_CONTROL);
	node->setTransitionTime((float)0.25);
	
	idle();
}

ThayCung::~ThayCung(void)
{
	node->remove();
}

void ThayCung::setState(eState newState)
{
	oldState = curState;
	curState = newState;
}

void ThayCung::setPosition(core::vector3df pos)
{
	node->setPosition(pos);
}

void ThayCung::setRotate(core::vector3df rot)
{
	node->setRotation(rot);
}

	
core::vector3df ThayCung::getPosition()
{
	return node->getPosition();
}

core::vector3df ThayCung::getRotate()
{
	return node->getRotation();
}

ThayCung::eState ThayCung::getState()
{
	return curState;
}


void ThayCung::run()
{
	if(this->getState() == Run) return;

	setState(ThayCung::Run);
	
	node->setAnimationSpeed(ANIMATION_SPEED);
	node->setLoopMode(true);
	node->setFrameLoop(71,100);
}

void ThayCung::walk()
{
	if(this->getState() == Walk) return;

	setState(ThayCung::Walk);

	node->setAnimationSpeed(ANIMATION_SPEED);
	node->setLoopMode(true);
	node->setFrameLoop(41,70);
}

void ThayCung::idle()
{
	setState(ThayCung::Idle);
	
	node->setAnimationSpeed(ANIMATION_SPEED);
	node->setLoopMode(true);
	node->setFrameLoop(1,40);
}


void ThayCung::attack(eAttacktype attackType)
{
	setState(ThayCung::Attack);
	switch (attackType)
	{
		case PowerAttack :
		{	
			node->setAnimationSpeed(ANIMATION_SPEED*2);
			node->setLoopMode(false);
			node->setFrameLoop(116,130);
			node->setAnimationEndCallback(this);
			return;
		} 	

		case DownswipeAttack:
		{	
			node->setAnimationSpeed(ANIMATION_SPEED*2);
			node->setLoopMode(false);
			node->setFrameLoop(131,145);
			node->setAnimationEndCallback(this);
			return;
		} 

		case MagicAttack:
		{	
			node->setAnimationSpeed(ANIMATION_SPEED*2);
			node->setLoopMode(false);
			node->setFrameLoop(161,180);
			node->setAnimationEndCallback(this);
			return;
		} 

	}

}


core::vector3df faceTarget(irr::core::vector3df targetpos, irr::core::vector3df nodepos) {

  core::vector3df posDiff = targetpos - nodepos;
  f32 degree = nodepos.Y; //keep current rotation if nothing to do
  posDiff.normalize();

  if (posDiff.X != 0.0f || posDiff.Z != 0.0f)
    degree = atan2(posDiff.X,posDiff.Z) * core::RADTODEG;

  return core::vector3df(0,degree,0);
} 


void moveto(irr::scene::ISceneNode *node, //node to move
            irr::core::vector3df vel) //velocity vector
{
    irr::core::matrix4 m;
    m.setRotationDegrees(node->getRotation());
    m.transformVect(vel);
    node->setPosition(node->getPosition() + vel);
    node->updateAbsolutePosition();
} 

void ThayCung::move(core::vector3df pos)
{
	// Avoid complex operation in next step
	if(getState() != ThayCung::Walk && getState() != ThayCung::Run)
	{
		this->node->animateJoints() ;
		return;
	}

	if (node->getPosition().getDistanceFrom(pos) < 4)
	{
		idle();
	}
	else
	{
		node->setRotation(	faceTarget(pos, node->getPosition()) );
		moveto(node, core::vector3df(0,0,fSpeed));
	}

	this->node->animateJoints() ;
}

void ThayCung::update()
{
	//Do nothing
}

void ThayCung::OnAnimationEnd(IAnimatedMeshSceneNode* node)
{
    idle();
} 

void ThayCung::remove()
{
	node->remove();
}

irr::scene::IAnimatedMeshSceneNode* ThayCung::getNode()
{
	return node;	
}