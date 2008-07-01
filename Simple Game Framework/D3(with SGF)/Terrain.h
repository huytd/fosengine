#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <SGF.h>
#include "Demo3.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

#include "CGrassPatchSceneNode.h"
#include "CWindGenerator.h"

#define width  10
#define height 10
#define total  width*height

class Terrain: public sgfEntity, public LevelEntity<Terrain>
{
public:
	//core::array<video::E_MATERIAL_TYPE> modes;
	//video::ITexture *tex1;
	//video::ITexture *tex2;
	//video::IImage* heightMap;
	//bool tex;
	//video::IImage* textureMap;
	//video::IImage* grassMap  ;
	//scene::IWindGenerator *wind;

	//scene::CGrassPatchSceneNode *grass[width*height];

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

		//// create wind...
		//wind =  createWindGenerator( 30.0f, 3.0f );

		//tex1 = manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures/grass/grass.png");
		//tex2 = manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures/grass/grass2.bmp");
		//heightMap = manager->getCore()->getGraphicDevice()->getVideoDriver()->createImageFromFile("textures/editor_defaults/default_terrain_heightmap_for_grass.bmp");

		//textureMap = manager->getCore()->getGraphicDevice()->getVideoDriver()->createImageFromFile ("textures/grass/terrain-grasscol.bmp");  
		//grassMap   = manager->getCore()->getGraphicDevice()->getVideoDriver()->createImageFromFile ("textures/grass/terrain-grassmap.png"); 

		//manager->getCore()->getGraphicDevice()->getVideoDriver()->makeColorKeyTexture(tex2, core::position2d<s32>(0,0));

		////now we add some grass nodes
		//for (int x=0; x<width; ++x)
		//	for (int z=0; z<height; ++z)
		//	{   
		//		// add a grass node        
		//		grass[x*width + z] = new scene::CGrassPatchSceneNode(
		//			node, 
		//			manager->getCore()->getGraphicDevice()->getSceneManager(), 
		//			-1, 
		//			core::vector3d<s32>(x,0,z), 
		//			"grass", 
		//			heightMap, 
		//			textureMap, 
		//			grassMap, 
		//			wind);
		//		grass[x*width + z]->setMaterialFlag(video::EMF_LIGHTING,false);
		//		grass[x*width + z]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);        
		//		grass[x*width + z]->setMaterialTexture(0,tex1);
		//		grass[x*width + z]->drop();
		//	}

	}

	void onRemove()
	{
		//heightMap->drop();
		//textureMap->drop();
		//grassMap->drop();
		manager->getCore()->globalVars["worldCollision"].getAs<irr::scene::IMetaTriangleSelector*>()->removeTriangleSelector(node->getTriangleSelector());
		node->remove();
	}

protected:
	irr::scene::ITerrainSceneNode* node;
};

#endif
