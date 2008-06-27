#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <ITerrainSceneNode.h>

class Game;

class Terrain
{

private:
	
	irr::scene::ITerrainSceneNode* terrain;

public:

	Terrain(Game* game);

	~Terrain()
	{
		this->terrain->remove();
		this->terrain->drop();
	}
	

	inline irr::scene::ITerrainSceneNode* getTerrain() const { return this->terrain; }

	
};

#endif /*TERRAIN_H_*/
