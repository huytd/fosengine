/*
 ***********************************************
 * Character control
 * *********************************************
 * file name: ThayCung.h
 * encoding: UTF-8
 * tab size: 8
 * indentation: 4
 * created on: 8:55 PM 3/17/2008
 * init by: Do Quoc Khanh - doqkhanh
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

class ThayCung : public irr::scene::IAnimationEndCallBack 
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
	ThayCung(IrrlichtDevice* device, irr::scene::ISceneManager *smgr,irr::video::IVideoDriver *videodriver, char* meshPath);
	~ThayCung(void);
	
	void setState(eState newState);
	void setPosition(core::vector3df pos);
	void setRotate(core::vector3df rot);
	
	core::vector3df getPosition();
	core::vector3df getRotate();
	eState getState();
	irr::scene::IAnimatedMeshSceneNode* getNode();

	void run();
	void walk();
	void attack(eAttacktype);
	void idle();

	void move(core::vector3df pos);
	void update();

	void remove();

	void OnAnimationEnd(IAnimatedMeshSceneNode* node);
};
