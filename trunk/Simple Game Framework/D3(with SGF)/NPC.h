#ifndef _NPC_H_
#define _NPC_H_

#include <SGF.h>
#include "JointAnimator.h"
#include "StandOnTerrainAnimator.h"
#include "ThirdPersonCamera.h"
#include "HUDControler.h"
#include "Icon.h"
#include "Map.h"
#include <irrlicht.h>

class NPC: public sgfEntity, public LevelEntity<NPC>//this entity is loadable from level
{
public:

	NPC(irr::scene::ISceneNode* node)
	{
		speed=22.2f;
		goalReached=false;
		startPos=node->getAbsolutePosition();
		node->remove();
	}

	const char* getClassName() const
	{
		return "NPC";
	}
protected:
	void onLevelStart()
	{
		terrain=manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();//get the terrain from a global variable
	}
	void onAdd()
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();

		node=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/macay/macay.b3d"));

		irr::scene::ISceneNodeAnimator* anim1=new irr::scene::JointAnimator;
		node->setTransitionTime(0.2f);
		node->addAnimator(anim1);
		anim1->drop();

		node->setPosition(startPos);
		node->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		manager->getCore()->globalVars["NPCNode"] = node;
		idle();

		//! collision
		const irr::core::aabbox3df& box = node->getBoundingBox();
		irr::core::vector3df radius = box.MaxEdge - box.getCenter();

		irr::scene::ISceneNodeAnimator* anim=new irr::scene::StandOnTerrainAnimator(manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::ITriangleSelector*>(),
			manager->getCore()->getGraphicDevice()->getSceneManager()->getSceneCollisionManager(),
			irr::core::vector3df(0,-1.0f,0)
			);
		node->addAnimator(anim);
		anim->drop();

		//HUDControler* controler = new HUDControler();
		//manager->addEntity(controler);

		//! make update called every frame.
		manager->setActive(this,true);

	}

	void idle()
	{
		if(currentAction != "Idle")
		{
			node->setFrameLoop(0,40);
			currentAction = "Idle";
		}
	}
	void walk()
	{
			if(currentAction != "Walking")
			{
				node->setFrameLoop(41,80);
				currentAction = "Walking";       
			}
	}


	void onMouse(SMouseEvent& args)
	{
	}

	void update(float deltaTime)
	{
		//! Very simple AI
		irr::scene::IAnimatedMeshSceneNode* NPCnode = manager->getCore()->globalVars["characterNode"].getAs<irr::scene::IAnimatedMeshSceneNode*>();

		//! Get positon near target node, -5 mean behind 5 units
		targetPos = getNearPosition(NPCnode, irr::core::vector3df(0,0,-5));

		irr::core::vector3df diffVect = node->getPosition() - targetPos;
		diffVect.Y = 0.0f;
		float distance=diffVect.getLength();

		if(distance <= (speed*deltaTime))//reached target
		{
			idle();
			goalReached = true;
		}
		else
		{
			walk();				
			goalReached=false;
			node->setRotation(faceTarget(targetPos,node->getPosition()));				
			moveto(irr::core::vector3df(0,0,speed*deltaTime));
		}
	}

	void onRemove()
	{
		manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(&mouseDelegate);
		node->remove();
	}

	irr::core::vector3df faceTarget(irr::core::vector3df targetpos, irr::core::vector3df nodepos)
	{
		irr::core::vector3df posDiff = targetpos - nodepos;
		posDiff.Y=0;//dont look up/down
		posDiff.normalize();
		return posDiff.getHorizontalAngle();
	} 

	irr::core::vector3df getNearPosition(irr::scene::IAnimatedMeshSceneNode* targetNode, irr::core::vector3df vel) //velocity vector
	{
		irr::core::matrix4 m;
		m.setRotationDegrees(targetNode->getRotation());
		m.transformVect(vel);
		return (targetNode->getPosition() + vel);
	}

	void moveto(irr::core::vector3df vel) //velocity vector
	{
		irr::core::matrix4 m;
		m.setRotationDegrees(node->getRotation());
		m.transformVect(vel);
		node->setPosition(node->getPosition() + vel);
		node->updateAbsolutePosition();
	}

protected:
	float speed;
	bool goalReached;
	char* currentAction;
	sgfMethodDelegate<NPC,SMouseEvent> mouseDelegate;
	irr::core::vector3df startPos;
	irr::core::vector3df targetPos;
	irr::scene::IAnimatedMeshSceneNode* node;
	irr::scene::ITerrainSceneNode* terrain;
};

#endif