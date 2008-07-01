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
#include "Enemy.h"
#include "Magic.h"

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
		startPos=node->getAbsolutePosition();
		node->remove();

		mouseDelegate.addRef();
		mouseDelegate.bind(this,&Character::onMouse);
		
		collisionDelegate.addRef();
		collisionDelegate.bind(this,&Character::onCollision);

		//attackDelegate=new sgfMethodDelegate<Character,SAttack>(this,&Character::onAttack);
		//attackEvent.addDelegate(attackDelegate);
	}

	const char* getClassName() const
	{
		return "Character";
	}
	
	static int colID;
protected:
	void onCollision(SCollisionEvent& arg)
	{
		//printf("Touch enemy");
		if(magic->isEnd())
		{
			magic->attack(0,node->getPosition(), mouse->getPosition(), 200.0f, 200.0f, 30000.0f, 200.0f);
			//mouse->setPosition(core::vector3df(0.0f,10.0f,0.0f) + mouse->getPosition());
		}

	}
	void onLevelStart()
	{
		terrain=manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();//get the terrain from a global variable
	}
	void onAdd()
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		
		node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/thaycung/thaycung.b3d"));
		
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
		//world->getPairCollisionEvent(colID,Enemy::colID,ERT_SIMPLE_RESPONSE)->addDelegate(&collisionDelegate);
		//sgfPhysicDebugger::add(smgr,body);
		
		mouse = new sgfPhysicBody(shape);
		mouse->setOffset(m);
		mouse->userData=this;
		
		//world->attachNode(mouse,node);
		world->addBody(mouse);
		world->setBodyCollisionClass(mouse,colID);
		world->getPairCollisionEvent(colID,Enemy::colID,ERT_SIMPLE_RESPONSE)->addDelegate(&collisionDelegate);
		
		magic = new Magic();	
		manager->addEntity(magic);

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
		node->addAnimator(anim);
		anim->drop();

		//setup mouse
		manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(&mouseDelegate);
		
		//add camera
		cam = new ThirdPersonCamera(node);
		manager->addEntity(cam);

		controler = new HUDControler();
        manager->addEntity(controler);
		

	}

	void idle()
	{
		if(currentAction != "Idle")
		{
			node->setFrameLoop(10,40);
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
				mouse->setPosition(collisionPoint);
				goalReached=false;
				node->setRotation(faceTarget(targetPos,node->getPosition()));
				manager->setActive(this,true);//make update called every frame.

			}
		}
		else if(args.mouseEvent==irr::EMIE_RMOUSE_PRESSED_DOWN)
		{
			irr::core::line3df ray(smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(
				manager->getCore()->getGraphicDevice()->getCursorControl()->getPosition()));
			irr::scene::ISceneNode* enemynode = smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);
			
			if(enemynode)
			{
				if(enemynode->getType() == ESCENE_NODE_TYPE::ESNT_ANIMATED_MESH)
				{
					
					idle();
					mouse->setPosition(enemynode->getAbsolutePosition());
					targetPos = node->getPosition();
					node->setRotation(faceTarget(enemynode->getAbsolutePosition(),node->getPosition()));
					manager->setActive(this,true);//make update called every frame.
				}
			}
		}
	}

	void update(float deltaTime)
	{
		irr::core::vector3df diffVect = targetPos - node->getPosition();
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
			diffVect.setLength(speed*deltaTime);
			node->setPosition(node->getPosition()+diffVect);
			node->updateAbsolutePosition();
		}
	}

	void onRemove()
	{
		manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(&mouseDelegate);
		manager->getCore()->getPhysicWorld()->getPairCollisionEvent(colID,Enemy::colID,ERT_SIMPLE_RESPONSE)->removeDelegate(&collisionDelegate);
		node->remove();
		manager->getCore()->getPhysicWorld()->removeBody(body);
		manager->getCore()->getPhysicWorld()->removeBody(mouse);		
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
	Magic* magic;
	sgfPhysicBody* body;
	sgfPhysicBody* mouse;
	sgfMethodDelegate<Character,SCollisionEvent> collisionDelegate;

	ThirdPersonCamera* cam;
	HUDControler* controler;
};

int Character::colID;

#endif