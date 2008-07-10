#include "StandOnTerrainAnimator.h"

namespace irr{
	namespace scene{

StandOnTerrainAnimator::StandOnTerrainAnimator(ITriangleSelector* tri,ISceneCollisionManager* cm,irr::core::vector3df gravity)
:tri(tri),cm(cm),gravity(gravity)
{
	tri->grab();
	cm->grab();
}
StandOnTerrainAnimator::~StandOnTerrainAnimator()
{
	tri->drop();
	cm->drop();
}
void StandOnTerrainAnimator::animateNode(ISceneNode* node,u32 timeMS)
{
	//node->updateAbsolutePosition();
	core::vector3df pos=node->getAbsolutePosition();
	pos+=gravity*(f32)timeMS/1000.0f;
	core::vector3df colPoint;
	core::triangle3df triangle;
	core::line3df ray(pos.X,pos.Y,pos.Z,pos.X,pos.Y+10000,pos.Z);
	bool collided=cm->getCollisionPoint  (ray,tri, colPoint,triangle);//check if we are under the terrain
	if(collided)
	{
		colPoint.Y+=1;
		pos=colPoint;//move the node above;
	}
	node->setPosition(pos);
}

}}