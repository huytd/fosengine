#include "JointAnimator.h"

namespace irr{
	namespace scene{

void JointAnimator::animateNode(ISceneNode* node,u32 timeMS)
{
	((IAnimatedMeshSceneNode*)node)->animateJoints();
}

}}