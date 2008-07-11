#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SGF.h>
#include "JointAnimator.h"
#include "StandOnTerrainAnimator.h"
#include "ThirdPersonCamera.h"
#include "HUDControler.h"
#include "Icon.h"
#include "Map.h"
#include <irrlicht.h>
#include "Magic.h"

class Enemy: public sgfEntity, public LevelEntity<Enemy>, irr::scene::IAnimationEndCallBack //this entity is loadable from 
{
public:

	Enemy(irr::scene::ISceneNode* node)
	{
		speed=5.0f;
		goalReached=false;
		startPos=node->getAbsolutePosition();
		node->remove();
	}

	const char* getClassName() const
	{
		return "Enemy";
	}
	static int colID;
protected:
	void onLevelStart()
	{
		terrain=manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();//get the terrain from a global variable
	}
	void onAdd()
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();

		node=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/macay/macay.b3d"));

		const irr::core::aabbox3df& box=node->getBoundingBox();
		irr::core::vector3df size=box.MaxEdge-box.MinEdge;
		irr::core::matrix4 m;
		m.setTranslation(irr::core::vector3df(0,size.Y/2,0));
		sgfPhysicWorld* world=manager->getCore()->getPhysicWorld();
		sgfPtr<sgfPhysicShape> shape=world->createBox(size.X,size.Y,size.Z);
		body=new sgfPhysicBody(shape);
		body->setOffset(m);
		body->userData=this;
		world->attachNode(body,node);
		world->addBody(body);
		world->setBodyCollisionClass(body,colID);

		irr::scene::ISceneNodeAnimator* anim1=new irr::scene::JointAnimator;
		node->setTransitionTime(0.2f);
		node->addAnimator(anim1);
		anim1->drop();

		node->setPosition(startPos);
		node->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		manager->getCore()->globalVars["EnemyNode"] = node;
		idle();

		//! collision
//		const irr::core::aabbox3df& box = node->getBoundingBox();
		irr::core::vector3df radius = box.MaxEdge - box.getCenter();

		irr::scene::ISceneNodeAnimator* anim=new irr::scene::StandOnTerrainAnimator(manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::ITriangleSelector*>(),
			manager->getCore()->getGraphicDevice()->getSceneManager()->getSceneCollisionManager(),
			irr::core::vector3df(0,-1.0f,0)
			);
		node->addAnimator(anim);
		anim->drop();

		magic = new Magic();	
		manager->addEntity(magic);

		//! make update called every frame.
		manager->setActive(this,true);

		//
		core::stringw strEnemyName = L"Ma cây";
		charName = manager->getCore()->getGraphicDevice()->getSceneManager()->addTextSceneNode(
			manager->getCore()->getGraphicDevice()->getSceneManager()->getGUIEnvironment()->getFont("font/myfont.xml"),
            strEnemyName.c_str(),
            video::SColor(100, 255, 255, 255),
            node,
			node->getPosition(),
            -1); 

	}

	void idle()
	{
		if(currentAction != "Idle")
		{
			node->setFrameLoop(0,40);
			node->setLoopMode(true);
			currentAction = "Idle";
		}
	}
	void walk()
	{
		if(currentAction != "Walking")
		{
			node->setFrameLoop(41,80);
			node->setLoopMode(true);
			currentAction = "Walking";       
		}
	}

	void attack()
	{
		if(currentAction != "Attacking")
		{
			node->setFrameLoop(106,125);
			node->setLoopMode(false);
			currentAction = "Attacking";       
		}
		node->setAnimationEndCallback(this);
	}

	void onMouse(SMouseEvent& args)
	{
	}

	void OnAnimationEnd(IAnimatedMeshSceneNode* node)
	{
		idle();
	} 

	void update(float deltaTime)
	{
		//! Very simple AI
		irr::scene::IAnimatedMeshSceneNode* Enemynode = manager->getCore()->globalVars["characterNode"].getAs<irr::scene::IAnimatedMeshSceneNode*>();

		//! Get positon near target node, -5 mean behind 5 units
		targetPos = Enemynode->getPosition();//getNearPosition(Enemynode, irr::core::vector3df(0,0,25));

		irr::core::vector3df diffVect = node->getPosition() - targetPos;
		
		float distance = diffVect.getLength();

		if(distance <= 55 )//reached target
		{			
			if(magic->isEnd())
			{
				node->setRotation(faceTarget(targetPos,node->getPosition()));	
				attack();
				if(currentAction == "Attacking")
					magic->attack(0, node->getPosition(), Enemynode->getPosition(), 200.0f, 200.0f, 30000.0f, 200.0f);
				
			}

			goalReached = true;
		}
		else
		{			
			walk();				
			goalReached=false;
			node->setRotation(faceTarget(targetPos,node->getPosition()));				
			moveto(irr::core::vector3df(0,0,speed*deltaTime));			

			charName->setPosition( core::vector3df(0.0f, 12.3f, 0.0f));
		}

		
		
	}


	void onRemove()
	{
		manager->getCore()->getPhysicWorld()->removeBody(body);
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
	Magic* magic;
	irr::core::vector3df startPos;
	irr::core::vector3df targetPos;
	irr::scene::IAnimatedMeshSceneNode* node;
	irr::scene::ITerrainSceneNode* terrain;
	sgfPhysicBody* body;

	irr::scene::ITextSceneNode* charName;
};
int Enemy::colID;
#endif