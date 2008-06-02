#ifndef _JOINT_ANIMATOR_H_
#define _JOINT_ANIMATOR_H_

#include <irrlicht.h>

namespace irr{
	namespace scene{

class JointAnimator:public ISceneNodeAnimator
{
public:
	void animateNode(ISceneNode* node,u32 timeMS);
};

}}

#endif