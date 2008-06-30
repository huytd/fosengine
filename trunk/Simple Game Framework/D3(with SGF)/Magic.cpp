#include <SGF.h>
#include <irrlicht.h>

#include "Magic.h"


Magic::Magic()
{
//	collisionDelegate.addRef();
//	collisionDelegate.bind(this,&Magic::onCollision);
}

void Magic::attack( u32 startTime, //don't use param
				   irr::core::vector3df startPosition, //the magic start position
				   irr::core::vector3df StartTargetPostion, //the target position
				   float startIdleTime, float startPreTime, float startTTL,  
				   float startBlowTime, float initSpeed, float initSpeedDelta)
{

	isAttacking = true;

	idleTime = startIdleTime;

	irr::IrrlichtDevice* device = manager->getCore()->getGraphicDevice();
	beginTime =  device->getTimer()->getRealTime();	

	vTargetPostion = StartTargetPostion;
	vPostition = startPosition;

	vTargetPostion.Y += 2.0f;
	vPostition.Y += 6.0f;

	magicLight->setPosition(vPostition);
	magicLight->setVisible(false);

	TTL = startTTL;
	preTime = startPreTime;
	blowTime = startBlowTime;
	speed = initSpeed;
	speedDelta = initSpeedDelta;

	CurrentAction = "Start";
}

//! Set magic movement speed
void Magic::setSpeed(float newspeed)
{
	speed = newspeed;
}

//! Set magic movement increase speed
void Magic::setSpeedDelta(float newspeeddelta)
{
	speedDelta = newspeeddelta;
}

const char* Magic::getClassName() const
{
	return "Magic";
}

void Magic::setPosition(core::vector3df pos)
{
	//ps->setPosition(pos);
}

bool Magic::isEnd()
{
	if(CurrentAction == "End") return true;
	return false;
}

core::vector3df Magic::psfaceTarget(irr::core::vector3df targetpos, irr::core::vector3df nodepos)
{
	irr::core::vector3df posDiff = targetpos - nodepos;
	posDiff.normalize();
	return posDiff.getHorizontalAngle();
} 

//! Arras's code
void Magic::psMoveTo(irr::scene::ISceneNode *node, //node to move
					 irr::core::vector3df vel) //velocity vector
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	m.transformVect(vel);
	node->setPosition(node->getPosition() + vel);
	node->updateAbsolutePosition();
} 

void Magic::onLevelStart()
{
}

void Magic::onCollision(SCollisionEvent& arg)
{
}

void Magic::onAdd()
{

	isAttacking = false;

	// add light 2 (gray)
	magicLight = manager->getCore()->getGraphicDevice()->getSceneManager()->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(1.0f, 0.2f, 0.2f, 0.0f), 100.0f);

	// attach billboard to light
	bill = manager->getCore()->getGraphicDevice()->getSceneManager()->addBillboardSceneNode(magicLight, core::dimension2d<f32>(8, 8));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\particlewhite.bmp"));

	// add particle system
	ps = manager->getCore()->getGraphicDevice()->getSceneManager()->addParticleSystemSceneNode(false, magicLight);
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
	ps->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\lighteffect.bmp"));
	
	magicLight->setVisible(false);

	CurrentAction = "End";
	
	//! make update called every frame.
	manager->setActive(this,true);
}


void Magic::onMouse(SMouseEvent& args)
{
}

void Magic::update(float deltaTime)
{

	if (!isAttacking) return;

	irr::IrrlichtDevice* device = manager->getCore()->getGraphicDevice();

	u32 tickTime = device->getTimer()->getRealTime();
	u32 subTime = tickTime - beginTime;

	if(subTime < idleTime) //Do nothing
	{
		//Do nothing in idle time
		if(magicLight->isVisible())
			magicLight->setVisible(false);
	}
	else if(subTime < preTime + idleTime) //Make the magic bigger or change color or animated it
	{
		//Scalar

		//magicLight->setScale(newScale);
		if(!magicLight->isVisible())
			magicLight->setVisible(true);

		//Rotate

		//Animation
	}
	else if(subTime < TTL + preTime + idleTime) //The magic is fired, fly to the enemy
	{
		//! Target is reached
		if( magicLight->getPosition().getDistanceFrom(vTargetPostion) <= speed*deltaTime) 
		{
			isAttacking = false;	
			magicLight->setVisible(false);
			CurrentAction = "End";
			return;
		}

		//! Fly until TTL
		magicLight->setRotation( psfaceTarget(vTargetPostion, magicLight->getPosition()));

		speed += speedDelta;

		//! TODO: Change funtion to define magic's quy dao
		psMoveTo(magicLight, core::vector3df(0.0f, 0.0f, speed*deltaTime));

	}
	else
	{
		isAttacking = false;	
		magicLight->setVisible(false);
	}
}

void Magic::onRemove()
{

	if(ps)
	{
		ps->remove();
		//ps->drop();
	}

	if(bill)
	{
		bill->remove();
		//bill->drop();
	}

	if(magicLight)
	{
		magicLight->remove();
		//magicLight->drop();
	}

	//if(anim)
	{
		//anim->remove();
	}
}