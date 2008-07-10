#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <SGF.h>
#include "Demo3.h"
#include <irrlicht.h>

//#include "TSTerrainSceneNode.h"

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
		//IrrlichtDevice         *irrDevice         = manager->getCore()->getGraphicDevice();
		//video::IVideoDriver      *videoDriver      = manager->getCore()->getGraphicDevice()->getVideoDriver();
		//scene::ISceneManager   *sceneManager      = manager->getCore()->getGraphicDevice()->getSceneManager();

		//// Create TSTerrainSceneNode
		//TSTerrainSceneNode *terrain = new TSTerrainSceneNode(
		//		irrDevice->getLogger(),
		//		sceneManager,
		//		irrDevice->getTimer(),
		//		"test03",                     // Mesh Name
		//		"test03.png",                  // HeightMap Image File Name
		//		core::dimension2d<f32>(1.0, 1.0),   // Stretch Size            :Same with ISceneManager::addTerrainMesh's parameter
		//		250.0f,                        // Max Height            :Same with ISceneManager::addTerrainMesh's parameter
		//		core::dimension2d<s32>(32, 32),      // Default Vertex Block Size:Same with ISceneManager::addTerrainMesh's parameter
		//		9.39f, 12.85f, 7.947f, 11.478f);   // 1 ~ 4 textures scale      :Same With scene::ITerrainSceneNode::ScaleTexture

		// Set Material Textures In SPECIFIC ORDER

		//terrain->setMaterialTexture(0, videoDriver->getTexture("test03_d.png"));   // Detail Map
		//terrain->setMaterialTexture(1, videoDriver->getTexture("grayRock.png"));   // 1st Detail Texture
		//terrain->setMaterialTexture(2, videoDriver->getTexture("hardDirt.png"));   // 2nd Detail Texture
		//terrain->setMaterialTexture(3, videoDriver->getTexture("bigRockFace.png"));   // 3rd Detail Texture
		//terrain->setMaterialTexture(4, videoDriver->getTexture("shortGrass.png"));   // 4th Detail Texture
		//terrain->setMaterialTexture(5, videoDriver->getTexture("test03_l.png"));   // Light Map
		//terrain->setMaterialTexture(6, videoDriver->getTexture("test03_c.png"));   // Color Map

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
