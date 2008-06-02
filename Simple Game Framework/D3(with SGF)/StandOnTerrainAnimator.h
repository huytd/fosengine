#ifndef _STAND_ON_TERRAIN_ANIMATOR_H_
#define _STAND_ON_TERRAIN_ANIMATOR_H_

#include <irrlicht.h>

namespace irr{
	namespace scene{

class StandOnTerrainAnimator:public ISceneNodeAnimator
{
public:
	StandOnTerrainAnimator(ITriangleSelector* tri,ISceneCollisionManager* cm,irr::core::vector3df gravity);
	~StandOnTerrainAnimator();
	void animateNode(ISceneNode* node,u32 timeMS);
private:
	irr::core::vector3df gravity;
	ITriangleSelector* tri;
	ISceneCollisionManager* cm;
};

}}

#endif