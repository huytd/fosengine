#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "SGF.h"
#include "JointAnimator.h"
#include "StandOnTerrainAnimator.h"
#include <irrlicht.h>

class ThirdPersonCamera: public sgfEntity
{
public:
	ThirdPersonCamera(irr::scene::ISceneNode* targetNode,irr::core::vector3df posOffset,irr::core::vector3df targetOffset)
		:targetNode(targetNode),posOffset(posOffset),targetOffset(targetOffset),rotating(0)
	{
		mouseDelegate.addRef();
		mouseDelegate.bind(this,&ThirdPersonCamera::onMouse);
	}
	float acceleration;
private:
	bool rotating;
	int oldX;
	int nxY;
	irr::gui::ICursorControl* cursor;
	irr::core::vector3df posOffset;
	irr::core::vector3df targetOffset;
	irr::scene::ISceneNode* targetNode;
	irr::scene::ICameraSceneNode* camera;
	irr::scene::ISceneNode* root;
	sgfMethodDelegate<ThirdPersonCamera,SMouseEvent> mouseDelegate;
	void onAdd()
	{
		cursor=manager->getCore()->getGraphicDevice()->getCursorControl();
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		root=smgr->addEmptySceneNode();
		camera=smgr->addCameraSceneNode(root,posOffset);
		manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(&mouseDelegate);
		manager->setActive(this,true);
	}
	void update(float deltaTime)
	{
		if(rotating)
		{
			float deltaY=cursor->getPosition().X-oldX;
			//printf("%f %d\n",cursor->getRelativePosition().Y,oldY);
			root->setRotation(root->getRotation()+irr::core::vector3df(0.0f,deltaTime*(float)deltaY/50.0f,0.0f));
		}
		irr::core::vector3df target=targetNode->getPosition();
		//if((target-camera->getAbsolutePosition()).getLength()>5)
		root->setPosition(target);
		camera->setTarget(targetNode->getPosition()+targetOffset);

		irr::scene::ITerrainSceneNode* terr = manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();
		if (camera->getPosition().Y < terr->getHeight(camera->getPosition().X, camera->getPosition().Z))
		{
			camera->setPosition(irr::core::vector3df(camera->getPosition().X, terr->getHeight(camera->getPosition().X, camera->getPosition().Z), camera->getPosition().Z));
		}
		//Change camera position by mouse wheel (smooth)
		irr::core::vector3df camPos = camera->getPosition();
		if (camPos.Y < nxY) camPos.Y += 0.25;
		if (camPos.Y > nxY) camPos.Y -= 0.25;
		camera->setPosition(camPos);
		
	}
	void onRemove()
	{
		manager->getCore()->getInputManager()->getMouseEvent()->removeDelegate(&mouseDelegate);
		camera->remove();
		root->remove();
	}
	void onMouse(SMouseEvent& args)
	{
		if(args.mouseEvent==irr::EMIE_MOUSE_WHEEL)
		{
			nxY = camera->getPosition().Y + args.wheel*2;
			/*if (curCY < nxY)
			{
				curCY += 1;
				//camera->setPosition(camera->getPosition()+irr::core::vector3df(0,args.wheel,0));
				camera->setPosition(camera->getPosition()+irr::core::vector3df(0,1,0));
			}
			if (curCY > nxY)
			{
				curCY -= 1;
				//camera->setPosition(camera->getPosition()+irr::core::vector3df(0,args.wheel,0));
				camera->setPosition(camera->getPosition()+irr::core::vector3df(0,-1,0));
			}*/
		}
		else if(args.mouseEvent==irr::EMIE_MMOUSE_PRESSED_DOWN)
		{
			rotating=true;
			oldX=args.x;
		}
		else if(args.mouseEvent==irr::EMIE_MMOUSE_LEFT_UP)
		{
			rotating=false;
		}
	}
};

class Character: public sgfEntity
{
public:
	static sgfEntity* createFromNode(irr::scene::ISceneNode* node)
	{
		return new Character(node);
	}

	Character(irr::scene::ISceneNode* node)
	{
		speed=22.2f;
		goalReached=false;
		mouseDelegate.addRef();
		mouseDelegate.bind(this,&Character::onMouse);
		startPos=node->getAbsolutePosition();
		node->remove();
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
		ThirdPersonCamera* cam=new ThirdPersonCamera(node,irr::core::vector3df(0,10,-30),irr::core::vector3df(0,5,0));
		manager->addEntity(cam);
		//node->setVisible(false);
		//manager->getCore()->getGraphicDevice()->getSceneManager()->addCameraSceneNodeFPS()->setPosition(startPos);
	}
	void idle()
	{//will be rewritten
		node->setFrameLoop(205,249);
	}
	void walk()
	{
		node->setFrameLoop(0,14);
	}
	void onMouse(SMouseEvent& args)
	{
		irr::scene::ISceneManager* smgr=manager->getCore()->getGraphicDevice()->getSceneManager();
		if(args.mouseEvent==irr::EMIE_RMOUSE_PRESSED_DOWN)
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

				manager->setActive(this,true);//make update called every frame
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