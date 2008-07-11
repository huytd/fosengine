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
	//! Reconize attack type to process update
	attackType = POINT_TO_POINT_ATTACK;

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

/**
 * \Summary: Node to point magic attack
 *
 */
void Magic::attack( irr::scene::ISceneNode* attackerSceneNode,
					irr::core::vector3df targetPostion,
					float startIdleTime, float startPreTime, float startTTL,  
					float startBlowTime, float initSpeed, float initSpeedDelta)
{
	//! Reconize attack type to process update
	attackType = NODE_TO_POINT_ATTACK;

	//! Set magic status
	CurrentAction = MAGIC_STARTED;

	//! Turn on attacking flag
	isAttacking = true;
	
	//! Set target node
	targetNode = attackerSceneNode;

	vTargetPostion = targetPostion;
	vTargetPostion.Y += 2.0f;
	
	magicLight->setPosition(targetNode->getAbsolutePosition());
	magicLight->setVisible(false);

	//! Set idle time
	idleTime = startIdleTime;

	//! Set magic begin time
	irr::IrrlichtDevice* device = manager->getCore()->getGraphicDevice();
	beginTime =  device->getTimer()->getRealTime();	

	//! Setting magic timer
	TTL = startTTL;
	preTime = startPreTime;
	blowTime = startBlowTime;
	speed = initSpeed;
	speedDelta = initSpeedDelta;
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

void Magic::setStartPosition(irr::core::vector3df newStartPosition)
{
	vPostition = newStartPosition;
}

//! This method is Arras's code, many thank to Arras at Irrlicht Forum
void Magic::psMoveTo(irr::scene::ISceneNode *node, //node to move
					 irr::core::vector3df vel) //velocity vector
{
	irr::core::matrix4 m;
	m.setRotationDegrees(node->getRotation());
	m.transformVect(vel);
	node->setPosition(node->getPosition() + vel);
	node->updateAbsolutePosition();
} 

void Magic::onAdd()
{

	//! Turn off attacking flag
	isAttacking = false;

	// add light 2 (gray)
	magicLight = manager->getCore()->getGraphicDevice()->getSceneManager()->addLightSceneNode(
		0, //parent
		core::vector3df(0,0,0), //position
		video::SColorf(1.0f, 0.2f, 0.2f, 0.0f), //color
		100.0f);//radius

	// attach billboard to light
	bill = manager->getCore()->getGraphicDevice()->getSceneManager()->addBillboardSceneNode(magicLight, core::dimension2d<f32>(8, 8));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\particlewhite.bmp"));

	// add particle system
	ps = manager->getCore()->getGraphicDevice()->getSceneManager()->addParticleSystemSceneNode(false, magicLight);
	ps->setParticleSize(core::dimension2d<f32>(2.0f, 2.0f));
	
	// adjust some material settings
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\particlewhite.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	// create and set emitter
	em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-1,0,-1,1,1,1), 
		core::vector3df(0.0f,0.0f,0.0f),
		70,70, 
		video::SColor(0,255,255,255), video::SColor(0,255,255,255),
		500,1100);
	ps->setEmitter(em);
	em->drop();

	// create and set affector
	paf = ps->createFadeOutParticleAffector();
	ps->addAffector(paf);
	paf->drop();

	// adjust some material settings	
	ps->setMaterialTexture(0, manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\lighteffect.bmp"));
	
	//! Turn off magic light
	magicLight->setVisible(false);

	//! Set magic statud
	CurrentAction = MAGIC_ENDED;
	
	//! make update called every frame.
	manager->setActive(this,true);
}



void Magic::update(float deltaTime)
{

	if (!isAttacking) return;

	switch(attackType)
	{
		case POINT_TO_POINT_ATTACK:
		{
				device = manager->getCore()->getGraphicDevice();

				tickTime = device->getTimer()->getRealTime();
				subTime = tickTime - beginTime;

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
					magicLight->setPosition(this->vPostition);
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

			/////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////	
			case NODE_TO_POINT_ATTACK:
			{
				device = manager->getCore()->getGraphicDevice();

				tickTime = device->getTimer()->getRealTime();
				subTime = tickTime - beginTime;

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
					//magicLight->setPosition(targetNode->getAbsolutePosition());
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
	}
	
}

void Magic::onRemove()
{
}