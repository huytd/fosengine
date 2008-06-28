#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SGF.h>
#include "JointAnimator.h"
#include "StandOnTerrainAnimator.h"
#include "ThirdPersonCamera.h"
#include "HUDControler.h"
#include "Icon.h"
#include "Map.h"
#include <irrlicht.h>

struct SAttack
{
	sgfEntity* attacker;
	int damage;
};

class Character: public sgfEntity, public LevelEntity<Character>//this entity is loadable from level
{
public:

	Character(irr::scene::ISceneNode* node)
	{
		speed=10.2f;
		goalReached=false;
		mouseDelegate.addRef();
		mouseDelegate.bind(this,&Character::onMouse);
		startPos=node->getAbsolutePosition();
		node->remove();
		attackDelegate=new sgfMethodDelegate<Character,SAttack>(this,&Character::onAttack);
		attackEvent.addDelegate(attackDelegate);
	}

	const char* getClassName() const
	{
		return "Character";
	}

	void receiveAttack(SAttack& atk)
	{
		attackEvent(atk);
	}

protected:
	void onAttack(SAttack& atk)
	{
		printf("Attacked by %s , damage %d \n",atk.attacker->getClassName(),atk.damage);
	}
	void onLevelStart()
	{
		terrain=manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();//get the terrain from a global variable
	}
	void onAdd()
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		
		node=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/thaycung/thaycung.b3d"));
		const irr::core::aabbox3df& box=node->getBoundingBox();
		irr::core::vector3df size=box.MaxEdge-box.getCenter();
		irr::core::matrix4 m;
		m.setTranslation(irr::core::vector3df(0,-size.Y,0));
		sgfPhysicWorld* world=manager->getCore()->getPhysicWorld();
		sgfPtr<sgfPhysicShape> shape=world->createSphere(size.X,size.Y,size.Z);
		body=manager->getCore()->getPhysicWorld()->createBody(shape);
		body->setMass(50.0f,shape);		
		body->setNode(node);
		body->setPosition(startPos);
		body->setOffset(m);
		new NewtonUpVector(world,body,irr::core::vector3df(0,-1,0));
		//sgfPhysicDebugger::add(smgr,body);
		
		irr::scene::ISceneNodeAnimator* anim1=new irr::scene::JointAnimator;
		node->setTransitionTime(0.2f);
		node->addAnimator(anim1);
		anim1->drop();

		node->setPosition(startPos);
		node->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		manager->getCore()->globalVars["characterNode"] = node;
		idle();
		//collision
		//const irr::core::aabbox3df& box = node->getBoundingBox();
		irr::core::vector3df radius = box.MaxEdge - box.getCenter();
		
		irr::scene::ISceneNodeAnimator* anim=new irr::scene::StandOnTerrainAnimator(manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::ITriangleSelector*>(),
			manager->getCore()->getGraphicDevice()->getSceneManager()->getSceneCollisionManager(),
			irr::core::vector3df(0,-1.0f,0)
			);
		//node->addAnimator(anim);
		anim->drop();
		//setup mouse
		manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(&mouseDelegate);
		//add camera
		ThirdPersonCamera* cam = new ThirdPersonCamera(node);
		manager->addEntity(cam);

		HUDControler* controler = new HUDControler();
        manager->addEntity(controler);

		//node->setVisible(false);
		//manager->getCore()->getGraphicDevice()->getSceneManager()->addCameraSceneNodeFPS()->setPosition(startPos);
	}
	void idle()
	{
		if(currentAction != "Idle")
		{
			node->setFrameLoop(1,40);
			currentAction = "Idle";
		}
	}
	void walk()
	{
		if(currentAction != "Walking")
			{
				node->setFrameLoop(71,100);
				currentAction = "Walking";       
			}
	}
	void onMouse(SMouseEvent& args)
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		if(args.mouseEvent==irr::EMIE_LMOUSE_PRESSED_DOWN)
		{
			irr::core::line3df ray(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(manager->getCore()->getGraphicDevice()->getCursorControl()->getPosition()));
			irr::core::vector3df collisionPoint;
			irr::core::triangle3df collisionTriangle;
			if(smgr->getSceneCollisionManager()->getCollisionPoint(ray, terrain->getTriangleSelector(), collisionPoint, collisionTriangle))
			{
				walk();
				targetPos = collisionPoint;
				goalReached=false;
				manager->setActive(this,true);//make update called every frame.
			}
		}
	}
	void update(float deltaTime)
	{
		//printf_s("%f\n",deltaTime);
		//deltaTime=15;
		irr::core::vector3df diffVect=node->getPosition()+body->getVelocity()-targetPos;
		diffVect.Y=0.0f;
		float distance=diffVect.getLength();
		if(distance<=(speed*deltaTime))//reached target
		{
			goalReached=true;
			manager->setActive(this,false);//stop updating
			idle();
		}
		else
		{
			body->unfreeze();
			body->setRotation(faceTarget(targetPos,node->getPosition()));
			body->addLocalForce(irr::core::vector3df(0,0,500));
		}
	}

	void onRemove()
	{
		manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(&mouseDelegate);
		node->remove();
		manager->getCore()->getPhysicWorld()->destroyBody(body);
	}

	irr::core::vector3df faceTarget(irr::core::vector3df targetpos, irr::core::vector3df nodepos)
	{
		irr::core::vector3df posDiff = targetpos - nodepos;
		posDiff.Y=0;//dont look up/down
		posDiff.normalize();
		return posDiff.getHorizontalAngle();
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
	sgfMethodDelegate<Character,SMouseEvent> mouseDelegate;
	irr::core::vector3df startPos;
	irr::core::vector3df targetPos;
	irr::scene::IAnimatedMeshSceneNode* node;
	irr::scene::ITerrainSceneNode* terrain;
	sgfPhysicBody* body;
	sgfEvent<SAttack> attackEvent;
	sgfPtr<sgfDelegate<SAttack>> attackDelegate;
};

#endif