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
#define FOV_VALUE 150

core::vector3df faceTargets(irr::core::vector3df targetpos, irr::core::vector3df nodepos) {

  core::vector3df posDiff = targetpos - nodepos;
  f32 degree = nodepos.Y; //keep current rotation if nothing to do
  posDiff.normalize();

  if (posDiff.X != 0.0f || posDiff.Z != 0.0f)
    degree = atan2(posDiff.X,posDiff.Z) * core::RADTODEG;

  return core::vector3df(0,degree,0);
} 

bool isFacingNode(irr::scene::ISceneNode* self, irr::scene::ISceneNode* other)
   {
      // get the point or points that represents the node 'other'
      
      // these coordinates need to be the world coordinate system,
      // so you may need to use a transformation.
      // get local coordinate position by  getPostion
      // tranform to world coordinate
      irr::core::vector3df otherPosition = other->getPosition();
      other->getAbsoluteTransformation().transformVect(otherPosition);

      //or simpler??
      //otherPosition = other->getAbsolutePosition();

      // get a transformation matrix to transform from world coordinates
      // to 'self' coordinate system. Remember that the result of
      // self->getAbsoluteTransformation() will transform from 'self'
      // coordinate system to the world coordinate system. you want
      // the inverse of that transformation.
      irr::core::matrix4 selfTransformation = self->getAbsoluteTransformation();
      irr::core::matrix4 invertTransformation;
      selfTransformation.getInverse(invertTransformation);
      
      // transform the points that represent the node 'other' into
      // 'self' coordinates using the matrix above
      invertTransformation.transformVect(otherPosition);

      // if the distance on the X axis is less than 0, then the given
      // point is behind 'self'.

      if(otherPosition.Z> 0) // if not behind
      {         
         float zXy = sqrtf(otherPosition.Y*otherPosition.Y + otherPosition.X*otherPosition.X);

         float tanAnpha = zXy/otherPosition.Z;

         // in the range [-pi, +pi] radians.
         float anpha = abs(atan(tanAnpha));

         // Hard code irr::core::PI/6 for testing purpose
         if(anpha < irr::core::PI/6) return true;
      }

      return false;
            
   }

void movetos(irr::scene::ISceneNode *node, //node to move
            irr::core::vector3df vel) //velocity vector
{
    irr::core::matrix4 m;
    m.setRotationDegrees(node->getRotation());
    m.transformVect(vel);
    node->setPosition(node->getPosition() + vel);
    node->updateAbsolutePosition();
} 

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

bool CCharacter::seePlayer(scene::ISceneNode* target)
{
	bool result = false;

	if ((node->getPosition().getDistanceFrom(target->getPosition()) <= FOV_VALUE)&&(isFacingNode(node, target)))
	{
		result = true;
	}
	return result;
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
		node->setRotation(	faceTargets(pos, node->getPosition()) );
		movetos(node, core::vector3df(0,0,fSpeed));
	}

	this->node->animateJoints() ;
}

void CCharacter::rotateTo(core::vector3df tpos)
{
	node->setRotation(faceTargets(node->getPosition(),tpos));
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


