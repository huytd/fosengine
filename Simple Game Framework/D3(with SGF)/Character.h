#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "SGF.h"
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
	void update(int deltaTime)
	{
		if(rotating)
		{
			float deltaY=cursor->getPosition().X-oldX;
			//printf("%f %d\n",cursor->getRelativePosition().Y,oldY);
			root->setRotation(root->getRotation()+irr::core::vector3df(0,deltaY/4*deltaTime/1000.0f,0));
		}
		irr::core::vector3df target=targetNode->getAbsolutePosition();
		//if((target-camera->getAbsolutePosition()).getLength()>5)
			root->setPosition(target);
		camera->setTarget(targetNode->getAbsolutePosition()+targetOffset);
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
			camera->setPosition(camera->getPosition()+irr::core::vector3df(0,args.wheel,0));
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
		speed=50;
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
		node->setPosition(startPos);
		node->setMaterialFlag(irr::video::EMF_LIGHTING,false);
		idle();
		//collision
		const irr::core::aabbox3df& box = node->getBoundingBox();
		irr::core::vector3df radius = box.MaxEdge - box.getCenter();
		
		irr::scene::ISceneNodeAnimator* anim=smgr->createCollisionResponseAnimator(manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::ITriangleSelector*>(),
			node,
			radius,
			irr::core::vector3df(0,-0.1f,0),
			irr::core::vector3df(0,-(box.MaxEdge-box.getCenter()).Y,0)//,
			//0.0009f
			);
		node->addAnimator(anim);
		anim->drop();
		//setup mouse
		manager->getCore()->getInputManager()->getMouseEvent()->addDelegate(&mouseDelegate);
		//add camera
		ThirdPersonCamera* cam=new ThirdPersonCamera(node,irr::core::vector3df(0,10,-30),irr::core::vector3df(0,5,0));
		manager->addEntity(cam);
	}
	void idle()
	{//will be rewritten
		node->setFrameLoop(205,249);
	}
	void walk()
	{
		node->setFrameLoop(0,13);
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
				manager->setActive(this,true);//make update called every frame
			}
		}
	}
	void update(int deltaTime)
	{
		irr::core::vector3df diffVect=node->getAbsolutePosition()-targetPos;
		diffVect.Y=0;
		float distance=diffVect.getLength();
		if(distance<=(speed*deltaTime/1000))//reached target
		{
			goalReached=true;
			manager->setActive(this,false);//stop updating
			idle();
		}
		else
		{
			node->setRotation(faceTarget(targetPos,node->getAbsolutePosition()));
			moveto(irr::core::vector3df(0,0,speed*deltaTime/1000));
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