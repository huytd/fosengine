#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "SGF.h"
#include "Demo3.h"
#include <irrlicht.h>

class Terrain: public sgfEntity
{
public:
	static sgfEntity* createFromNode(irr::scene::ISceneNode* node)
	{
		return new Terrain(node);
	}

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