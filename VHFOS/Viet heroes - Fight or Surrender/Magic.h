#ifndef _MAGIC_H_
#define _MAGIC_H_

#include <SGF.h>
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

//! Define attack type cho nhanh
#define POINT_TO_POINT_ATTACK 1
#define NODE_TO_POINT_ATTACK  2
#define NODE_TO_NODE_ATTACK   3

//! Define attack status
#define MAGIC_STARTED "Start"
#define MAGIC_ENDED "End"

class Magic: public sgfEntity
{
public:

	//! The target scene node to update position
	irr::scene::ISceneNode* targetNode;

	//! The magic speed and increase speed degree
	float speed, speedDelta;

	//! Is gold reached?
	bool goalReached;

	//! The string store current magic phase
	//! TODO: replace this string with enum type
	char* CurrentAction;

	//! Magic position and enemy position
	irr::core::vector3df vTargetPostion, vPostition;

	//! The time in each magic phase
	float TTL, preTime, blowTime, idleTime;

	//! Is the magic in attacking status
	bool isAttacking;

	//! Attack type 
	int attackType;

	//Real time when start magic
	irr::u32 beginTime;

	//! The magic element :)
	irr::scene::ISceneNode* magicLight;
	irr::scene::IParticleSystemSceneNode* ps;

	irr::scene::IParticleEmitter* em;
	irr::scene::IParticleAffector* paf;
	irr::scene::ISceneNode* bill;
	//irr::scene::ISceneNodeAnimator* anim;

	//sgfMethodDelegate<Magic,SCollisionEvent> collisionDelegate;

	//! For a faster update method
	irr::IrrlichtDevice* device;
	u32 tickTime; // current tick cout
	u32 subTime;  // the time from magic start to current time

private:
	//! Get the rotation to face target to enemy
	core::vector3df psfaceTarget(irr::core::vector3df targetpos, irr::core::vector3df nodepos);

	//! arras code
	void psMoveTo(irr::scene::ISceneNode *node, //node to move
		irr::core::vector3df vel); //velocity vector

public:
	//! Default constructor
	Magic();
		
	/**
	 *
	 *
	 */
	void attack( u32 startTime, irr::core::vector3df startPosition,
		irr::core::vector3df StartTargetPostion,
		float startIdleTime=1000.0f, float startPreTime=1000.0f, float startTTL=30000.0f,  
		float startBlowTime=1000.0f, float initSpeed = 70.0f, float initSpeedDelta = 1.0f);

	/**
	 *
	 *
	 */
	void attack(irr::scene::ISceneNode* attackerSceneNode,
		irr::core::vector3df StartTargetPostion,
		float startIdleTime=1000.0f, float startPreTime=1000.0f, float startTTL=30000.0f,  
		float startBlowTime=1000.0f, float initSpeed = 70.0f, float initSpeedDelta = 1.0f);

	//! Set start position to update magic position in idle phase
	void setStartPosition(irr::core::vector3df newStartPosition);


	//! Set magic movement speed
	void setSpeed(float newspeed);

	//! Set magic movement increase speed
	void setSpeedDelta(float newspeeddelta);

	const char* getClassName() const;

	void setPosition(core::vector3df pos);

	//! Is the magic end, mean it has been reached the target
	bool isEnd();

protected:
	void onAdd();	
	void update(float deltaTime);
	void onRemove();

};

#endif