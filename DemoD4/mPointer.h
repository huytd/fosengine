#pragma once
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class mPointer
{
public:
	scene::IAnimatedMeshSceneNode* node;
	mPointer(void);
	void setPosition(core::vector3df pos);
	core::vector3df getPosition();
	void createMesh(irr::scene::ISceneManager *smgr,irr::video::IVideoDriver* driver);
	void setVisible(bool visi);
public:
	virtual ~mPointer(void);
};
