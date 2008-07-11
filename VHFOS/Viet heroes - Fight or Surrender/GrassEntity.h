#ifndef _GRASS_H_
#define _GRASS_H_

#include <SGF.h>
#include "Demo3.h"
#include <irrlicht.h>

using namespace irr;
using namespace scene;
using namespace core;

#include "CGrassPatchSceneNode.h"
#include "CWindGenerator.h"


#define width  5
#define height 5
#define total  width*height

class Grass: public sgfEntity
{
public:
	core::array<video::E_MATERIAL_TYPE> modes;
	video::ITexture *tex1;
	video::ITexture *tex2;
	video::IImage* heightMap;
	bool tex;
	video::IImage* textureMap;
	video::IImage* grassMap  ;
	scene::IWindGenerator *wind;

	scene::CGrassPatchSceneNode *grass[width*height];
	irr::scene::ITerrainSceneNode* terrain;

public:
	Grass()
	{

	}

	void onAdd()
	{
		terrain = manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>();//get the terrain from a global variable
		
		// create wind...
		wind =  createWindGenerator( 30.0f, 3.0f );

		tex1 = manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\grass\\grass.png");
		tex2 = manager->getCore()->getGraphicDevice()->getVideoDriver()->getTexture("textures\\grass\\grass2.bmp");

		heightMap = manager->getCore()->getGraphicDevice()->getVideoDriver()->createImageFromFile("textures\\terrain\\terrain-heightmap.bmp");
		textureMap = manager->getCore()->getGraphicDevice()->getVideoDriver()->createImageFromFile ("textures\\grass\\terrain-grasscol.bmp");  
		grassMap   = manager->getCore()->getGraphicDevice()->getVideoDriver()->createImageFromFile ("textures\\grass\\terrain-grassmap.png"); 

		manager->getCore()->getGraphicDevice()->getVideoDriver()->makeColorKeyTexture(tex2, core::position2d<s32>(0,0));

		if(terrain)
		{
		//now we add some grass nodes
		for (int x=0; x<width; ++x)
			for (int z=0; z<height; ++z)
			{   
				// add a grass node        
				grass[x*width + z] = new scene::CGrassPatchSceneNode(
					manager->getCore()->globalVars["terrain"].getAs<irr::scene::ITerrainSceneNode*>(), 
					manager->getCore()->getGraphicDevice()->getSceneManager(), 
					-1, 
					core::vector3d<s32>(x,0,z), 
					"grass", 
					heightMap, 
					textureMap, 
					grassMap, 
					wind);
				grass[x*width + z]->setMaterialFlag(video::EMF_LIGHTING,false);
				grass[x*width + z]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);        
				grass[x*width + z]->setMaterialTexture(0,tex1);
				grass[x*width + z]->drop();
			}
		}

	}

	void onRemove()
	{
		//! Remove resource for wind and grass
		if(heightMap) heightMap->drop();
		if(textureMap) textureMap->drop();
		if(grassMap) grassMap->drop();		
	}

};

#endif //_GRASS_H_
