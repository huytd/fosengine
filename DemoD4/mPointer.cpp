#include "mPointer.h"
#include <irrlicht.h>

mPointer::mPointer(void)
{
}

mPointer::~mPointer(void)
{
}

core::vector3df mPointer::getPosition()
{
	return node->getPosition();
}

void mPointer::createMesh(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver* driver)
{
	node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media\\model\\pointer\\pointer.b3d"));
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setScale(vector3df(0.1f,0.1f,0.1f));
	node->setMaterialTexture( 0, driver->getTexture("media\\model\\pointer\\point.bmp") );
	
	scene::ISceneNodeAnimator* anim=smgr->createRotationAnimator(vector3df(0,2,0));
	node->addAnimator(anim);
	anim->drop();
}

void mPointer::setPosition(irr::core::vector3df pos)
{
	node->setPosition(pos);
}

void mPointer::setVisible(bool visi)
{
	node->setVisible(visi);
}