#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SGF.h>
#include "JointAnimator.h"
#include "StandOnTerrainAnimator.h"
#include "ThirdPersonCamera.h"
#include "HUDControler.h"
#include <irrlicht.h>

class Character: public sgfEntity, public LevelEntity<Character>//this entity is loadable from level
{
public:

	Character(irr::scene::ISceneNode* node)
	{
		speed=22.2f;
		goalReached=false;
		mouseDelegate.addRef();
		mouseDelegate.bind(this,&Character::onMouse);
		startPos=node->getAbsolutePosition();
		node->remove();
	}

	const char* getClassName() const
	{
		return "Character";
	}
protected:
	void onLevelStart()
	{
		terrain=manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();//get the terrain from a global variable
	}
	void onAdd()
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		
		node=smgr->addAnimatedMeshSceneNode(smgr->getMesh("models/ninja/ninja.b3d"));
		//node->setDebugDataVisible(irr::scene::EDS_FULL);
		irr::scene::ISceneNodeAnimator* anim1=new irr::scene::JointAnimator;
		node->setTransitionTime(0.2f);
		node->addAnimator(anim1);
		anim1->drop();
		//temporary work around
		//node->setScale(irr::core::vector3df(0.01,0.01,-0.01));
		node->setPosition(startPos);
		node->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		manager->getCore()->globalVars["characterNode"] = node;
		idle();
		//collision
		const irr::core::aabbox3df& box = node->getBoundingBox();
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
		ThirdPersonCamera* cam = new ThirdPersonCamera(node);
		manager->addEntity(cam);

		HUDControler* controler = new HUDControler(node);
        manager->addEntity(controler);

		//node->setVisible(false);
		//manager->getCore()->getGraphicDevice()->getSceneManager()->addCameraSceneNodeFPS()->setPosition(startPos);
	}
	void idle()
	{//will be rewritten
		node->setFrameLoop(205,249);
	}
	void walk()
	{
	    if ((node->getStartFrame()!=0)&&(node->getEndFrame()!=14))
		{
		node->setFrameLoop(0,14);
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
				node->setRotation(faceTarget(targetPos,node->getPosition()));
				manager->setActive(this,true);//make update called every frame.
			}
		}
	}
	void update(float deltaTime)
	{
		//printf_s("%f\n",deltaTime);
		//deltaTime=15;
		irr::core::vector3df diffVect=node->getPosition()-targetPos;
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
	sgfMethodDelegate<Character,SMouseEvent> mouseDelegate;
	irr::core::vector3df startPos;
	irr::core::vector3df targetPos;
	irr::scene::IAnimatedMeshSceneNode* node;
	irr::scene::ITerrainSceneNode* terrain;
};

#endif