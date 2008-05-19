/*
 ***********************************************
 * Character control
 * *********************************************
 * file name: CCharacter.h
 * encoding: UTF-8
 * tab size: 8
 * indentation: 4
 * created on: 8:55 PM 3/17/2008
 * created by: FOSP Team
 * copyright: FOS Project
 */

#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CCharacter : public irr::scene::IAnimationEndCallBack 
{
private:
	float fSpeed;
	scene::IAnimatedMeshSceneNode* node;


public:
	enum eAttacktype{
		MagicAttack,
		PowerAttack,
		DownswipeAttack
	};

	enum eState{
		Run,
		Walk, 
		Attack,
		Idle
	};	
	
	eAttacktype curAttackType;
	eState curState;
	eState oldState;

public:
	CCharacter(IrrlichtDevice* device, irr::scene::ISceneManager *smgr,irr::video::IVideoDriver *videodriver, char* meshPath);
	~CCharacter(void);
	
	void setState(eState newState);
	void setPosition(core::vector3df pos);
	void setRotate(core::vector3df rot);
	
	core::vector3df getPosition();
	core::vector3df getRotate();
	eState getState();
	irr::scene::IAnimatedMeshSceneNode* getMesh();

	void run();
	void walk();
	void attack(eAttacktype);
	void idle();

	void move(core::vector3df pos);
	void update();

	void remove();

	void OnAnimationEnd(IAnimatedMeshSceneNode* node);
};
