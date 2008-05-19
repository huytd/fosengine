/*
 ***********************************************
 * Character control
 * *********************************************
 * file name: CCharacter.h
 * encoding: UTF-8
 * tab size: 8
 * indentation: 4
 * created on: 9:03 PM 3/17/2008
 * created by: FOSP Team
 * copyright: FOS Project
 */
#include "CCharacter.h"

#include <irrlicht.h>

irr::scene::IAnimatedMeshSceneNode* CCharacter::getMesh()
{
	return node;
}

CCharacter::CCharacter(IrrlichtDevice* device, irr::scene::ISceneManager *smgr,irr::video::IVideoDriver *videodriver, char* meshPath)
{
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(meshPath));
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	fSpeed = 0.4f;

	curState = CCharacter::eState::Idle;
	oldState = CCharacter::eState::Idle;

	curAttackType = CCharacter::eAttacktype::MagicAttack;
	
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
	setState(CCharacter::eState::Run);
	
	node->setAnimationSpeed(30);
	node->setLoopMode(true);
	node->setFrameLoop(1,14);
}

void CCharacter::walk()
{
	setState(CCharacter::eState::Walk);
	
	node->setAnimationSpeed(40);
	node->setLoopMode(true);
	node->setFrameLoop(1,14);
}

void CCharacter::idle()
{
	setState(CCharacter::eState::Idle);
	
	node->setAnimationSpeed(25);
	node->setLoopMode(true);
	node->setFrameLoop(206,300);
}


void CCharacter::attack(eAttacktype attackType)
{
	setState(CCharacter::eState::Attack);
	switch (attackType)
	{
		case eAttacktype::PowerAttack :
		{	
			node->setAnimationSpeed(20);
			node->setLoopMode(false);
			node->setFrameLoop(45,59);
			node->setAnimationEndCallback(this);
			return;
		} 	

		case eAttacktype::DownswipeAttack:
		{	
			node->setAnimationSpeed(15);
			node->setLoopMode(false);
			node->setFrameLoop(60,68);
			node->setAnimationEndCallback(this);
			return;
		} 

		case eAttacktype::MagicAttack:
		{	
			node->setAnimationSpeed(15);
			node->setLoopMode(false);
			node->setFrameLoop(69,72);
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

void CCharacter::move(core::vector3df pos)
{
	// Avoid complex operation in next step
	if(getState() != CCharacter::eState::Walk && getState() != CCharacter::eState::Run)
		return;

	if (node->getPosition().getDistanceFrom(pos) < 4)
	{
		idle();
	}
	else
	{
		node->setRotation(	faceTarget(pos, node->getPosition()) );
		moveto(node, core::vector3df(0,0,fSpeed));
	}
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
