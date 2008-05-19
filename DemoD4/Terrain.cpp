#include "Terrain.h"
#include "Game.h"

Terrain::Terrain(Game* game) :
	terrain(0)
{
	irr::scene::ISceneManager* smgr = game->getSceneManager();
	irr::video::IVideoDriver* driver = game->getVideoDriver();

	// add terrain scene node
	terrain = smgr->addTerrainSceneNode( 
		"media/images/terrain/default/terrain-heightmap.bmp",
		0,										// parent node
		-1,										// node id
		irr::core::vector3df(0.f, 0.f, 0.f),	// position
		irr::core::vector3df(0.f, 0.f, 0.f),	// rotation
		irr::core::vector3df(40.f, 4.4f, 40.f),	// scale
		irr::video::SColor ( 255, 255, 255, 255 ),	// vertexColor,
		5,										// maxLOD
		irr::scene::ETPS_17,					// patchSize
		5										// smoothFactor
	);

	this->terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	this->terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	this->terrain->setMaterialFlag(irr::video::EMF_FOG_ENABLE, false);
	this->terrain->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	this->terrain->setMaterialTexture(0, driver->getTexture("media/images/terrain/default/terrain-texture.jpg"));
	this->terrain->setMaterialTexture(1, driver->getTexture("media/images/terrain/default/detailmap3.jpg"));
	this->terrain->scaleTexture(5.0f, 250.0f);
	this->terrain->grab();
	
}
