#ifndef _MAGIC_H_
#define _MAGIC_H_

#include <SGF.h>
#include "JointAnimator.h"
#include "StandOnTerrainAnimator.h"
#include "ThirdPersonCamera.h"
#include "HUDControler.h"
#include "Icon.h"
#include "Map.h"
#include <irrlicht.h>

class Magic: public sgfEntity
{
public:

	void attack( u32 startTime, irr::core::vector3df startPosition,
		irr::core::vector3df StartTargetPostion,
		float startIdleTime=1000.0f, float startPreTime=1000.0f, float startTTL=30000.0f,  float startBlowTime=1000.0f)
	{
		
		isAttack = true;

		idleTime = startIdleTime;

		irr::IrrlichtDevice* device = manager->getCore()->getGraphicDevice();
		beginTime =  device->getTimer()->getRealTime();

		vTargetPostion = StartTargetPostion;
		vPostition = startPosition;

		vTargetPostion.Y += 2.0f;
		vPostition.Y += 6.0f;

		light2->setPosition(vPostition);
		light2->setVisible(false);

		TTL = startTTL;
		preTime = startPreTime;
		blowTime = startBlowTime;
	}

	Magic()
	{
		collisionDelegate.addRef();
		collisionDelegate.bind(this,&Magic::onCollision);
	}

	//! Set magic movement speed
	void setSpeed(float newspeed)
	{
		speed = newspeed;
	}

	const char* getClassName() const
	{
		return "Magic";
	}

	void setPosition(core::vector3df pos)
	{
		ps->setPosition(pos);
	}
private:
	core::vector3df psfaceTarget(irr::core::vector3df targetpos, irr::core::vector3df nodepos)
	{
		irr::core::vector3df posDiff = targetpos - nodepos;
		posDiff.normalize();
		return posDiff.getHorizontalAngle();
	} 

	//! arras code
	void psMoveTo(irr::scene::ISceneNode *node, //node to move
		irr::core::vector3df vel) //velocity vector
	{
		irr::core::matrix4 m;
		m.setRotationDegrees(node->getRotation());
		m.transformVect(vel);
		node->setPosition(node->getPosition() + vel);
		node->updateAbsolutePosition();
	} 
protected:
	void onLevelStart()
	{
	}

	void onCollision(SCollisionEvent& arg)
	{
	}

	void onAdd()
	{

		isAttack = false;

		// add light 2 (gray)
		light2 =
			manager->getCore()->getGraphicDevice()->getSceneManager()->addLightSceneNode(0, core::vector3df(0,0,0),
			video::SColorf(1.0f, 0.2f, 0.2f, 0.0f), 100.0f);

		// attach billboard to light
		bill = manager->getCore()->getGraphicDevice()->getSceneManager()->addBillboardSceneNode(light2, core::dimension2d<f32>(8, 8));
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		bill->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\particlewhite.bmp"));

		// add particle system
		ps = manager->getCore()->getGraphicDevice()->getSceneManager()->addParticleSystemSceneNode(false, light2);
		ps->setParticleSize(core::dimension2d<f32>(5.0f, 5.0f));

		// create and set emitter
		em = ps->createBoxEmitter(
			core::aabbox3d<f32>(-1,0,-1,1,1,1), 
			core::vector3df(0.0f,0.0f,0.0f),
			100,100, 
			video::SColor(0,255,255,255), video::SColor(0,255,255,255),
			100,300);
		ps->setEmitter(em);
		em->drop();

		// create and set affector
		paf = ps->createFadeOutParticleAffector();
		ps->addAffector(paf);
		paf->drop();

		// adjust some material settings
		ps->setMaterialFlag(video::EMF_LIGHTING, false);
		ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\lighteffect.bmp"));
		ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

		light2->setVisible(false);

		//! make update called every frame.
		manager->setActive(this,true);
	}

	
	void onMouse(SMouseEvent& args)
	{
	}

	void update(float deltaTime)
	{

		if (!isAttack) return;

		irr::IrrlichtDevice* device = manager->getCore()->getGraphicDevice();

		u32 tickTime = device->getTimer()->getRealTime();
		u32 subTime = tickTime - beginTime;

		if(subTime < idleTime)
		{
			//Do nothing in idle time
			if(light2->isVisible())
				light2->setVisible(false);
		}
		else if(subTime < preTime + idleTime)
		{
			//Scalar

			//light2->setScale(newScale);
			if(!light2->isVisible())
				light2->setVisible(true);

			//Rotate

			//Animation
		}
		else if(subTime < TTL + preTime + idleTime)
		{
			if( light2->getPosition().getDistanceFrom(vTargetPostion) <= 50*deltaTime) 
			{
				isAttack = false;	
				light2->setVisible(false);
				return;
			}

			//Fly in TTL
			light2->setRotation( psfaceTarget(vTargetPostion, light2->getPosition()));

			//Change funtion to define magic's quy dao
			psMoveTo(light2, core::vector3df(0.0f, 0.0f, 50*deltaTime));

		}
		else
		{
			isAttack = false;	
			light2->setVisible(false);
		}
	}

	void onRemove()
	{
	}


protected:
	float speed;
	bool goalReached;
	char* CurrentAction;
	irr::scene::ISceneNode* node;

	irr::core::vector3df vTargetPostion, vPostition;
	float TTL, preTime, blowTime, idleTime;
	bool isAttack;

	//Real time when start magic
	irr::u32 beginTime;

	irr::scene::ISceneNode* light2;
	irr::scene::IParticleSystemSceneNode* ps;
	irr::scene::IParticleEmitter* em;
	irr::scene::IParticleAffector* paf;

	irr::scene::ISceneNode* bill;
	irr::scene::ISceneNodeAnimator* anim;

	sgfMethodDelegate<Magic,SCollisionEvent> collisionDelegate;
};

#endif