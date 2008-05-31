/*
 ***********************************************
 * Character control
 * *********************************************
 * file name: CCharacter.h
 * encoding: UTF-8
 * tab size: 8
 * indentation: 4
 * created on: 9:03 PM 3/17/2008
 * init by: Do Quoc Khanh - doqkhanh
 * created by: FOSP Team
 * copyright: FOS Project
 */
#include "CCharacter.h"

#include <irrlicht.h>
#define ANIMATION_SPEED 10
irr::scene::IAnimatedMeshSceneNode* CCharacter::getMesh()
{
	return node;
}

CCharacter::CCharacter(IrrlichtDevice* device, irr::scene::ISceneManager *smgr,irr::video::IVideoDriver *videodriver, char* meshPath)
{
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(meshPath));
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	fSpeed = 0.4f;

	curState =  Idle;
	oldState = Idle;

	curAttackType = MagicAttack;

	node->setJointMode(irr::scene::EJUOR_CONTROL);
	node->setTransitionTime((float)0.2);
	
	idle();
}

CCharacter::~CCharacter(void)
{
	node->remove();
}

void CCharacter::setState(eState newState)
{
	oldState = curState;
	curState = newState;
}

void CCharacter::setPosition(core::vector3df pos)
{
	node->setPosition(pos);
}

void CCharacter::setRotate(core::vector3df rot)
{
	node->setRotation(rot);
}

	
core::vector3df CCharacter::getPosition()
{
	return node->getPosition();
}

core::vector3df CCharacter::getRotate()
{
	return node->getRotation();
}

CCharacter::eState CCharacter::getState()
{
	return curState;
}


void CCharacter::run()
{
	if(this->getState() == Run) return;

	setState(CCharacter::Run);
	
	node->setAnimationSpeed(ANIMATION_SPEED);
	node->setLoopMode(true);
	node->setFrameLoop(1,14);
}

void CCharacter::walk()
{
	if(this->getState() == Walk) return;

	setState(CCharacter::Walk);

	node->setAnimationSpeed(ANIMATION_SPEED);
	node->setLoopMode(true);
	node->setFrameLoop(1,14);
}

void CCharacter::idle()
{
	setState(CCharacter::Idle);
	
	node->setAnimationSpeed(ANIMATION_SPEED);
	node->setLoopMode(true);
	node->setFrameLoop(206,300);
}


void CCharacter::attack(eAttacktype attackType)
{
	setState(CCharacter::Attack);
	switch (attackType)
	{
		case PowerAttack :
		{	
			node->setAnimationSpeed(ANIMATION_SPEED*2);
			node->setLoopMode(false);
			node->setFrameLoop(45,59);
			node->setAnimationEndCallback(this);
			return;
		} 	

		case DownswipeAttack:
		{	
			node->setAnimationSpeed(ANIMATION_SPEED*2);
			node->setLoopMode(false);
			node->setFrameLoop(60,68);
			node->setAnimationEndCallback(this);
			return;
		} 

		case MagicAttack:
		{	
			node->setAnimationSpeed(ANIMATION_SPEED*2);
			node->setLoopMode(false);
			node->setFrameLoop(69,72);
			node->setAnimationEndCallback(this);
			return;
		} 

	}

}




void CCharacter::move(core::vector3df pos)
{
	// Avoid complex operation in next step
	if(getState() != CCharacter::Walk && getState() != CCharacter::Run)
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

void CCharacter::update()
{
	//Do nothing
}

void CCharacter::OnAnimationEnd(IAnimatedMeshSceneNode* node)
{
    idle();
} 

void CCharacter::remove()
{
	node->remove();
}

irr::scene::IAnimatedMeshSceneNode* CCharacter::getNode()
{
	return node;	
}