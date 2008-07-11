#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <SGF.h>
#include "Demo3.h"
#include <irrlicht.h>

//#include "TSTerrainSceneNode.h"
#include "RealisticWater.h"

//! Testing
#include "GrassEntity.h"

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


		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//IrrlichtDevice         *irrDevice         = manager->getCore()->getGraphicDevice();
		//video::IVideoDriver    *videoDriver      = manager->getCore()->getGraphicDevice()->getVideoDriver();
		//scene::ISceneManager   *sceneManager      = manager->getCore()->getGraphicDevice()->getSceneManager();

		//// add terrain scene node
		//scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("media/terrain-heightmap.bmp");
		//video::IImage* heightMap  = driver->createImageFromFile ("media/terrain-heightmap.bmp");  
		//video::IImage* textureMap = driver->createImageFromFile ("media/terrain-grasscol.bmp");  
		//video::IImage* grassMap   = driver->createImageFromFile ("media/terrain-grassmap.png"); 

		//core::vector3df ts(40, 4.0f, 40);
		//terrain->setScale(ts);
		//terrain->setPosition( core::vector3df(0,0,0) );

		//terrain->setMaterialFlag(video::EMF_LIGHTING, false);

		//terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
		//terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg")); 

		//terrain->setMaterialType(video::EMT_DETAIL_MAP);

		//terrain->scaleTexture(1.0f, 20.0f);


		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////

		//! Testing water
		//! RUN OK but there are 2 error: shader is bad and memory error after quit program
		//water = new RealisticWaterSceneNode(
		//		manager->getCore()->getGraphicDevice()->getSceneManager(), 
		//		1024.0f, 
		//		1024.0f,
		//		manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("data/waterbump.dds"), 
		//		manager->getCore()->getGraphicDevice()->getTimer()
		//		
		//		);




		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////

		//! Temporary  adding skydome here for testing only
		manager->getCore()->getGraphicDevice()->getSceneManager()->addSkyDomeSceneNode(
			manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures/skydome/dayfair.jpg"), // this is the path to the texture you want to use for your sphere
			32,          // 16 rows from top to bottom
			32,          // rectangles each row
			1.0f,        // percent of the texture used, example 0.9f will only use the top 90% of the image file (range 0.0f-1.0f)
			1.0f);       // percent of the sphere to cover 1.0f covers the top half of the sphere, while 2.0f covers the entire sphere



		//Try playing around with the values and increase and decrease the rows and rectangles each row to see the effect 
		//it has on your skydome,, also try to adjust the last value from 0.1f to 2.0f to see how the skydome covers which portion of the sky. 
		manager->getCore()->getGraphicDevice()->getSceneManager()->addSkyDomeSceneNode(
			manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures/skydome/dayfair.jpg"),
			32,
			32,
			1.0f,
			1.1f);


		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////


		/**
		*	Test grass
		*/

		//IrrlichtDevice         *irrDevice         = manager->getCore()->getGraphicDevice();
		//video::IVideoDriver    *videoDriver      = manager->getCore()->getGraphicDevice()->getVideoDriver();
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

		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		manager->getCore()->globalVars["terrain"] = (void*)node;

		//! Test fog
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



		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		node->updateAbsolutePosition();

		irr::core::matrix4 m;
		m.setTranslation(node->getPosition());
		m.setRotationDegrees(node->getRotation());
		m.setScale(node->getScale());

		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////

		//! Add here for testing purpose
		//grass = new Grass();
		//manager->addEntity(grass);

	}

	void onRemove()
	{
		//water->remove();

		//! Remove collision event
		manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::IMetaTriangleSelector*>()->removeTriangleSelector(node->getTriangleSelector());

		//! Remove terrain scene node
		node->remove();
	}

protected:
	irr::scene::ITerrainSceneNode* node;
	//RealisticWaterSceneNode* water;

	//! Testing
	Grass* grass;

};

#endif
