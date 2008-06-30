#include <SGF.h>
#include <irrlicht.h>

#include "Magic.h"


Magic::Magic()
{
//	collisionDelegate.addRef();
//	collisionDelegate.bind(this,&Magic::onCollision);
}

void Magic::attack( u32 startTime, irr::core::vector3df startPosition,
				   irr::core::vector3df StartTargetPostion,
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

	light2->setPosition(vPostition);
	light2->setVisible(false);

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
	ps->setPosition(pos);
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

//! arras code
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
	light2 = manager->getCore()->getGraphicDevice()->getSceneManager()->addLightSceneNode(0, core::vector3df(0,0,0),
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
	ps->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\lighteffect.bmp"));
	
	light2->setVisible(false);

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
		if( light2->getPosition().getDistanceFrom(vTargetPostion) <= speed*deltaTime) 
		{
			isAttacking = false;	
			light2->setVisible(false);
			CurrentAction = "End";
			return;
		}

		//Fly in TTL
		light2->setRotation( psfaceTarget(vTargetPostion, light2->getPosition()));

		speed += speedDelta;

		//Change funtion to define magic's quy dao
		psMoveTo(light2, core::vector3df(0.0f, 0.0f, speed*deltaTime));

	}
	else
	{
		isAttacking = false;	
		light2->setVisible(false);
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

	if(light2)
	{
		light2->remove();
		//light2->drop();
	}

	if(anim)
	{
		//anim->remove();
	}


}


