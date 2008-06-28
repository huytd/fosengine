#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <SGF.h>
#include "Demo3.h"
#include <irrlicht.h>

class Terrain: public sgfEntity, public LevelEntity<Terrain>
{
public:
	Terrain(irr::scene::ISceneNode* node)
	{
		this->node=(irr::scene::ITerrainSceneNode*)node;
	}

	void onAdd()
	{
		manager->getCore()->globalVars["terrain"]=(void*)node;
		//node->setMaterialFlag(irr::video::EMF_WIREFRAME,true);
		//node->
		irr::scene::IMetaTriangleSelector* worldCollision=manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::IMetaTriangleSelector*>();
		irr::scene::ITriangleSelector* tri=manager->getCore()->getGraphicDevice()->getSceneManager()->createTerrainTriangleSelector(node,0);
		worldCollision->addTriangleSelector(tri);
		node->setTriangleSelector(tri);
		sgfPhysicWorld* world=manager->getCore()->getPhysicWorld();
		irr::scene::SMeshBufferLightMap mb;
		node->getMeshBufferForLOD(mb,2);
		node->updateAbsolutePosition();
		irr::core::matrix4 m;
		m.setTranslation(node->getPosition());
		m.setRotationDegrees(node->getRotation());
		m.setScale(node->getScale());
		//printf("%f %f %f\n",m.getScale().X,m.getScale().Y,m.getScale().Z);
		
		//(new sgfPhysicDebugger(manager->getCore()->getGraphicDevice()->getSceneManager(),body))->drop();
		tri->drop();
	}
	void onRemove()
	{
		manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::IMetaTriangleSelector*>()->removeTriangleSelector(node->getTriangleSelector());
		node->remove();
	}
protected:
	irr::scene::ITerrainSceneNode* node;
};

#endif
