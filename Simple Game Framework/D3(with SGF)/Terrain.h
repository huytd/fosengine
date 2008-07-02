#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <SGF.h>
#include "Demo3.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

class Terrain: public sgfEntity, public LevelEntity<Terrain>
{

public:
	Terrain(irr::scene::ISceneNode* node)
	{
		this->node=(irr::scene::ITerrainSceneNode*)node;
	}

	void onAdd()
	{
		manager->getCore()->globalVars["terrain"] = (void*)node;

		manager->getCore()->getGraphicDevice()->getVideoDriver()->setFog(
			irr::video::SColor(0,155,155,155),  // fog color
			true, //linear fog
			100, //begin fog distance
			500, //
			0.1, //indensity
			false, 
			false);

		irr::scene::IMetaTriangleSelector* worldCollision=manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::IMetaTriangleSelector*>();
		irr::scene::ITriangleSelector* tri=manager->getCore()->getGraphicDevice()->getSceneManager()->createTerrainTriangleSelector(node,0);
		worldCollision->addTriangleSelector(tri);
		node->setTriangleSelector(tri);
		sgfPhysicWorld* world=manager->getCore()->getPhysicWorld();
		tri->drop();

		irr::scene::SMeshBufferLightMap mb;
		node->getMeshBufferForLOD(mb,2);

		node->updateAbsolutePosition();

		irr::core::matrix4 m;
		m.setTranslation(node->getPosition());
		m.setRotationDegrees(node->getRotation());
		m.setScale(node->getScale());		

	}

	void onRemove()
	{
		//! Remove collision event
		manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::IMetaTriangleSelector*>()->removeTriangleSelector(node->getTriangleSelector());

		//! Remove terrain scene node
		node->remove();
	}

protected:
	irr::scene::ITerrainSceneNode* node;

};

#endif
